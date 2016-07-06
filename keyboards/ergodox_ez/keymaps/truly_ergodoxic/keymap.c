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
#include "led.h"

#define BASE 0 // default layer
#define PLVR 1 // Plover layer, ripped from pre-existing Plover layout
#define SYMB 2 // Function key layer
#define MDIA 3 // media keys

#define LEADER_TIMEOUT 300

// I want to use some modifier keys as modifier/tap dual-function keys.
// The MT() macro's native behavior is bad for a fast typist, though -
// if the whole sequence takes less than TAPPING_TERM, you'll get the keycodes
// for the two keys you pressed, rather than modifier+key.
//
// I have therefore adopted a workaround proposed on GitHub:
//
// https://github.com/jackhumbert/qmk_firmware/issues/303#issuecomment-217328415
//
// which means we only get the tap keycode if the tap was less than
// TAPPING_TERM *and* no other keys were pressed during that time.
#define TE_CTL_ESC 8

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap 0: Basic layer
 *
 * Symbols surrounded by () can be triggered by pressing the key once after the
 * leader key. Their Shift equivalent can be triggered by pressing the key
 * twice after the leader key.
 *
 * TODO Do something useful on tapping Shift.
 * I currently use an IDE at $DAYJOB that uses Shift-Shift as an unremappable
 * high-importance shortcut. Thus, I can't do anything else with the Shift
 * keys, but I would like to.
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |   `    |   1  |   2  |   3  |   4  |   5  |Leader|           |Leader|   6  |   7  |   8  |   9  |   0  |  - (=) |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * | / (\)  |   Q  |   W  |   E  |   R  |   T  | Tab  |           | Tab  |   Y  |   U  |   I  |   O  |   P  | [ (])  |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * | Shift  |   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |   ;  | Shift  |
 * |--------+------+------+------+------+------| Back |           | Back |------+------+------+------+------+--------|
 * |Ctrl/Esc|   Z  |   X  |   C  |   V  |   B  | space|           | space|   N  |   M  |   ,  |   .  |   '  |Ctrl/Esc|
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |  Alt |  '"  |AltShf| Left | LGui |                                       | RGui | Down |   [  |   ]  | Alt  |
 *   `----------------------------------'                                       `----------------------------------'
 *
 *                                        ,-------------.       ,-------------.
 *                                        |  Fn  |Plover|       |Plover|  Fn   |
 *                                 ,------|------|------|       |------+-------+------.
 *                                 |      |      | Left |       |  Up  |       |      |
 *                                 | Space| Enter|------|       |------| Enter |Space |
 *                                 |      |      |Right |       | Down |       |      |
 *                                 `--------------------'       `----------------------'
 */
