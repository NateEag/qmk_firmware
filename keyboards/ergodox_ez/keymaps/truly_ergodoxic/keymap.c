#include QMK_KEYBOARD_H
#include "version.h"

// An adaptation of the TrulyErgonomic's layout to the ErgoDox EZ.
//
// The main thing I like about the TrulyErgonomic's layout is the symmetry.
// Being able to do any non-letter keystroke with either hand has helped my
// wrist problems a lot, by letting me spread the stress evenly.
//
// The central location of Tab and Backspace has also helped, by moving load
// from the weaker pinky fingers to the stronger index fingers.
//
// Also includes a Plover layer, because I'm interested in learning
// stenography.

enum layers {
    BASE,  // default layer
    PLVR,  // Plover layer, ripped from pre-existing Plover layout
    SYMB,  // Function key layer
    NMPD   // Numeric key
};

enum custom_keycodes {
    VRSN = SAFE_RANGE,
    // On many Apple laptops, Caps Lock must be depressed for at least 200
    // milliseconds before it will register:
    // https://web.archive.org/web/20150115033359/https://support.apple.com/en-us/HT201509
    //
    // I've opted to work around this in hardware, so that Caps Lock should Just
    // Work no matter what computer I have my keyboard plugged into.
    //
    // The idea to use a macro to work around this "feature" came from a GitHub
    // discussion:
    //
    // https://github.com/qmk/qmk_firmware/issues/2664#issuecomment-939075333
    INSTA_CAPS_LOCK
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch(keycode) {
  case INSTA_CAPS_LOCK:
    if (record->event.pressed) {
      SEND_STRING(SS_TAP(X_CAPS));
    }
    break;
  }

  return true;
}


// The following abstractions were adapted from the docs, which provide an
// example of how to implement hold/single-tap/double-tap/double-tap-and-hold
// keys:
//
// https://docs.qmk.fm/feature_tap_dance.html#example-4-quad-function-tap-dance
//
// This layout uses them to implement modifier keys with single- and double-tap
// functions.
//
// Note that the cur_dance function has been adapted to handle tap/modifier
// keys differently from the original. Details on why are inline.

//**************** Definitions needed for quad function to work *********************//
//Enums used to clearly convey the state of the tap dance
enum {
  SINGLE_TAP = 1,
  SINGLE_HOLD = 2,
  DOUBLE_TAP = 3,
  DOUBLE_HOLD = 4,
  DOUBLE_SINGLE_TAP = 5 //send SINGLE_TAP twice - NOT DOUBLE_TAP
  // Add more enums here if you want for triple, quadruple, etc.
};

typedef struct {
  bool is_press_action;
  int state;
} tap;

int cur_dance (tap_dance_state_t *state) {
  if (state->count == 1) {
    // Modified from original. The goal is to keep new modifiers from
    // interrupting held tap-dance modifiers, so when I hold a tap dance
    // modifier/single-tap key then press another modifier with it, it doesn't
    // send the single-tap instead of registering the hold. (I use this to map
    // a single key to Escape when tapped and Control when held, and I use
    // Ctrl+Shift a lot).
    //
    // This change may be responsible for the sudden breakage of my Ctrl+Shift+a
    // shortcut for activating screensaver - it apparently sends a delayed
    // event after I'm done with the keystroke, which smells a lot like tap
    // dance not doing quite what I want...
    if ((state->interrupted && state->pressed!=1) || state->pressed==0) {
      return SINGLE_TAP;
    } else {
      return SINGLE_HOLD;
    }
  }
  // If count = 2, and it has been interrupted - assume that user is trying to type the letter associated
  // with single tap. In example below, that means to send `xx` instead of `Escape`.
  else if (state->count == 2) {
    if (state->interrupted) return DOUBLE_SINGLE_TAP;
    else if (state->pressed) return DOUBLE_HOLD;
    else return DOUBLE_TAP;
  }
  else return 6; //magic number. At some point this method will expand to work for more presses
}

//**************** END Definitions needed for quad function to work *********************//


// Definitions for modifier tap dance keys.

static tap ctrltap_state = {
  .is_press_action = true,
  .state = 0
};

void ctrl_finished(tap_dance_state_t *state, void *user_data) {
  ctrltap_state.state = cur_dance(state);
  switch (ctrltap_state.state) {
    case SINGLE_TAP: register_code(KC_ESC); break;
    case SINGLE_HOLD: register_code(KC_LCTL); break;
    // There are some cases where I want to send Escape twice quickly, so I
    // bind double-tap to do exactly that.
    case DOUBLE_TAP: register_code(KC_ESC); unregister_code(KC_ESC); break;
  }
}

