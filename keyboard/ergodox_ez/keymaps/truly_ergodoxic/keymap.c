// An attempt to adapt the best parts of the TrulyErgonomic layout with the
// ErgoDox EZ's features.
//
// The main thing I like about the TrulyErgonomic's layout is the symmetry.
// Being able to do any non-letter keystroke with either hand has helped my
// wrist problems a lot, by letting me spread the stress evenly.
//
// The central location of Tab and Backspace has also helped, by moving load
// from the weaker pinky fingers to the stronger index fingers.
//
// Also includes a Plover layer, because I'm interested in learning Plover.

#include "ergodox_ez.h"
#include "debug.h"
#include "action_layer.h"

#define BASE 0 // default layer
#define PLVR 1 // Plover layer, ripped from pre-existing Plover layout
#define SYMB 2 // Function key layer
#define MDIA 3 // media keys

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap 0: Basic layer
 *
 * TODO Decide where [] and {} go.
 *
 * I've considered putting them in the function layer, on the home row,
 * which is not a crazy idea. It does mean modifiers for brackets, though, which
 * seems like unnecessary keypressing.
 *
 * I suppose another option is learning to use them down in the sub-row where
 * they currently live. I don't love that, but it might actually be usable.
 *
 * For now, I guess I'll try all of the above and see which approach I wind up using.
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |   `    |   1  |   2  |   3  |   4  |   5  |  -   |           |  =   |   6  |   7  |   8  |   9  |   0  |   [    |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |   /    |   Q  |   W  |   E  |   R  |   T  | Tab  |           | Tab  |   Y  |   U  |   I  |   O  |   P  |   \    |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |Shft/Cap|   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |   ;  |Shft/Cap|
 * |--------+------+------+------+------+------| Back |           | Back |------+------+------+------+------+--------|
 * |Ctrl/Esc|   Z  |   X  |   C  |   V  |   B  | space|           | space|   N  |   M  |   ,  |   .  |   '  |Ctrl/Esc|
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |  Alt |  '"  |AltShf| Left | Right|                                       |  Up  | Down |   [  |   ]  | Alt  |
 *   `----------------------------------'                                       `----------------------------------'
 *
 *                                        ,-------------.       ,-------------.
 *                                        |  Fn  |Plover|       |Plover|  Fn   |
 *                                 ,------|------|------|       |------+-------+------.
 *                                 |      |      | Home |       | PgUp |       |      |
 *                                 | Space| Enter|------|       |------| Enter |Space |
 *                                 |      |      | End  |       | PgDn |       |      |
 *                                 `--------------------'       `----------------------'
 */
// If it accepts an argument (i.e, is a function), it doesn't need KC_.
// Otherwise, it needs KC_*
[BASE] = KEYMAP(  // layer 0 : default
        // left hand
        KC_GRV,         KC_1,         KC_2,   KC_3,   KC_4,   KC_5,   KC_MINS,
        KC_SLSH,        KC_Q,         KC_W,   KC_E,   KC_R,   KC_T,   KC_TAB,
        SFT_T(KC_CAPS), KC_A,         KC_S,   KC_D,   KC_F,   KC_G,
        CTL_T(KC_ESC),  KC_Z,         KC_X,   KC_C,   KC_V,   KC_B,   KC_BSPC,
        KC_LALT,        KC_QUOT,      LALT(KC_LSFT),  KC_LEFT,KC_LBRC,
                                              MO(2), TG(1),
                                                              KC_HOME,
                                              KC_SPC, KC_ENT, KC_END,
        // right hand
        KC_EQL,      KC_6,   KC_7,   KC_8,   KC_9,   KC_0,             KC_LBRC,
        KC_TAB,      KC_Y,   KC_U,   KC_I,   KC_O,   KC_P,             KC_BSLS,
                     KC_H,   KC_J,   KC_K,   KC_L,   KC_SCLN,          SFT_T(KC_CAPS),
        KC_BSPC,     KC_N,   KC_M,   KC_COMM,KC_DOT, KC_QUOT,          CTL_T(KC_ESC),
                             KC_UP,  KC_DOWN,KC_LBRC,KC_RBRC,          KC_RALT,
             TG(1),       MO(2),
             KC_PGUP,
             KC_PGDN,KC_ENT, KC_SPC
    ),

/* Keymap 1: Steno for Plover from https://github.com/shayneholmes/tmk_keyboard/commit/11290f8489013018f778627db725160c745e75bd
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |   1  |   2  |   3  |   4  |   5  |      |           |      |  6   |  7   |   8  |   9  |  0   |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |   q  |   w  |   e  |   r  |   t  |------|           |------|  y   |  u   |   i  |   o  |  p   |   [    |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |   a  |   s  |   d  |   f  |   g  |      |           |      |  h   |  j   |   k  |   l  |  ;   |   '    |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      |      |      |                                       |      |      |      |      |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 |   c  |   v  |------|       |------|  n   |  m   |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */

[PLVR] = KEYMAP(  // layout: layer 1: Steno for Plover
        // left hand
        KC_NO, KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,
        KC_NO,  KC_1,   KC_2,   KC_3,   KC_4,   KC_5,   KC_NO,
        KC_NO,  KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,
        KC_NO,  KC_A,   KC_S,   KC_D,   KC_F,   KC_G,   KC_NO,
        KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,
                                      KC_FN4, KC_NO,
                                           KC_NO,
                                 KC_C,   KC_V,   KC_NO,
        // right hand
        KC_TRNS,  KC_NO,   KC_NO,   KC_NO,  KC_NO,   KC_NO,   KC_TRNS,
        KC_NO,    KC_6,    KC_7,    KC_8,   KC_9,    KC_0,    KC_TRNS,
                  KC_Y,    KC_U,    KC_I,   KC_O,    KC_P,    KC_LBRC,
        KC_NO,    KC_H,    KC_J,    KC_K,   KC_L,    KC_SCLN, KC_QUOT,
                           KC_TRNS, KC_TRNS,KC_NO,   KC_NO,   KC_NO,
        KC_TRNS, KC_TRNS,
        KC_TRNS,
        KC_NO,  KC_N,   KC_M
),


/* Keymap 2: Function Layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |  F1  |  F2  |  F3  |  F4  |  F5  |Insert|           |PrtScr|  F6  |  F7  |  F8  |  F9  |  F10 |   F11  |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |   F12  |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |  [   |  {   |  (   |      |      |------|           |------|      |      |  )   |  }   |  ]   |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      |      |      |                                       |      |    . |      |      |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 |      |      |------|       |------|      |      |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
// Function layer
[SYMB] = KEYMAP(
       // left hand
       KC_TRNS,KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_INS,
       KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,
       KC_TRNS,KC_LBRC,KC_LCBR,KC_LPRN,KC_TRNS,KC_TRNS,
       KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,
       KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,
                                       KC_TRNS,KC_TRNS,
                                               KC_TRNS,
                               KC_TRNS,KC_TRNS,KC_TRNS,
       // right hand
       KC_PSCR, KC_F6,   KC_F7,  KC_F8,   KC_F9,   KC_F10,  KC_F11,
       KC_TRNS, KC_TRNS, KC_TRNS,KC_TRNS, KC_TRNS, KC_TRNS, KC_F12,
                KC_TRNS, KC_TRNS,KC_RPRN, KC_RCBR, KC_RBRC, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS,KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                         KC_TRNS,KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS,
       KC_TRNS, KC_TRNS,
       KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS
),

/* Keymap 3: Function key layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |      |      | MsUp |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |MsLeft|MsDown|MsRght|      |------|           |------|      |      |      |      |      |  Play  |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      |      |      | Prev | Next |      |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      | Lclk | Rclk |                                       |VolUp |VolDn | Mute |      |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |Brwser|
 *                                 |      |      |------|       |------|      |Back  |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
// MEDIA AND MOUSE
KEYMAP(
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_MS_U, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_MS_L, KC_MS_D, KC_MS_R, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_BTN1, KC_BTN2,
                                           KC_TRNS, KC_TRNS,
                                                    KC_TRNS,
                                  KC_TRNS, KC_TRNS, KC_TRNS,
    // right hand
       KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                 KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MPLY,
       KC_TRNS,  KC_TRNS, KC_TRNS, KC_MPRV, KC_MNXT, KC_TRNS, KC_TRNS,
                          KC_VOLU, KC_VOLD, KC_MUTE, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS,
       KC_TRNS,
       KC_TRNS, KC_TRNS, KC_WBAK
),
};

// TODO What does this actually do?
const uint16_t PROGMEM fn_actions[] = {
    [1] = ACTION_LAYER_TAP_TOGGLE(SYMB)                // FN1 - Momentary Layer 1 (Symbols)
};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
  // MACRODOWN only works in this function
      switch(id) {
        case 0:
        if (record->event.pressed) {
          register_code(KC_RSFT);
        } else {
          unregister_code(KC_RSFT);
        }
        break;
      }
    return MACRO_NONE;
};

// Runs just one time when the keyboard initializes.
void * matrix_init_user(void) {

};

// Runs constantly in the background, in a loop.
void * matrix_scan_user(void) {

    uint8_t layer = biton32(layer_state);

    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();
    switch (layer) {
      // TODO: Make this relevant to the ErgoDox EZ.
        case 1:
            ergodox_right_led_1_on();
            break;
        case 2:
            ergodox_right_led_2_on();
            break;
        default:
            // none
            break;
    }

};
