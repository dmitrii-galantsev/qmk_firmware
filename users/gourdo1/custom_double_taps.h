// Copyright 2022 Google LLC
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gourdo1.h"

static bool process_esc_to_base(uint16_t keycode, keyrecord_t * record) {
    static bool tapped = false;
    static uint16_t tap_timer = 0;

    if (keycode == KC_ESC) {
        if (user_config.esc_double_tap_to_baselyr) {
            if (record -> event.pressed) {
                if (tapped && !timer_expired(record -> event.time, tap_timer)) {
                    // The key was double tapped.
                    layer_clear();
                }
                tapped = true;
                tap_timer = record -> event.time + TAPPING_TERM;
            }
        }
    } else {
        // On an event with any other key, reset the double tap state.
        tapped = false;
    }
    return true;
}

static bool process_lsft_for_caps(uint16_t keycode, keyrecord_t * record) {
    static bool tapped = false;
    static uint16_t tap_timer = 0;

    if (keycode == KC_LSFT) {
        if (user_config.double_tap_shift_for_capslock) {
          if (!keymap_config.no_gui) {
            if (record->event.pressed) {
                if (tapped && !timer_expired(record->event.time, tap_timer)) {
                  // The key was double tapped.
                  //clear_mods();  // If needed, clear the mods.
                  // Do something interesting...
                  register_code(KC_CAPS);
                }
                tapped = true;
                tap_timer = record->event.time + TAPPING_TERM;
            } else {
                unregister_code(KC_CAPS);
            }
          }
        }
    } else {
            // On an event with any other key, reset the double tap state.
            tapped = false;
    }
    return true;
}


static bool process_popi_meh(uint16_t keycode, keyrecord_t * record) {
    if (keycode == POPI_MEH) {
        if (record -> event.pressed) {
            //send_string("MEH!");
            set_mods(MOD_MEH);
            register_code16(keycode);
        } else {
            unregister_code16(keycode);
            del_mods(MOD_MEH);
        }
    }
    return true;
}

