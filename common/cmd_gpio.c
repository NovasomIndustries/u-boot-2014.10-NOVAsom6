/*
 * Control GPIO pins on the fly
 *
 * Copyright (c) 2008-2011 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <command.h>
#include <errno.h>
#include <dm.h>
#include <asm/gpio.h>

int __weak name_to_gpio(const char *name)
{
	return simple_strtoul(name, NULL, 10);
}

enum gpio_cmd {
	GPIO_INPUT,
	GPIO_SET,
	GPIO_CLEAR,
	GPIO_TOGGLE,
};

#if defined(CONFIG_DM_GPIO) && !defined(gpio_status)
static const char * const gpio_function[GPIOF_COUNT] = {
	"input",
	"output",
	"unused",
	"unknown",
	"func",
};

/* A few flags used by show_gpio() */
enum {
	FLAG_SHOW_ALL		= 1 << 0,
	FLAG_SHOW_BANK		= 1 << 1,
	FLAG_SHOW_NEWLINE	= 1 << 2,
};

static void show_gpio(struct udevice *dev, const char *bank_name, int offset,
		      int *flagsp)
{
	struct dm_gpio_ops *ops = gpio_get_ops(dev);
	int func = GPIOF_UNKNOWN;
	char buf[80];
	int ret;

	BUILD_BUG_ON(GPIOF_COUNT != ARRAY_SIZE(gpio_function));

	if (ops->get_function) {
		ret = ops->get_function(dev, offset);
		if (ret >= 0 && ret < ARRAY_SIZE(gpio_function))
			func = ret;
	}
	if (!(*flagsp & FLAG_SHOW_ALL) && func == GPIOF_UNUSED)
		return;
	if ((*flagsp & FLAG_SHOW_BANK) && bank_name) {
		if (*flagsp & FLAG_SHOW_NEWLINE) {
			putc('\n');
			*flagsp &= ~FLAG_SHOW_NEWLINE;
		}
		printf("Bank %s:\n", bank_name);
		*flagsp &= ~FLAG_SHOW_BANK;
	}
	*buf = '\0';
	if (ops->get_state) {
		ret = ops->get_state(dev, offset, buf, sizeof(buf));
		if (ret) {
			puts("<unknown>");
			return;
		}
	} else {
		sprintf(buf, "%s%u: %8s %d", bank_name, offset,
			gpio_function[func], ops->get_value(dev, offset));
	}

	puts(buf);
	puts("\n");
}

static int do_gpio_status(bool all, const char *gpio_name)
{
	struct udevice *dev;
	int banklen;
	int flags;
	int ret;

	flags = 0;
	if (gpio_name && !*gpio_name)
		gpio_name = NULL;
	for (ret = uclass_first_device(UCLASS_GPIO, &dev);
	     dev;
	     ret = uclass_next_device(&dev)) {
		const char *bank_name;
		int num_bits;

		flags |= FLAG_SHOW_BANK;
		if (all)
			flags |= FLAG_SHOW_ALL;
		bank_name = gpio_get_bank_info(dev, &num_bits);
		if (!num_bits)
			continue;
		banklen = bank_name ? strlen(bank_name) : 0;

		if (!gpio_name || !bank_name ||
		    !strncmp(gpio_name, bank_name, banklen)) {
			const char *p = NULL;
			int offset;

			p = gpio_name + banklen;
			if (gpio_name && *p) {
				offset = simple_strtoul(p, NULL, 10);
				show_gpio(dev, bank_name, offset, &flags);
			} else {
				for (offset = 0; offset < num_bits; offset++) {
					show_gpio(dev, bank_name, offset,
						  &flags);
				}
			}
		}
		/* Add a newline between bank names */
		if (!(flags & FLAG_SHOW_BANK))
			flags |= FLAG_SHOW_NEWLINE;
	}

	return ret;
}
#endif

static int do_gpio(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	unsigned int gpio;
	enum gpio_cmd sub_cmd;
	ulong value;
	const char *str_cmd, *str_gpio = NULL;
	int ret;
#ifdef CONFIG_DM_GPIO
	bool all = false;
#endif

	if (argc < 2)
 show_usage:
		return CMD_RET_USAGE;
	str_cmd = argv[1];
	argc -= 2;
	argv += 2;
#ifdef CONFIG_DM_GPIO
	if (argc > 0 && !strcmp(*argv, "-a")) {
		all = true;
		argc--;
		argv++;
	}
#endif
	if (argc > 0)
		str_gpio = *argv;
	if (!strcmp(str_cmd, "status")) {
		/* Support deprecated gpio_status() */
#ifdef gpio_status
		gpio_status();
		return 0;
#elif defined(CONFIG_DM_GPIO)
		return cmd_process_error(cmdtp, do_gpio_status(all, str_gpio));
#else
		goto show_usage;
#endif
	}

	if (!str_gpio)
		goto show_usage;

	/* parse the behavior */
	switch (*str_cmd) {
		case 'i': sub_cmd = GPIO_INPUT;  break;
		case 's': sub_cmd = GPIO_SET;    break;
		case 'c': sub_cmd = GPIO_CLEAR;  break;
		case 't': sub_cmd = GPIO_TOGGLE; break;
		default:  goto show_usage;
	}

#if defined(CONFIG_DM_GPIO)
	/*
	 * TODO(sjg@chromium.org): For now we must fit into the existing GPIO
	 * framework, so we look up the name here and convert it to a GPIO number.
	 * Once all GPIO drivers are converted to driver model, we can change the
	 * code here to use the GPIO uclass interface instead of the numbered
	 * GPIO compatibility layer.
	 */
	ret = gpio_lookup_name(str_gpio, NULL, NULL, &gpio);
	if (ret)
		return cmd_process_error(cmdtp, ret);
#else
	/* turn the gpio name into a gpio number */
	gpio = name_to_gpio(str_gpio);
	if (gpio < 0)
		goto show_usage;
#endif
	/* grab the pin before we tweak it */
	ret = gpio_request(gpio, "cmd_gpio");
	if (ret && ret != -EBUSY) {
		printf("gpio: requesting pin %u failed\n", gpio);
		return -1;
	}

	/* finally, let's do it: set direction and exec command */
	if (sub_cmd == GPIO_INPUT) {
		gpio_direction_input(gpio);
		value = gpio_get_value(gpio);
	} else {
		switch (sub_cmd) {
			case GPIO_SET:    value = 1; break;
			case GPIO_CLEAR:  value = 0; break;
			case GPIO_TOGGLE: value = !gpio_get_value(gpio); break;
			default:          goto show_usage;
		}
		gpio_direction_output(gpio, value);
	}
	printf("gpio: pin %s (gpio %i) value is %lu\n",
		str_gpio, gpio, value);

	if (ret != -EBUSY)
		gpio_free(gpio);

	return value;
}

U_BOOT_CMD(gpio, 4, 0, do_gpio,
	   "query and control gpio pins",
	   "<input|set|clear|toggle> <pin>\n"
	   "    - input/set/clear/toggle the specified pin\n"
	   "gpio status [-a] [<bank> | <pin>]  - show [all/claimed] GPIOs");