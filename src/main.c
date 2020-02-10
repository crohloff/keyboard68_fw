/*
 * Copyright (c) 2017 Linaro Limited
 * Copyright (c) 2018 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <errno.h>
#include <string.h>

#define LOG_LEVEL 4
#include <logging/log.h>
LOG_MODULE_REGISTER(main);

#include <zephyr.h>
#include <drivers/led_strip.h>
#include <device.h>
#include <drivers/spi.h>
#include <sys/util.h>

/*
 * Number of RGB LEDs in the LED strip, adjust as needed.
 */
#define STRIP_NUM_LEDS 70
#define STRIP_DEV_NAME DT_INST_0_WORLDSEMI_WS2812_LABEL

#define DELAY_TIME K_MSEC(50)
#define BREATHING_TIME K_MSEC(25)
//0x32 , full brightness would be 255 => TOO MUCH
#define MAX_BRIGHTNESS 25

static const struct led_rgb colors[] = {
	{
		.r = 0x08,
		.g = 0x00,
		.b = 0x00,
	}, /* red */
	{
		.r = 0x00,
		.g = 0x08,
		.b = 0x00,
	}, /* green */
	{
		.r = 0x00,
		.g = 0x00,
		.b = 0x08,
	}, /* blue */
};

static const struct led_rgb black = {
	.r = 0x00,
	.g = 0x00,
	.b = 0x00,
};

static struct led_rgb breathing = {
	.r = 0x00,
	.g = 0x00,
	.b = 0x00,
};

//reihen - spalten
static size_t led_matrix[5][16] = {
	{ 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2 },
	{ 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 0, 1 },
	{ 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 71, 32, 71, 71 },
	{ 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 71, 71, 56, 57, 71 },
	{ 69, 68, 67, 66, 71, 65, 71, 64, 71, 63, 62, 71, 61, 60, 59, 58 }
};

struct led_rgb strip_colors[STRIP_NUM_LEDS];

const struct led_rgb *color_at(size_t time, size_t i)
{
	size_t rgb_start = time % STRIP_NUM_LEDS;

	if (rgb_start <= i && i < rgb_start + ARRAY_SIZE(colors)) {
		return &colors[i - rgb_start];
	} else {
		return &black;
	}
}

static void clear_all(void)
{
	struct device *strip;
	size_t i;

	breathing.r = 0;
	breathing.g = 0;
	breathing.b = 0;

	strip = device_get_binding(STRIP_DEV_NAME);
	if (strip) {
		LOG_INF("Found LED strip device %s", STRIP_DEV_NAME);
	} else {
		LOG_ERR("LED strip device %s not found", STRIP_DEV_NAME);
		return;
	}

	for (i = 0; i < STRIP_NUM_LEDS; i++) {
		memcpy(&strip_colors[i], &black, sizeof(strip_colors[i]));
	}
	led_strip_update_rgb(strip, strip_colors, STRIP_NUM_LEDS);
}

static void led_roll(void)
{
	struct device *strip;
	size_t i, j, k;

	strip = device_get_binding(STRIP_DEV_NAME);
	if (strip) {
		LOG_INF("Found LED strip device %s", STRIP_DEV_NAME);
	} else {
		LOG_ERR("LED strip device %s not found", STRIP_DEV_NAME);
		return;
	}

	clear_all();

	for (k = 0; k < 16; k++) {
		for (i = 0; i < STRIP_NUM_LEDS; i++) {
			memcpy(&strip_colors[i], &black,
			       sizeof(strip_colors[i]));
		}
		for (j = 0; j < 5; j++) {
			memcpy(&strip_colors[led_matrix[j][k]], &colors[1],
			       sizeof(strip_colors[led_matrix[j][k]]));
		}
		led_strip_update_rgb(strip, strip_colors, STRIP_NUM_LEDS);
		k_sleep(K_MSEC(50));
	}
}