void ctrl_reset(tap_dance_state_t *state, void *user_data) {
  switch (ctrltap_state.state) {
    case SINGLE_TAP: unregister_code(KC_ESC); break;
    case SINGLE_HOLD: unregister_code(KC_LCTL); break;
    case DOUBLE_TAP: register_code(KC_ESC); unregister_code(KC_ESC); break;
  }

  ctrltap_state.state = 0;
}

// END Definitions for modifier tap dance keys.

// Declare tap dance key identifiers for use in the actual keymaps.
enum {
  TD_CTRL_ESC,
  TD_RGUI_F19,
  TD_LGUI_F19
};

tap_dance_action_t tap_dance_actions[] = {
  [TD_CTRL_ESC] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ctrl_finished, ctrl_reset),
  [TD_RGUI_F19] = ACTION_TAP_DANCE_DOUBLE(KC_RGUI, KC_F19),
  [TD_LGUI_F19] = ACTION_TAP_DANCE_DOUBLE(KC_LGUI, KC_F19)
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap 0: Basic layer
 *
 * TODO Do something additional with the top center keys. Maybe a Hyper
 * modifier on double-tap-and-hold?
 *
 * TODO Figure out what to do with the thumb Fn keys. I don't plan to use it
 * for much any more since I now have Fn by the GUI keys.
 *
 * TODO Make more use of mod/tap. I have some tappable keys that might be
 * moddable, and several mod keys that could be tapped (Alt, Shift, GUI, Fn).
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |   `    |   1  |   2  |   3  |   4  |   5  |  -   |           |  =   |   6  |   7  |   8  |   9  |   0  |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |   /    |   Q  |   W  |   E  |   R  |   T  | Tab  |           | Tab  |   Y  |   U  |   I  |   O  |   P  |   \    |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * | Shift  |   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |   ;  | Shift  |
 * |--------+------+------+------+------+------| Back |           | Back |------+------+------+------+------+--------|
 * |Ctrl/Esc|   Z  |   X  |   C  |   V  |   B  | space|           | space|   N  |   M  |   ,  |   .  |   '  |Ctrl/Esc|
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |  Alt |  '"  |AltShf|  Fn  | LGui |                                       | RGui |  Fn  |   [  |   ]  | Alt  |
 *   `----------------------------------'                                       `----------------------------------'
 *
 *                                        ,-------------.       ,-------------.
 *                                        |CapsLk|      |       |Plover|       |
 *                                 ,------|------|------|       |------+-------+------.
 *                                 |      |      | Left |       |  Up  |       |      |
 *                                 | Space| Enter|------|       |------| Enter |Space |
 *                                 |      |      |Right |       | Down |       |      |
 *                                 `--------------------'       `----------------------'
 */
