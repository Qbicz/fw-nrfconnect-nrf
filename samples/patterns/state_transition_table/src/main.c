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

#define BUTTON_COUNT	2
#define EVENT_COUNT	BUTTON_COUNT
#define EVENT_ID(x)	(x - 1)

/* Car door lock state */
enum door_state {
	STATE_OPEN,
	STATE_CLOSED,

	STATE_COUNT
};

struct automobile {
	enum door_state state;
};

static struct automobile car;

static enum door_state transition_table[STATE_COUNT][EVENT_COUNT] = {
	{STATE_OPEN, STATE_CLOSED},
	{STATE_OPEN, STATE_CLOSED}
};

void door_open(void) {
	car.state = transition_table[car.state][EVENT_ID(BUTTON_OPEN)];

	printk("Transition to %s", ((car.state == STATE_OPEN) ? "OPEN":"CLOSED"));
}

void door_close(void) {
	car.state = transition_table[car.state][EVENT_ID(BUTTON_CLOSE)];

	printk("Transition to %s", ((car.state == STATE_OPEN) ? "OPEN":"CLOSED"));
}

void button_changed(u32_t button_state, u32_t has_changed)
{
	if ((has_changed == BUTTON_OPEN) && (button_state & BUTTON_OPEN)) {
		printk("user requested: open\n");
		door_open();
	}

	else if ((has_changed == BUTTON_CLOSE) && (button_state & BUTTON_CLOSE)) {
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
	printk("State: transition table\n");

	car.state = STATE_CLOSED;
	configure_buttons();
}