static void led_breathing(void)
{
	struct device *strip;
	size_t i, j;

	strip = device_get_binding(STRIP_DEV_NAME);
	if (strip) {
		LOG_INF("Found LED strip device %s", STRIP_DEV_NAME);
	} else {
		LOG_ERR("LED strip device %s not found", STRIP_DEV_NAME);
		return;
	}

	for (j = 0; j < MAX_BRIGHTNESS; j++) {
		breathing.r = j;
		for (i = 0; i < STRIP_NUM_LEDS; i++) {
			memcpy(&strip_colors[i], &breathing,
			       sizeof(strip_colors[i]));
		}
		led_strip_update_rgb(strip, strip_colors, STRIP_NUM_LEDS);
		k_sleep(BREATHING_TIME);
	}

	for (j = MAX_BRIGHTNESS; j > 0; j--) {
		breathing.r = j;
		for (i = 0; i < STRIP_NUM_LEDS; i++) {
			memcpy(&strip_colors[i], &breathing,
			       sizeof(strip_colors[i]));
		}
		led_strip_update_rgb(strip, strip_colors, STRIP_NUM_LEDS);
		k_sleep(BREATHING_TIME);
	}
	clear_all();

	k_sleep(K_MSEC(1000));

	for (j = 0; j < MAX_BRIGHTNESS; j++) {
		breathing.g = j;
		for (i = 0; i < STRIP_NUM_LEDS; i++) {
			memcpy(&strip_colors[i], &breathing,
			       sizeof(strip_colors[i]));
		}
		led_strip_update_rgb(strip, strip_colors, STRIP_NUM_LEDS);
		k_sleep(BREATHING_TIME);
	}

	for (j = MAX_BRIGHTNESS; j > 0; j--) {
		breathing.g = j;
		for (i = 0; i < STRIP_NUM_LEDS; i++) {
			memcpy(&strip_colors[i], &breathing,
			       sizeof(strip_colors[i]));
		}
		led_strip_update_rgb(strip, strip_colors, STRIP_NUM_LEDS);
		k_sleep(BREATHING_TIME);
	}
	clear_all();

	k_sleep(K_MSEC(1000));

	for (j = 0; j < MAX_BRIGHTNESS; j++) {
		breathing.b = j;
		for (i = 0; i < STRIP_NUM_LEDS; i++) {
			memcpy(&strip_colors[i], &breathing,
			       sizeof(strip_colors[i]));
		}
		led_strip_update_rgb(strip, strip_colors, STRIP_NUM_LEDS);
		k_sleep(BREATHING_TIME);
	}

	for (j = MAX_BRIGHTNESS; j > 0; j--) {
		breathing.b = j;
		for (i = 0; i < STRIP_NUM_LEDS; i++) {
			memcpy(&strip_colors[i], &breathing,
			       sizeof(strip_colors[i]));
		}
		led_strip_update_rgb(strip, strip_colors, STRIP_NUM_LEDS);
		k_sleep(BREATHING_TIME);
	}
	clear_all();

	k_sleep(K_MSEC(1000));
}

static void led_walking(void)
{
	struct device *strip;
	size_t i, time, j;

	strip = device_get_binding(STRIP_DEV_NAME);
	if (strip) {
		LOG_INF("Found LED strip device %s", STRIP_DEV_NAME);
	} else {
		LOG_ERR("LED strip device %s not found", STRIP_DEV_NAME);
		return;
	}

	/*
	 * Display a pattern that "walks" the three primary colors
	 * down the strip until it reaches the end, then starts at the
	 * beginning. This has the effect of moving it around in a
	 * circle in the case of rings of pixels.
	 */
	LOG_INF("Displaying pattern on strip");
	time = 0;
	for (j = 0; j < STRIP_NUM_LEDS * 3; j++) {
		for (i = 0; i < STRIP_NUM_LEDS; i++) {
			memcpy(&strip_colors[i], color_at(time, i),
			       sizeof(strip_colors[i]));
		}

		led_strip_update_rgb(strip, strip_colors, STRIP_NUM_LEDS);
		k_sleep(DELAY_TIME);

		time++;
	}
	clear_all();
	k_sleep(K_MSEC(1000));
}

static void led_rgb(void)
{
	struct device *strip;
	size_t i;

	strip = device_get_binding(STRIP_DEV_NAME);
	if (strip) {
		LOG_INF("Found LED strip device %s", STRIP_DEV_NAME);
	} else {
		LOG_ERR("LED strip device %s not found", STRIP_DEV_NAME);
		return;
	}

	for (i = 0; i < STRIP_NUM_LEDS; i++) {
		memcpy(&strip_colors[i], &colors[0], sizeof(strip_colors[i]));
	}
	led_strip_update_rgb(strip, strip_colors, STRIP_NUM_LEDS);
	k_sleep(K_MSEC(1000));

	for (i = 0; i < STRIP_NUM_LEDS; i++) {
		memcpy(&strip_colors[i], &colors[1], sizeof(strip_colors[i]));
	}
	led_strip_update_rgb(strip, strip_colors, STRIP_NUM_LEDS);
	k_sleep(K_MSEC(1000));

	for (i = 0; i < STRIP_NUM_LEDS; i++) {
		memcpy(&strip_colors[i], &colors[2], sizeof(strip_colors[i]));
	}
	led_strip_update_rgb(strip, strip_colors, STRIP_NUM_LEDS);
	k_sleep(K_MSEC(1000));
}

void main(void)
{
	size_t i;
	LOG_INF("Starting LED demo patterns");

	while (1) {
		led_rgb();

		led_breathing();

		led_walking();
		for (i = 0; i < 5; i++) {
			led_roll();
		}
	}
}