// If it accepts an argument (i.e, is a function), it doesn't need KC_.
// Otherwise, it needs KC_*
[BASE] = LAYOUT_ergodox(  // layer 0 : default
        // left hand
        KC_GRV,         KC_1,         KC_2,   KC_3,   KC_4,   KC_5,   KC_MINUS,
        KC_SLSH,        KC_Q,         KC_W,   KC_E,   KC_R,   KC_T,   KC_TAB,
        KC_LSFT,        KC_A,         KC_S,   KC_D,   KC_F,   KC_G,
        TD(TD_CTRL_ESC), KC_Z,         KC_X,   KC_C,   KC_V,   KC_B,   KC_BSPC,
        KC_LALT,        KC_QUOT,      LALT(KC_LSFT),  MO(2),TD(TD_LGUI_F19),
                                              INSTA_CAPS_LOCK, KC_NO,
                                                              KC_LEFT,
                                              KC_SPC, KC_ENT, KC_RIGHT,
        // right hand
        KC_EQL,       KC_6,   KC_7,   KC_8,   KC_9,   KC_0,             KC_DEL,
        KC_TAB,      KC_Y,   KC_U,   KC_I,   KC_O,   KC_P,             KC_BSLS,
                     KC_H,   KC_J,   KC_K,   KC_L,   KC_SCLN,          KC_RSFT,
        KC_BSPC,     KC_N,   KC_M,   KC_COMM,KC_DOT, KC_QUOT,          TD(TD_CTRL_ESC),
                             TD(TD_RGUI_F19),  MO(2),KC_LBRC,KC_RBRC,          KC_RALT,
             TG(PLVR),       TG(NMPD),
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

[PLVR] = LAYOUT_ergodox(  // layout: layer 1: Steno for Plover
        // left hand
        KC_NO, KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,
        // FIXME Looks like I have my numbers offset wrong here?
        KC_NO,  KC_1,   KC_2,   KC_3,   KC_4,   KC_5,   KC_5,
        KC_NO,  KC_NO,  KC_Q,   KC_W,   KC_E,   KC_R,
        KC_NO,  KC_NO,  KC_A,   KC_S,   KC_D,   KC_F,   KC_G,
        KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,
                                      KC_TRNS, KC_TRNS,
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
 * |        |      |MsWhDn| MsUp |MsWhUp|      |      |           |      |      |      |      |      |      |   F12  |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        | Rclk |MsLeft|MsDown|MsRght| Lclk |------|           |------|      | VolDn| VolUp| Play | Stop |        |
 * |--------+------+------+------+------+------| Del  |           | Del  |------+------+------+------+------+--------|
 * |        |      |BrtUp |BrtDwn|      |      |      |           |      |      |      |      |      |      |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |CADel |      |      |      |      |                                       |      |      |      |      |      |
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
[SYMB] = LAYOUT_ergodox(
       // left hand
       KC_TRNS,           KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_INS,
       KC_TRNS, KC_TRNS, KC_MS_WH_DOWN, KC_MS_U, KC_MS_WH_UP, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_BTN2, KC_MS_L, KC_MS_D, KC_MS_R, KC_BTN1,
       KC_TRNS,           KC_TRNS,KC_BRID,KC_BRIU,KC_TRNS,KC_TRNS,KC_DEL,
       LALT(LCTL(KC_DEL)),KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,
                                       KC_TRNS,KC_TRNS,
                                               KC_HOME,
                               KC_TRNS,KC_TRNS,KC_END,
       // right hand
       KC_PSCR,  KC_F6,   KC_F7,  KC_F8,   KC_F9,   KC_F10,  KC_F11,
       KC_TRNS,  KC_TRNS, KC_TRNS,KC_TRNS, KC_TRNS, KC_TRNS, KC_F12,
                 KC_TRNS, KC_VOLD, KC_VOLU, KC_MPLY,KC_MSTP, KC_TRNS,
       KC_DEL,   KC_TRNS, KC_TRNS,KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                          KC_TRNS,KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS,
       KC_TRNS,  KC_TRNS,
       KC_PGUP,
       KC_PGDN,KC_TRNS, KC_TRNS
),

/* Keymap 3: Numeric Keypad / inverted-t arrow keys
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |      |      |  Up  |      |      |      |           |      |      |  7   |  8   |  9   |  +   |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      | Left | Down | Right|      |------|           |------| Tab  |  4   |  5   |  6   |  .   |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      |      |  1   |  2   |  3   |  -   |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      |      |      |                                       |  0   |  *   |  /   |      |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 |      |      |------|       |------|      |Enter |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
//
LAYOUT_ergodox(
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_UP, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_LEFT, KC_DOWN, KC_RIGHT, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                           KC_TRNS, KC_TRNS,
                                                    KC_TRNS,
                                  KC_TRNS, KC_TRNS, KC_TRNS,
    // right hand
       KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS,  KC_TRNS, KC_7, KC_8, KC_9, KC_PLUS, KC_TRNS,
                 KC_TAB, KC_4, KC_5, KC_6, KC_DOT, KC_TRNS,
       KC_TRNS,  KC_TRNS, KC_1, KC_2, KC_3, KC_MINUS, KC_TRNS,
                          KC_0, KC_ASTERISK, KC_SLASH, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS,
       KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS
),
};

const uint16_t PROGMEM fn_actions[] = {
  [1] = ACTION_LAYER_TAP_TOGGLE(SYMB)                // FN1 - Momentary Layer 1 (Symbols)
};

// Runs whenever there is a layer state change.
layer_state_t layer_state_set_user(layer_state_t state) {
    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();

    uint8_t layer = get_highest_layer(state);
    switch (layer) {
        case 0:
#ifdef RGBLIGHT_COLOR_LAYER_0
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_0);
#endif
            break;
        case 1:
            ergodox_right_led_1_on();
#ifdef RGBLIGHT_COLOR_LAYER_1
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_1);
#endif
            break;
        case 2:
            ergodox_right_led_2_on();
#ifdef RGBLIGHT_COLOR_LAYER_2
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_2);
#endif
            break;
        case 3:
            ergodox_right_led_3_on();
#ifdef RGBLIGHT_COLOR_LAYER_3
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_3);
#endif
            break;
        case 4:
            ergodox_right_led_1_on();
            ergodox_right_led_2_on();
#ifdef RGBLIGHT_COLOR_LAYER_4
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_4);
#endif
            break;
        case 5:
            ergodox_right_led_1_on();
            ergodox_right_led_3_on();
#ifdef RGBLIGHT_COLOR_LAYER_5
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_5);
#endif
            break;
        case 6:
            ergodox_right_led_2_on();
            ergodox_right_led_3_on();
#ifdef RGBLIGHT_COLOR_LAYER_6
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_6);
#endif
            break;
        case 7:
            ergodox_right_led_1_on();
            ergodox_right_led_2_on();
            ergodox_right_led_3_on();
#ifdef RGBLIGHT_COLOR_LAYER_7
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_7);
#endif
            break;
        default:
            break;
    }

    return state;
};

