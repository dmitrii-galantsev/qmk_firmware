/* Copyright 2021 Jonavin Eng @Jonavin
   Copyright 2022 gourdo1 <gourdo1@outlook.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#define DEBOUNCE_PRESS 5
#define DEBOUNCE_RELEASE 10

// Force n-key rollover
#define FORCE_NKRO

// Set TT to two taps
#define TAPPING_TOGGLE 2

#ifdef COMMAND_ENABLE
#define IS_COMMAND() (get_mods() == MOD_MASK_CTRL) //debug commands accessed by holding down both CTRLs: https://github.com/qmk/qmk_firmware/blob/master/docs/feature_command.md
#endif

// Caps Word configuration
#define BOTH_SHIFTS_TURNS_ON_CAPS_WORD
#define CAPS_WORD_IDLE_TIMEOUT 10000      // Automatically turn off after x milliseconds of idle. 0 to never timeout.

// Handle GRAVESC combo keys
#define GRAVE_ESC_ALT_OVERRIDE
// Always send Escape if Alt is pressed
#define GRAVE_ESC_CTRL_OVERRIDE
// Always send Escape if Control is pressed

// #define TAPPING_TERM 180
#define TAPPING_TERM 300
#define TAPPING_TERM_PER_KEY

#ifdef RGB_MATRIX_ENABLE
    #undef RGB_MATRIX_KEYPRESSES
    #undef RGB_MATRIX_KEYRELEASES
    #undef RGB_MATRIX_FRAMEBUFFER_EFFECTS

    #define RGB_MATRIX_HUE_STEP 32
    #define RGB_MATRIX_SAT_STEP 17
    #define RGB_MATRIX_VAL_STEP 17

    #define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_CYCLE_OUT_IN_DUAL
    #define RGB_MATRIX_DEFAULT_HUE 24
    #define RGB_MATRIX_DEFAULT_SAT 255
    #define RGB_MATRIX_DEFAULT_VAL 127
    #define RGB_MATRIX_DEFAULT_SPD 127

    #undef ENABLE_RGB_MATRIX_ALPHAS_MODS
    #undef ENABLE_RGB_MATRIX_GRADIENT_UP_DOWN
    #undef ENABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT
    #undef ENABLE_RGB_MATRIX_BREATHING
    #undef ENABLE_RGB_MATRIX_BAND_SAT
    #undef ENABLE_RGB_MATRIX_BAND_VAL
    #undef ENABLE_RGB_MATRIX_BAND_PINWHEEL_SAT
    #undef ENABLE_RGB_MATRIX_BAND_PINWHEEL_VAL
    #undef ENABLE_RGB_MATRIX_BAND_SPIRAL_SAT
    #undef ENABLE_RGB_MATRIX_BAND_SPIRAL_VAL
    #undef ENABLE_RGB_MATRIX_CYCLE_ALL
    #undef ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
    #undef ENABLE_RGB_MATRIX_CYCLE_UP_DOWN
    #undef ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
    #undef ENABLE_RGB_MATRIX_CYCLE_OUT_IN
    #undef ENABLE_RGB_MATRIX_CYCLE_PINWHEEL
    #undef ENABLE_RGB_MATRIX_CYCLE_SPIRAL
    #undef ENABLE_RGB_MATRIX_DUAL_BEACON
    #undef ENABLE_RGB_MATRIX_RAINBOW_BEACON
    #undef ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS
    #undef ENABLE_RGB_MATRIX_RAINDROPS
    #undef ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
    #undef ENABLE_RGB_MATRIX_HUE_BREATHING
    #undef ENABLE_RGB_MATRIX_HUE_PENDULUM
    #undef ENABLE_RGB_MATRIX_HUE_WAVE
    #undef ENABLE_RGB_MATRIX_PIXEL_RAIN
    #undef ENABLE_RGB_MATRIX_PIXEL_FLOW
    #undef ENABLE_RGB_MATRIX_PIXEL_FRACTAL
    #undef ENABLE_RGB_MATRIX_TYPING_HEATMAP
    #undef ENABLE_RGB_MATRIX_DIGITAL_RAIN
    #undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
    #undef ENABLE_RGB_MATRIX_SOLID_REACTIVE
    #undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE
    #undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE
    #undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS
    #undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS
    #undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS
    #undef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
    #undef ENABLE_RGB_MATRIX_SPLASH
    #undef ENABLE_RGB_MATRIX_MULTISPLASH
    #undef ENABLE_RGB_MATRIX_SOLID_SPLASH
    #undef ENABLE_RGB_MATRIX_SOLID_MULTISPLASH
#endif // RGB_MATRIX_ENABLE

// Add a layer for colemak  -- set "COLEMAK_LAYER_ENABLE = yes" in rules.mk to enable
#if defined COLEMAK_LAYER_ENABLE
    #define DYNAMIC_KEYMAP_LAYER_COUNT 5
    #define _COLEMAK 4
#endif // COLEMAK_LAYER_ENABLE

/*
// Mouse Keys Accelerated Mode Definitions
#define MOUSEKEY_DELAY 3               // Delay between pressing a movement key and cursor movement (default: 10)
#define MOUSEKEY_INTERVAL 13           // Time between cursor movements in milliseconds (default: 20); Try setting to 1000/monitor refresh for smooth movement.
#define MOUSEKEY_MOVE_DELTA 8          // Step size (default: 8)
#define MOUSEKEY_MAX_SPEED 9           // Maximum cursor speed at which acceleration stops (default: 10)
#define MOUSEKEY_TIME_TO_MAX 150       // Time until maximum cursor speed is reached (default: 30)
#define MOUSEKEY_WHEEL_DELAY 0         // Delay between pressing a wheel key and wheel movement (default: 10)
#define MOUSEKEY_WHEEL_INTERVAL 80     // Time between wheel movements (default: 80)
#define MOUSEKEY_WHEEL_MAX_SPEED 8     // Maximum number of scroll steps per scroll action (default: 8)
#define MOUSEKEY_WHEEL_TIME_TO_MAX 40  // Time until maximum scroll speed is reached (default: 40)
*/

