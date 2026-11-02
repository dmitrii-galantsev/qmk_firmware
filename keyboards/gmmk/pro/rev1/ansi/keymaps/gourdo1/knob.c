// Copyright 2026 Dmitrii Galantsev
// SPDX-License-Identifier: GPL-2.0-or-later

#include <string.h>
#include "quantum.h"
#include "raw_hid.h"
#include "knob.h"

static uint8_t  level;
static bool     host_present;
static uint32_t host_seen;
static bool     showing;
static uint32_t show_at;
static bool     turned_this_hold;
static uint8_t  tick_seq;

static void show(void) {
    showing = true;
    show_at = timer_read32();
}

bool knob_host_present(void) {
    if (host_present && TIMER_DIFF_32(timer_read32(), host_seen) >= KNOB_HOST_TIMEOUT) {
        host_present = false;
    }
    return host_present;
}

bool knob_tick(bool clockwise) {
    if (!knob_host_present()) {
        return false;
    }
    uint8_t report[KNOB_REPORT_SIZE] = {KNOB_CMD_TICK, clockwise ? 0x01 : 0xFF, ++tick_seq};
    raw_hid_send(report, sizeof report);
    turned_this_hold = true;
    show();
    return true;
}

bool knob_via_command(uint8_t *data, uint8_t length) {
    if (data[0] != KNOB_CMD_LEVEL) {
        return false;
    }
    uint8_t flags = data[1];
    if (flags & KNOB_FLAG_BYE) {
        host_present = false;
        showing      = false;
    } else {
        level        = data[2] > 100 ? 100 : data[2];
        host_present = true;
        host_seen    = timer_read32();
        if (flags & KNOB_FLAG_SHOW) {
            show();
        }
    }
    memset(data + 1, 0, length - 1);
    data[1] = KNOB_VERSION;
    data[2] = level;
    data[3] = host_present;
    raw_hid_send(data, length);
    return true;
}

int16_t knob_display_level(bool fn_held) {
    if (!fn_held) {
        turned_this_hold = false;
    }
    if (!knob_host_present()) {
        showing          = false;
        turned_this_hold = false;
        return -1;
    }
    if (showing && TIMER_DIFF_32(timer_read32(), show_at) >= KNOB_SHOW_MS) {
        showing = false;
    }
    return showing || turned_this_hold ? level : -1;
}

uint8_t knob_bar_cell(uint8_t value, uint8_t index, uint8_t count) {
    int16_t fill = (int16_t)value * count - (int16_t)index * 100;
    if (fill <= 0) {
        return 0;
    }
    if (fill >= 100) {
        return 255;
    }
    return (uint8_t)(fill * 255 / 100);
}
