/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#include <misc/util.h>

#include "hid_keymap.h"

/*
 * HID keymap. The Consumer Control keys are defined in section 15 of
 * the HID Usage Tables document under the following URL:
 * http://www.usb.org/developers/hidpage/Hut1_12v2.pdf
 */
const struct hid_keymap hid_keymap[] = {
	{ 0, 0x01, TARGET_REPORT_MOUSE }, /* Left Mouse Button */
	{ 1, 0x02, TARGET_REPORT_MOUSE }, /* Right Mouse Button */
	{ 2, 0x03, TARGET_REPORT_MOUSE }, /* Middle Mouse Button */
//remove peer control button	{ 3, 0x04, TARGET_REPORT_MOUSE }, /* Additional Mouse Button */
	{ 4, 0x05, TARGET_REPORT_MOUSE }, /* Additional Mouse Button */
	{ 5, 0x06, TARGET_REPORT_MOUSE }, /* Additional Mouse Button */
	{ 6, 0x07, TARGET_REPORT_MOUSE }, /* Additional Mouse Button */
	{ 7, 0x08, TARGET_REPORT_MOUSE }, /* Additional Mouse Button */
};

const size_t hid_keymap_size = ARRAY_SIZE(hid_keymap);