// Mouse Keys Kinetic Mode Definitions
#define MK_KINETIC_SPEED                        // Enable Kinetic mode:  Uses a quadratic curve on cursor speed to allow precise movements at the beginning and increases speed thereafter.
#define MOUSEKEY_DELAY 3                        // Delay between pressing a movement key and cursor movement (default: 10)
#define MOUSEKEY_INTERVAL 13                    // Time between cursor movements in milliseconds (default: 20); Try setting to 1000/monitor refresh for smooth movement.
#define MOUSEKEY_MOVE_DELTA 5                   // Step size for accelerating from initial to base speed (default: 8)
#define MOUSEKEY_MOVE_MAX 50                    // use instead of BASE SPEED to limit speed in Kinetic mode
#define MOUSEKEY_INITIAL_SPEED 100              // Initial speed of the cursor in pixels per second (default: 100)
//#define MOUSEKEY_BASE_SPEED 800               // (broken in QMK 0.16.0) Maximum cursor speed at which acceleration stops (default: 1000)
#define MOUSEKEY_DECELERATED_SPEED 400          // Decelerated cursor speed (default: 400)
#define MOUSEKEY_ACCELERATED_SPEED 2000         // Accelerated cursor speed (default: 3000)
#define MOUSEKEY_WHEEL_INITIAL_MOVEMENTS 16     // Initial number of movements of the mouse wheel (default: 16)
#define MOUSEKEY_WHEEL_BASE_MOVEMENTS 32        // Maximum number of movements at which acceleration stops (default: 32)
#define MOUSEKEY_WHEEL_ACCELERATED_MOVEMENTS 48 // Accelerated wheel movements (default: 48)
#define MOUSEKEY_WHEEL_DECELERATED_MOVEMENTS 8  // Decelerated wheel movements (default: 8)
