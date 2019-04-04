#include <zephyr/types.h>
#include <stddef.h>
#include <errno.h>
#include <misc/printk.h>
#include <zephyr.h>
#include <gpio.h>
#include <soc.h>
#include <assert.h>

#include <dk_buttons_and_leds.h>

#define BUTTON_OPEN	0x01
#define BUTTON_CLOSE	0x02

/* Car door lock state */
enum door_state {
	STATE_OPEN,
	STATE_CLOSED
};

struct automobile {
	enum door_state state;
};

struct automobile car;

void door_open(void) {
	switch (car.state) {
	case STATE_OPEN:
		printk("ALREADY OPEN\n");
		/* Do nothing */
		break;

	case STATE_CLOSED:
		printk("OPEN THE CAR\n");
		car.state = STATE_OPEN;
		break;

	default:
		__ASSERT_NO_MSG(false);
	}
}

void door_close(void) {
	switch (car.state) {
	case STATE_OPEN:
		printk("CLOSE THE CAR\n");
		car.state = STATE_CLOSED;
		break;

	case STATE_CLOSED:
		printk("ALREADY CLOSED\n");
		/* Do nothing */
		break;

	default:
		__ASSERT_NO_MSG(false);
	}
}

void button_changed(u32_t button_state, u32_t has_changed)
{
	if (has_changed == BUTTON_OPEN && button_state & BUTTON_OPEN) {
		printk("user requested: open\n");
		door_open();
	}

	if (has_changed == BUTTON_CLOSE && button_state & BUTTON_CLOSE) {
		printk("user requested: close\n");
		door_close();
	}
}



void configure_buttons(void)
{
	int err = dk_buttons_init(button_changed);
	if (err) {
		printk("Cannot init buttons (err: %d)\n", err);
	}
}


void main(void)
{
	printk("Start zephyr\n");

	car.state = STATE_CLOSED;
	configure_buttons();
}