// If it accepts an argument (i.e, is a function), it doesn't need KC_.
// Otherwise, it needs KC_*
[BASE] = KEYMAP(  // layer 0 : default
        // left hand
        KC_GRV,         KC_1,         KC_2,   KC_3,   KC_4,   KC_5,   KC_LEAD,
        KC_SLSH,        KC_Q,         KC_W,   KC_E,   KC_R,   KC_T,   KC_TAB,
        KC_LSFT,        KC_A,         KC_S,   KC_D,   KC_F,   KC_G,
        F(TE_CTL_ESC),  KC_Z,         KC_X,   KC_C,   KC_V,   KC_B,   KC_BSPC,
        KC_LALT,        KC_QUOT,      LALT(KC_LSFT),  KC_LEFT,KC_LGUI,
                                              MO(2), TG(1),
                                                              KC_LEFT,
                                              KC_SPC, KC_ENT, KC_RIGHT,
        // right hand
        KC_LEAD,     KC_6,   KC_7,   KC_8,   KC_9,   KC_0,             KC_MINUS,
        KC_TAB,      KC_Y,   KC_U,   KC_I,   KC_O,   KC_P,             KC_LBRC,
                     KC_H,   KC_J,   KC_K,   KC_L,   KC_SCLN,          KC_RSFT,
        KC_BSPC,     KC_N,   KC_M,   KC_COMM,KC_DOT, KC_QUOT,          F(TE_CTL_ESC),
                             KC_RGUI,  KC_DOWN,KC_LBRC,KC_RBRC,          KC_RALT,
             TG(1),       MO(2),
             KC_UP,
             KC_DOWN,KC_ENT, KC_SPC
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
 *   |CADel |      |      |      |      |                                       |      |    . |      |      |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      | Home |       | PgUp |      |      |
 *                                 |      |      |------|       |------|      |      |
 *                                 |      |      | End  |       | PgDn |      |      |
 *                                 `--------------------'       `--------------------'
 */
// Function layer
[SYMB] = KEYMAP(
       // left hand
       KC_TRNS,           KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_INS,
       KC_TRNS,           KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,
       KC_TRNS,           KC_LBRC,KC_LCBR,KC_LPRN,KC_TRNS,KC_TRNS,
       KC_TRNS,           KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,
       LALT(LCTL(KC_DEL)),KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,
                                       KC_TRNS,KC_TRNS,
                                               KC_HOME,
                               KC_TRNS,KC_TRNS,KC_END,
       // right hand
       KC_PSCR,  KC_F6,   KC_F7,  KC_F8,   KC_F9,   KC_F10,  KC_F11,
       KC_TRNS,  KC_TRNS, KC_TRNS,KC_TRNS, KC_TRNS, KC_TRNS, KC_F12,
                 KC_TRNS, KC_TRNS,KC_RPRN, KC_RCBR, KC_RBRC, KC_TRNS,
       KC_TRNS,  KC_TRNS, KC_TRNS,KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                          KC_TRNS,KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS,
       KC_TRNS,  KC_TRNS,
       KC_PGUP,
       KC_PGDOWN,KC_TRNS, KC_TRNS
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

const uint16_t PROGMEM fn_actions[] = {
  [1] = ACTION_LAYER_TAP_TOGGLE(SYMB),                // FN1 - Momentary Layer 1 (Symbols)
  [TE_CTL_ESC] = ACTION_MACRO_TAP(TE_CTL_ESC)
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

        case TE_CTL_ESC:
          // TODO Look for a simpler way to express this idea.
          // Maybe there should be an alternative to the MT() macro that has
          // these semantics baked-in? MTI(), for Modifier-Tap-Interrupted?
          if (record->event.pressed) {
            if (record->tap.count && !record->tap.interrupted) {
              register_code(KC_ESC);
            } else {
              register_code(KC_LCTL);
            }
          } else {
            if (record->tap.count && !record->tap.interrupted) {
              unregister_code(KC_ESC);
            } else {
              unregister_code(KC_LCTL);
            }
          }
          break;
      }

      return MACRO_NONE;
};

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {

};

LEADER_EXTERNS();

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {
    LEADER_DICTIONARY() {
        leading = false;
        leader_end();

        SEQ_ONE_KEY(KC_LBRC) {
            register_code(KC_RBRC);
            unregister_code(KC_RBRC);
        }
        SEQ_TWO_KEYS(KC_LBRC, KC_LBRC) {
            register_code(KC_LSFT);
            register_code(KC_RBRC);
            unregister_code(KC_RBRC);
            unregister_code(KC_LSFT);
        }

        SEQ_ONE_KEY(KC_SLSH) {
            register_code(KC_BSLS);
            unregister_code(KC_BSLS);
        }
        SEQ_TWO_KEYS(KC_SLSH, KC_SLSH) {
          register_code(KC_LSFT);
          register_code(KC_BSLS);
          unregister_code(KC_BSLS);
          unregister_code(KC_LSFT);
        }

        SEQ_ONE_KEY(KC_MINUS) {
            register_code(KC_EQL);
            unregister_code(KC_EQL);
        }
        SEQ_TWO_KEYS(KC_MINUS, KC_MINUS) {
            register_code(KC_LSFT);
            register_code(KC_EQL);
            unregister_code(KC_EQL);
            unregister_code(KC_LSFT);
        }

        SEQ_ONE_KEY(KC_LSFT) {
            register_code(KC_CAPSLOCK);
            unregister_code(KC_CAPSLOCK);
        }
    }

    uint8_t layer = biton32(layer_state);

    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();

    switch (layer) {
      // TODO: Make this relevant to the ErgoDox EZ.
        case PLVR:
            ergodox_right_led_1_on();
            break;

        case 2:
            ergodox_right_led_2_on();
            break;

        default:
            // none
            break;
    }

    if (host_keyboard_leds() & (1 << USB_LED_CAPS_LOCK)) {
      ergodox_right_led_3_on();
    }
};
