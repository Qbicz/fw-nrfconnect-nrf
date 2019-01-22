/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#include <zephyr/types.h>

#include <soc.h>
#include <device.h>
#include <gpio.h>

#include "event_manager.h"
#include "button_event.h"
#include "power_event.h"

#define MODULE buttons
#include "module_state_event.h"

#include <logging/log.h>
LOG_MODULE_REGISTER(MODULE, CONFIG_DESKTOP_BUTTONS_LOG_LEVEL);

static struct device *gpio_dev;
static struct gpio_callback gpio_cb;
static atomic_t active;

static const u8_t pins[] = { SW0_GPIO_PIN };

static int get_state(u32_t *mask)
{
	for (size_t i = 0; i < ARRAY_SIZE(pins); i++) {
		u32_t val;

		if (gpio_pin_read(gpio_dev, pins[i], &val)) {
			LOG_ERR("cannot get pin");
			return -EFAULT;
		}

		(*mask) |= (val << i);
	}

	return 0;
}

void button_pressed(struct device *gpio_dev, struct gpio_callback *cb,
		    u32_t pin_mask)
{
	static u32_t old_state;
	u32_t new_state = 0;

	get_state(&new_state);

	// TODO: this algorithm! read carefully, with understanding.

	// TODO: if in doubt, test with printouts
	for (size_t i = 0; i < ARRAY_SIZE(pins); i++) {

		LOG_WRN("new %d, old %d", new_state, old_state);
		bool is_pressed = (new_state & (1 << i));
		bool was_pressed = (old_state & (1 << i));

		if (is_pressed != was_pressed) {
			struct button_event *event = new_button_event();

			event->key_id = (i & 0xFF);
			event->pressed = is_pressed;
			EVENT_SUBMIT(event);
		}
	}

	LOG_WRN("button %d", pin_mask);
	LOG_WRN("state %d", old_state);

	old_state = new_state;
}

static void init_fn(void)
{
	int err;

	gpio_dev = device_get_binding(DT_GPIO_P1_DEV_NAME);
	if (!gpio_dev) {
		LOG_ERR("cannot get GPIO device binding");
		goto error;
	}

	int flags = GPIO_PUD_PULL_UP | GPIO_DIR_IN | GPIO_INT |
		    GPIO_INT_EDGE | GPIO_INT_DOUBLE_EDGE; // TODO: ACTIVE_HIGH?

	for (size_t i = 0; i < ARRAY_SIZE(pins); i++) {
		err = gpio_pin_configure(gpio_dev, pins[i], flags);
		LOG_ERR("F: conf %d", pins[i]);

		if (err) {
			LOG_ERR("cannot configure gpio");
			goto error;
		}
	}

	u32_t pin_mask = 0;
	for (size_t i = 0; i < ARRAY_SIZE(pins); i++) {
		pin_mask |= BIT(pins[i]);
	}

	gpio_init_callback(&gpio_cb, button_pressed, pin_mask);

	err = gpio_add_callback(gpio_dev, &gpio_cb);
	if (err) {
		LOG_ERR("cannot add callback");
		goto error;
	}

	for (size_t i = 0; i < ARRAY_SIZE(pins); i++) {
		err = gpio_pin_enable_callback(gpio_dev, pins[i]);
		if (err) {
			LOG_ERR("cannot enable callback");
			goto error;
		}
	}

	module_set_state(MODULE_STATE_READY);
	return;
error:
	module_set_state(MODULE_STATE_ERROR);
}

static bool event_handler(const struct event_header *eh)
{
	if (is_module_state_event(eh)) {
		struct module_state_event *event = cast_module_state_event(eh);

		if (check_state(event, MODULE_ID(main), MODULE_STATE_READY)) {
			static bool initialized;

			LOG_WRN("INIT BUTTON %s pin %d", SW0_GPIO_CONTROLLER, SW0_GPIO_PIN);

			__ASSERT_NO_MSG(!initialized);
			initialized = true;

			init_fn();
			atomic_set(&active, true);
			// TODO: active not needed in callback-based module

			return false;
		}

		return false;
	}

	/* If event is unhandled, unsubscribe. */
	__ASSERT_NO_MSG(false);

	return false;
}
EVENT_LISTENER(MODULE, event_handler);
EVENT_SUBSCRIBE(MODULE, module_state_event);
EVENT_SUBSCRIBE_EARLY(MODULE, power_down_event);
EVENT_SUBSCRIBE(MODULE, wake_up_event);
