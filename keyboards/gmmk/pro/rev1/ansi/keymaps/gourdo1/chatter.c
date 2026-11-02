// Copyright 2026 Dmitrii Galantsev
// SPDX-License-Identifier: GPL-2.0-or-later

#include <string.h>
#include "quantum.h"
#include "debounce.h"
#include "raw_hid.h"

#ifndef DEBOUNCE_PRESS
#    define DEBOUNCE_PRESS 5
#endif
#ifndef DEBOUNCE_RELEASE
#    define DEBOUNCE_RELEASE 10
#endif
#ifndef CHATTER_SHADOW_DEBOUNCE
#    define CHATTER_SHADOW_DEBOUNCE 5
#endif
#ifndef CHATTER_SUSPECT_GAP
#    define CHATTER_SUSPECT_GAP 40
#endif
#define CHATTER_LOG_SIZE 64
#define CHATTER_KEYS (MATRIX_ROWS * MATRIX_COLS)
#define CHATTER_VERSION 1

#define CHATTER_CMD 0xD0
enum { CHATTER_INFO = 1, CHATTER_EVENTS = 2, CHATTER_COUNTERS = 3, CHATTER_CLEAR = 4 };
enum { CHATTER_PREVENTED = 1, CHATTER_SUSPECT = 2 };

typedef struct {
    uint32_t t0;
    bool     pending;
} filter_t;

typedef struct __attribute__((packed)) {
    uint32_t seq;
    uint32_t ms;
    uint8_t  key;
    uint8_t  type;
    uint16_t a;
    uint16_t b;
} chatter_event_t;

_Static_assert(sizeof(chatter_event_t) == 14, "chatter_event_t");
_Static_assert(CHATTER_KEYS <= 256, "key index must fit in uint8_t");

static filter_t        main_filter[CHATTER_KEYS];
static filter_t        shadow_filter[CHATTER_KEYS];
static bool            shadow_state[CHATTER_KEYS];
static bool            shadow_dropped[CHATTER_KEYS];
static bool            released_once[CHATTER_KEYS];
static uint32_t        pressed_at[CHATTER_KEYS];
static uint32_t        released_at[CHATTER_KEYS];
static uint32_t        shadow_released_at[CHATTER_KEYS];
static uint16_t        presses[CHATTER_KEYS];
static uint16_t        prevented[CHATTER_KEYS];
static uint16_t        suspects[CHATTER_KEYS];
static chatter_event_t events[CHATTER_LOG_SIZE];
static uint32_t        next_seq;

static uint16_t clamp16(uint32_t v) {
    return (uint16_t)(v > UINT16_MAX ? UINT16_MAX : v);
}

static void log_event(uint8_t key, uint8_t type, uint32_t a, uint32_t b) {
    chatter_event_t *e = &events[next_seq % CHATTER_LOG_SIZE];
    e->seq             = next_seq++;
    e->ms              = timer_read32();
    e->key             = key;
    e->type            = type;
    e->a               = clamp16(a);
    e->b               = clamp16(b);
}

static bool filter_step(filter_t *f, bool *state, bool raw, uint32_t now, uint8_t press_ms, uint8_t release_ms) {
    if (raw == *state) {
        f->pending = false;
        return false;
    }
    if (!f->pending) {
        f->pending = true;
        f->t0      = now;
    }
    if (TIMER_DIFF_32(now, f->t0) >= (*state ? release_ms : press_ms)) {
        *state     = raw;
        f->pending = false;
        return true;
    }
    return false;
}

void debounce_init(void) {}

