// Copyright 2026 Dmitrii Galantsev
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <stdbool.h>
#include <stdint.h>

#define KNOB_CMD_TICK 0xD1
#define KNOB_CMD_LEVEL 0xD2
#define KNOB_FLAG_SHOW 0x01
#define KNOB_FLAG_BYE 0x02
#define KNOB_VERSION 1
#define KNOB_REPORT_SIZE 32

#ifndef KNOB_HOST_TIMEOUT
#    define KNOB_HOST_TIMEOUT 5000
#endif
#ifndef KNOB_SHOW_MS
#    define KNOB_SHOW_MS 1000
#endif

bool    knob_host_present(void);
bool    knob_tick(bool clockwise);
bool    knob_via_command(uint8_t *data, uint8_t length);
int16_t knob_display_level(bool fn_held);
uint8_t knob_bar_cell(uint8_t value, uint8_t index, uint8_t count);
