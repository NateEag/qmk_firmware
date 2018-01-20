// An adaptation of the TrulyErgonomic's layout to the ErgoDox EZ.
//
// The main thing I like about the TrulyErgonomic's layout is the symmetry.
// Being able to do any non-letter keystroke with either hand has helped my
// wrist problems a lot, by letting me spread the stress evenly.
//
// The central location of Tab and Backspace has also helped, by moving load
// from the weaker pinky fingers to the stronger index fingers.
//
// Also includes a Plover layer, because I'm interested in learning Plover.

#include QMK_KEYBOARD_H
#include "debug.h"
#include "action_layer.h"
#include "led.h"

#define BASE 0 // default layer
#define PLVR 1 // Plover layer, ripped from pre-existing Plover layout
#define SYMB 2 // Function key layer
#define MDIA 3 // media keys

// TODO Figure out if I can use tap dance to create modifier/tap/double-tap keys.
//
// That would be *awesome*.
//
// I want to use some modifier keys as modifier/tap dual-function keys.
// The MT() macro's native behavior is bad for a fast typist, though -
// if the whole sequence takes less than TAPPING_TERM, you'll get the keycodes
// for the two keys you pressed, rather than modifier+key.
//
// I have therefore adopted a workaround proposed on GitHub:
//
// https://github.com/jackhumbert/qmk_firmware/issues/303#issue comment-217328415
//
// which means we only get the tap keycode if the tap was less than
// TAPPING_TERM *and* no other keys were pressed during that time.
#define TE_CTL_ESC 8

// Try tap dance for typing backslash.
enum {
  TD_BSLS = 0,
};

qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_BSLS] = ACTION_TAP_DANCE_DOUBLE(KC_GRV, KC_BSLS)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap 0: Basic layer
 *
 * Symbols surrounded by () can be triggered by tapping the key twice rapidly.
 * Their Shifted equivalent will appear if you double-tap while holding Shift.
 *
 * TODO Do something useful with the top center keys. Maybe a Hyper modifier?
 *
 * TODO Figure out what to do with the thumb Fn keys. I don't plan to use it
 * for much any more since I now have Fn by the GUI keys.
 *
 * TODO Make more use of mod/tap. I have some tappable keys that might be
 * moddable, and several mod keys that could be tapped (Alt, Shift, GUI, Fn).
 *
 * TODO Bind Caps Lock somewhere (it's useful for SQL and CONSTANT_NAMES).
 * Maybe Fn + Shift?
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * | ` (\)  |   1  |   2  |   3  |   4  |   5  |      |           |      |   6  |   7  |   8  |   9  |   0  |   -    |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |   /    |   Q  |   W  |   E  |   R  |   T  | Tab  |           | Tab  |   Y  |   U  |   I  |   O  |   P  |   =    |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * | Shift  |   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |   ;  | Shift  |
 * |--------+------+------+------+------+------| Back |           | Back |------+------+------+------+------+--------|
 * |Ctrl/Esc|   Z  |   X  |   C  |   V  |   B  | space|           | space|   N  |   M  |   ,  |   .  |   '  |Ctrl/Esc|
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |  Alt |  '"  |AltShf|  Fn  | LGui |                                       | RGui |  Fn  |   [  |   ]  | Alt  |
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
        TD(TD_BSLS),    KC_1,         KC_2,   KC_3,   KC_4,   KC_5,   KC_NO,
        KC_SLSH,        KC_Q,         KC_W,   KC_E,   KC_R,   KC_T,   KC_TAB,
        KC_LSFT,        KC_A,         KC_S,   KC_D,   KC_F,   KC_G,
        F(TE_CTL_ESC),  KC_Z,         KC_X,   KC_C,   KC_V,   KC_B,   KC_BSPC,
        KC_LALT,        KC_QUOT,      LALT(KC_LSFT),  MO(2),KC_LGUI,
                                              MO(2), TG(1),
                                                              KC_LEFT,
                                              KC_SPC, KC_ENT, KC_RIGHT,
        // right hand
        KC_NO,       KC_6,   KC_7,   KC_8,   KC_9,   KC_0,             KC_MINUS,
        KC_TAB,      KC_Y,   KC_U,   KC_I,   KC_O,   KC_P,             KC_EQL,
                     KC_H,   KC_J,   KC_K,   KC_L,   KC_SCLN,          KC_RSFT,
        KC_BSPC,     KC_N,   KC_M,   KC_COMM,KC_DOT, KC_QUOT,          F(TE_CTL_ESC),
                             KC_RGUI,  MO(2),KC_LBRC,KC_RBRC,          KC_RALT,
             TG(1),       MO(2),
             KC_UP,
             KC_DOWN,KC_ENT, KC_SPC
    ),

/* Keymap 1: Steno for Plover
 *
 * based on:
 *
 * https://github.com/shayneholmes/tmk_keyboard/commit/11290f8489013018f778627db725160c745e75bd
 *
 * I've basically shifted the fingers one column inward.
 *
 * That leaves my hands closer to the thumb keys, which I'm hoping will feel a
 * little better ergonomically.
 *
 * Since 't' and 'g' both just become '*', as do 'y' and 'h', I'm theorizing
 * that I only need one each of those pairs.
 *
 * We'll see how it pans out.
 *
 * TODO Maybe try moving 'c' and 'v'/'n' and 'm' to the top thumb keys? My hand
 * has felt too stretched trying basic steno, and I'm wondering if that would
 * be more comfortable. Would require changing my layer shift keys, but I have
 * some unused keys in the base layer anyway.
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |      |   1  |   2  |   3  |   4  |  5   |           |  6   |  7   |  8   |   9  |   0  |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |   q  |   w  |   e  |   r  |------|           |------|  u   |  i   |   o  |   p  |  [   |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |   a  |   s  |   d  |   f  |  g   |           |  h   |  j   |  k   |   l  |   ;  |  '   |        |
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
        // FIXME Looks like I have my numbers offset wrong here?
        KC_NO,  KC_1,   KC_2,   KC_3,   KC_4,   KC_5,   KC_5,
        KC_NO,  KC_NO,  KC_Q,   KC_W,   KC_E,   KC_R,
        KC_NO,  KC_NO,  KC_A,   KC_S,   KC_D,   KC_F,   KC_G,
        KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,
                                      KC_FN4, KC_NO,
                                           KC_NO,
                                 KC_C,   KC_V,   KC_NO,
        // right hand
        KC_TRNS,  KC_NO,   KC_NO,   KC_NO,  KC_NO,   KC_NO,   KC_TRNS,
        KC_6,     KC_7,    KC_8,    KC_9,   KC_0,    KC_TRNS, KC_TRNS,
                  KC_U,    KC_I,    KC_O,   KC_P,    KC_LBRC, KC_TRNS,
        KC_H,    KC_J,     KC_K,    KC_L,   KC_SCLN, KC_QUOT, KC_TRNS,
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
 * |--------+------+------+------+------+------| Del  |           | Del  |------+------+------+------+------+--------|
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
       KC_TRNS,           KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_DEL,
       LALT(LCTL(KC_DEL)),KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,
                                       KC_TRNS,KC_TRNS,
                                               KC_HOME,
                               KC_TRNS,KC_TRNS,KC_END,
       // right hand
       KC_PSCR,  KC_F6,   KC_F7,  KC_F8,   KC_F9,   KC_F10,  KC_F11,
       KC_TRNS,  KC_TRNS, KC_TRNS,KC_TRNS, KC_TRNS, KC_TRNS, KC_F12,
                 KC_TRNS, KC_TRNS,KC_RPRN, KC_RCBR, KC_RBRC, KC_TRNS,
       KC_DEL,   KC_TRNS, KC_TRNS,KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
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

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {
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