bool debounce(matrix_row_t raw[], matrix_row_t cooked[], bool changed) {
    (void)changed;
    uint32_t now            = timer_read32();
    bool     cooked_changed = false;

    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            uint8_t      k    = (uint8_t)(row * MATRIX_COLS + col);
            matrix_row_t mask = (matrix_row_t)(MATRIX_ROW_SHIFTER << col);
            bool         r    = raw[row] & mask;
            bool         down = cooked[row] & mask;

            if (filter_step(&main_filter[k], &down, r, now, DEBOUNCE_PRESS, DEBOUNCE_RELEASE)) {
                cooked[row] ^= mask;
                cooked_changed = true;
                if (down) {
                    presses[k]++;
                    if (released_once[k] && TIMER_DIFF_32(now, released_at[k]) < CHATTER_SUSPECT_GAP) {
                        suspects[k]++;
                        log_event(k, CHATTER_SUSPECT, TIMER_DIFF_32(now, released_at[k]), TIMER_DIFF_32(released_at[k], pressed_at[k]));
                    }
                    pressed_at[k] = now;
                } else {
                    released_at[k]    = now;
                    released_once[k]  = true;
                    shadow_dropped[k] = false;
                }
            }

            if (filter_step(&shadow_filter[k], &shadow_state[k], r, now, CHATTER_SHADOW_DEBOUNCE, CHATTER_SHADOW_DEBOUNCE)) {
                if (!shadow_state[k]) {
                    shadow_released_at[k] = now;
                    shadow_dropped[k]     = down;
                } else if (shadow_dropped[k] && down) {
                    shadow_dropped[k] = false;
                    prevented[k]++;
                    log_event(k, CHATTER_PREVENTED, TIMER_DIFF_32(now, shadow_released_at[k]), TIMER_DIFF_32(now, pressed_at[k]));
                }
            }
        }
    }
    return cooked_changed;
}

static void put32(uint8_t *p, uint32_t v) {
    memcpy(p, &v, sizeof v);
}

static uint32_t get32(const uint8_t *p) {
    uint32_t v;
    memcpy(&v, p, sizeof v);
    return v;
}

bool via_command_kb(uint8_t *data, uint8_t length) {
    if (data[0] != CHATTER_CMD) {
        return false;
    }
    uint8_t  sub   = data[1];
    uint32_t from  = get32(&data[2]);
    uint8_t  start = data[2];
    memset(data + 2, 0, length - 2);

    switch (sub) {
        case CHATTER_INFO:
            data[2] = CHATTER_VERSION;
            put32(&data[3], next_seq);
            put32(&data[7], timer_read32());
            data[11] = DEBOUNCE_PRESS;
            data[12] = DEBOUNCE_RELEASE;
            data[13] = CHATTER_SHADOW_DEBOUNCE;
            data[14] = CHATTER_SUSPECT_GAP;
            data[15] = CHATTER_LOG_SIZE;
            data[16] = MATRIX_ROWS;
            data[17] = MATRIX_COLS;
            break;
        case CHATTER_EVENTS: {
            uint32_t oldest = next_seq > CHATTER_LOG_SIZE ? next_seq - CHATTER_LOG_SIZE : 0;
            if (from < oldest) {
                from = oldest;
            }
            uint8_t n = 0;
            while (n < 2 && from + n < next_seq) {
                memcpy(&data[3 + n * sizeof(chatter_event_t)], &events[(from + n) % CHATTER_LOG_SIZE], sizeof(chatter_event_t));
                n++;
            }
            data[2] = n;
            break;
        }
        case CHATTER_COUNTERS: {
            uint8_t n = 0;
            while (n < 4 && start + n < CHATTER_KEYS) {
                uint8_t *p = &data[4 + n * 6];
                uint8_t  k = (uint8_t)(start + n);
                memcpy(p, &presses[k], 2);
                memcpy(p + 2, &prevented[k], 2);
                memcpy(p + 4, &suspects[k], 2);
                n++;
            }
            data[2] = start;
            data[3] = n;
            break;
        }
        case CHATTER_CLEAR:
            memset(presses, 0, sizeof presses);
            memset(prevented, 0, sizeof prevented);
            memset(suspects, 0, sizeof suspects);
            break;
        default:
            data[1] = 0xFF;
            break;
    }
    raw_hid_send(data, length);
    return true;
}
