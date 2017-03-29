#include "ergodox.h"
#include "debug.h"
#include "action_layer.h"
/* #include "keymap_extras/keymap_uk.h" */
#include "process_unicode.h"
#include "timer.h"

#define PREVENT_STUCK_MODIFIERS

#define BASE 0 // default layer
#define SYMB 1 // symbols
#define FKEY 2 // function keys
#define NPAD 3 // number pad


/*
  Author: Graeme Geldenhuys <graeme@geldenhuys.co.uk>
  Ver 1.7
  Based on the default Dvorak keymap (2016-08-16), and applied some
  tweaks from the TypeMatrix 2030 design. Some key locations are also
  designed to work well with my programming in Lazarus and Delphi.
*/

#define CUT      LCTL(KC_X)      // C-x Cut
#define COPY     LCTL(KC_C)      // C-c Copy
#define PASTE    LCTL(KC_V)      // C-v Paste
#define COMPI    LCTL(KC_F9)     // Ctrl+F9 used for compiling while developing with Object Pascal IDEs.

/* Custom keycodes */

enum {
  NONE = 0,

  // useful shortcuts
  MACRO_TODO,
  MACRO_ISEQUALS,
  MACRO_PARENTHESE,
  MDBL0,

  // Function / number keys
  KF_1, // 1, F1
  KF_2, // 2, F2
  KF_3, // ...
  KF_4,
  KF_5,
  KF_6,
  KF_7,
  KF_8,
  KF_9,
  KF_10,
  KF_11,
  KF_12
};

uint16_t kf_timers[12];


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap 0: Basic layer
 *
 * ╭────────┬──────┬──────┬──────┬──────┬──────┬──────╮           ╭──────┬──────┬──────┬──────┬──────┬──────┬────────╮
 * │  Esc   │  1  !│  2  @│  3  #│  4  $│  5  %│ C+F9 │           │ INS  │  6  ^│  7  &│  8  *│  9  (│  0  )│   \   |│
 * ├────────┼──────┼──────┼──────┼──────┼──────┼──────┤           ├──────┼──────┼──────┼──────┼──────┼──────┼────────┤
 * │  Tab   │ '   "│  ,  <│  .  >│  P   │   Y  │ DEL  │           │ BkSp │   F  │   G  │   C  │   R  │   L  │   /   ?│
 * ├────────┼──────┼──────┼──────┼──────┼──────┤      │           │      ├──────┼──────┼──────┼──────┼──────┼────────┤
 * │Shft =/+│   A  │   O  │   E  │  U • │   I  ├──────┤           ├──────┤   D  │ • H  │   T  │   N  │   S  │─ _ Shft│
 * ├────────┼──────┼──────┼──────┼──────┼──────┤  :=  │           │Enter ├──────┼──────┼──────┼──────┼──────┼────────┤
 * │ LShift │ ;/Ctl│   Q  │   J  │  K   │   X  │      │           │      │   B  │   M  │   W  │   V  │ Z/Ctl│ RShift │
 * ╰─┬──────┼──────┼──────┼──────┼──────┼──────┴──────╯           ╰──────┴──────┼──────┼──────┼──────┼──────┼──────┬─╯
 *   │LCtrl │ LAlt │ CUT  │ COPY │ PASTE│                                       │   ◀  │   ▲  │   ▼  │  ▶   │ RCtrl│
 *   ╰──────┴──────┴──────┴──────┴──────╯                                       ╰──────┴──────┴──────┴──────┴──────╯
 *                                        ╭──────┬──────╮       ╭──────┬──────╮
 *                                        │ CAPS │ RCtrl│       │ Home │ End  │
 *                                 ╭──────┼──────┼──────┤       ├──────┼──────┼──────╮
 *                                 │      │      │ Apps │       │ PgUp │      │      │
 *                                 │ ~L1  │Space ├──────┤       ├──────┤  ~L2 │ Space│
 *                                 │      │      │ PScr │       │ PgDn │      │      │
 *                                 ╰──────┴──────┴──────╯       ╰──────┴──────┴──────╯
 */
// If it accepts an argument (i.e, is a function), it doesn't need KC_.
// Otherwise, it needs KC_*
[BASE] = KEYMAP(  // layer 0 : default
        // left hand
        KC_ESC,      KC_1,           KC_2,     KC_3,     KC_4,     KC_5,   COMPI,
        KC_TAB,      KC_QUOT,        KC_COMM,  KC_DOT,   KC_P,     KC_Y,   KC_DELT,
        SFT_T(KC_EQL), KC_A,         KC_O,     KC_E,     KC_U,     KC_I,
        KC_LSFT,     CTL_T(KC_SCLN), KC_Q,     KC_J,     KC_K,     KC_X,   M(MACRO_ISEQUALS),
        KC_LCTRL,    KC_LALT,        CUT,      COPY,     PASTE,
                                             KC_CAPS, KC_RCTRL,
                                                 ALT_T(KC_APP),
                                     KC_FN1,  KC_SPC,  KC_PSCR,
        // right hand
             KC_INS,    KC_6,     KC_7,     KC_8,     KC_9,     KC_0,            KC_BSLS,
             KC_BSPC,   KC_F,     KC_G,     KC_C,     KC_R,     KC_L,            KC_SLSH,
                        KC_D,     KC_H,     KC_T,     KC_N,     KC_S,            SFT_T(KC_MINS),
             KC_ENT,    KC_B,     KC_M,     KC_W,     KC_V,     CTL_T(KC_Z),     KC_RSFT,
                                  KC_LEFT,  KC_UP,    KC_DOWN,  KC_RIGHT,        KC_RCTRL,
             KC_HOME,  KC_END,
             KC_PGUP,
             KC_PGDN, KC_FN2, KC_SPC
),
/* Keymap 1: Symbol Layer (red)
 *
 * ╭────────┬──────┬──────┬──────┬──────┬──────┬──────╮           ╭──────┬──────┬──────┬──────┬──────┬──────┬────────╮
 * │        │      │      │      │      │      │      │           │      │      │      │      │      │      │        │
 * ├────────┼──────┼──────┼──────┼──────┼──────┼──────┤           ├──────┼──────┼──────┼──────┼──────┼──────┼────────┤
 * │   |    │  !   │  @   │  {   │  }   │  +   │ TODO │           │      │      │  7   │  8   │  9   │  *   │        │
 * ├────────┼──────┼──────┼──────┼──────┼──────┤      │     1     │      ├──────┼──────┼──────┼──────┼──────┼────────┤
 * │   `    │  :   │  $   │  (   │  ) • │  =   ├──────┤           ├──────┤      │• 4   │  5   │  6   │  +   │        │
 * ├────────┼──────┼──────┼──────┼──────┼──────┤  ( ) │           │      ├──────┼──────┼──────┼──────┼──────┼────────┤
 * │        │  #   │  ^   │  [   │  ]   │  ~   │      │           │      │      │  1   │  2   │  3   │  \   │        │
 * ╰─┬──────┼──────┼──────┼──────┼──────┼──────┴──────╯           ╰──────┴──────┼──────┼──────┼──────┼──────┼──────┬─╯
 *   │      │  £   │      │      │      │                                       │  0   │  00  │  .   │  =   │ SLCK │
 *   ╰──────┴──────┴──────┴──────┴──────╯                                       ╰──────┴──────┴──────┴──────┴──────╯
 *                                        ╭──────┬──────╮       ╭──────┬────────╮
 *                                        │      │      │       │      │        │
 *                                 ╭──────┼──────┼──────┤       ├──────┼────────┼──────╮
 *                                 │      │      │      │       │      │        │      │
 *                                 │      │      ├──────┤       ├──────┤        │      │
 *                                 │      │      │      │       │      │        │      │
 *                                 ╰──────┴──────┴──────╯       ╰──────┴────────┴──────╯
 */
// SYMBOLS
[SYMB] = KEYMAP(
       // left hand
       KC_TRNS,  KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,   KC_TRNS,  KC_TRNS,
       KC_PIPE,  KC_EXLM,   KC_AT,     KC_LCBR,   KC_RCBR,   KC_PLUS,  M(MACRO_TODO),
       KC_GRV,   KC_COLN,   KC_DLR,    KC_LPRN,   KC_RPRN,   KC_EQL,
       KC_TRNS,  KC_HASH,   KC_CIRC,   KC_LBRC,   KC_RBRC,   KC_TILD,  M(MACRO_PARENTHESE),
       KC_TRNS,  UC(0x00A3),KC_TRNS,   KC_TRNS,   KC_TRNS,
                                       KC_TRNS,  KC_TRNS,
                                                 KC_TRNS,
                              KC_TRNS, KC_TRNS,  KC_TRNS,
       // right hand
       KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_7,   KC_8,    KC_9,    KC_ASTR, KC_TRNS,
                KC_TRNS, KC_4,   KC_5,    KC_6,    KC_PLUS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_1,   KC_2,    KC_3,    KC_BSLS, KC_TRNS,
                         KC_0,   M(MDBL0),KC_DOT,  KC_EQL,  KC_SLCK,
       KC_TRNS, KC_TRNS,
       KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS
),
/* Keymap 2: Function keys (green)
 *
 * ╭────────┬──────┬──────┬──────┬──────┬──────┬──────╮           ╭──────┬──────┬──────┬──────┬──────┬──────┬────────╮
 * │        │  F1  │  F2  │  F3  │  F4  │  F5  │      │           │      │ F6   │  F7  │  F8  │  F9  │ F10  │  F11   │
 * ├────────┼──────┼──────┼──────┼──────┼──────┼──────┤           ├──────┼──────┼──────┼──────┼──────┼──────┼────────┤
 * │        │      │      │      │      │      │      │           │      │      │      │      │      │      │  F12   │
 * ├────────┼──────┼──────┼──────┼──────┼──────┤      │     2     │      ├──────┼──────┼──────┼──────┼──────┼────────┤
 * │        │      │      │      │     •│      ├──────┤           ├──────┤      │•     │      │      │      │        │
 * ├────────┼──────┼──────┼──────┼──────┼──────┤      │           │      ├──────┼──────┼──────┼──────┼──────┼────────┤
 * │        │      │      │      │      │      │      │           │      │      │      │      │      │      │        │
 * ╰─┬──────┼──────┼──────┼──────┼──────┼──────┴──────╯           ╰──────┴──────┼──────┼──────┼──────┼──────┼──────┬─╯
 *   │      │      │      │      │      │                                       │      │      │      │      │ SLCK │
 *   ╰──────┴──────┴──────┴──────┴──────╯                                       ╰──────┴──────┴──────┴──────┴──────╯
 *                                        ╭──────┬──────╮       ╭──────┬────────╮
 *                                        │      │      │       │      │        │
 *                                 ╭──────┼──────┼──────┤       ├──────┼────────┼──────╮
 *                                 │      │      │      │       │      │        │      │
 *                                 │      │      ├──────┤       ├──────┤        │      │
 *                                 │      │      │      │       │      │        │      │
 *                                 ╰──────┴──────┴──────╯       ╰──────┴────────┴──────╯
 */
[FKEY] = KEYMAP(
       KC_TRNS, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                           KC_TRNS, KC_TRNS,
                                                    KC_TRNS,
                                  KC_TRNS, KC_TRNS, KC_TRNS,
    // right hand
       KC_TRNS,  KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10, KC_F11,
       KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_F12,
                 KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_SLCK,
       KC_TRNS, KC_TRNS,
       KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS
)
};

/* Empty template layer - copy and paste usage
 *
 * ╭────────┬──────┬──────┬──────┬──────┬──────┬──────╮           ╭──────┬──────┬──────┬──────┬──────┬──────┬────────╮
 * │        │      │      │      │      │      │      │           │      │      │      │      │      │      │        │
 * ├────────┼──────┼──────┼──────┼──────┼──────┼──────┤           ├──────┼──────┼──────┼──────┼──────┼──────┼────────┤
 * │        │      │      │      │      │      │      │           │      │      │      │      │      │      │        │
 * ├────────┼──────┼──────┼──────┼──────┼──────┤      │           │      ├──────┼──────┼──────┼──────┼──────┼────────┤
 * │        │      │      │      │      │      ├──────┤           ├──────┤      │      │      │      │      │        │
 * ├────────┼──────┼──────┼──────┼──────┼──────┤      │           │      ├──────┼──────┼──────┼──────┼──────┼────────┤
 * │        │      │      │      │      │      │      │           │      │      │      │      │      │      │        │
 * ╰─┬──────┼──────┼──────┼──────┼──────┼──────┴──────╯           ╰──────┴──────┼──────┼──────┼──────┼──────┼──────┬─╯
 *   │      │      │      │      │      │                                       │      │      │      │      │      │
 *   ╰──────┴──────┴──────┴──────┴──────╯                                       ╰──────┴──────┴──────┴──────┴──────╯
 *                                        ╭──────┬──────╮       ╭──────┬────────╮
 *                                        │      │      │       │      │        │
 *                                 ╭──────┼──────┼──────┤       ├──────┼────────┼──────╮
 *                                 │      │      │      │       │      │        │      │
 *                                 │      │      ├──────┤       ├──────┤        │      │
 *                                 │      │      │      │       │      │        │      │
 *                                 ╰──────┴──────┴──────╯       ╰──────┴────────┴──────╯
 */

const uint16_t PROGMEM fn_actions[] = {
    [1] = ACTION_LAYER_TAP_TOGGLE(SYMB),                // FN1 - Momentary Layer 1 (Symbols)
    [2] = ACTION_LAYER_TAP_TOGGLE(FKEY),                // FN2 - Momentary Layer 2 (F-keys)
    [3] = ACTION_LAYER_TAP_TOGGLE(NPAD)                 // FN3 - Momentary Layer 3 (Numpad)
};

void ang_handle_kf (keyrecord_t *record, uint8_t id)
{
  uint8_t code = id - KF_1;

  if (record->event.pressed) {
    kf_timers[code] = timer_read ();
  } else {
    uint8_t kc;

    if (timer_elapsed (kf_timers[code]) > TAPPING_TERM) {
      // Long press
      kc = KC_F1 + code;
    } else {
      if (id == KF_11)
        kc = KC_EQL;
      else
        kc = KC_1 + code;
    }

    register_code (kc);
    unregister_code (kc);
  }
}

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
  // MACRODOWN only works in this function
      switch(id) {
		case MACRO_TODO:
			if (record->event.pressed) {
				return MACRO( T(SLASH), T(SLASH), T(SPACE), D(LSHIFT), T(T), T(O), T(D), T(O), T(SCOLON), U(LSHIFT), T(SPACE), END );
			} 
			break;
		case MACRO_ISEQUALS:
			if (record->event.pressed) {
				return MACRO( D(LSFT), T(SCOLON), U(LSHIFT), T(EQL), T(SPC), END );
			} 
			break;
		case MACRO_PARENTHESE:
			if (record->event.pressed) {
				return MACRO(  D(LSHIFT),T(9), T(0),U(LSHIFT), T(LEFT), END);
			} 
			break;
		case MDBL0:
			if (record->event.pressed) {
				return MACRO( T(0), T(0), END );
			}
			break;
		/* Function keys */
		case KF_1 ... KF_10:
			ang_handle_kf (record, id);
			break;
      }
    return MACRO_NONE;

};

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
	
	set_unicode_input_mode(UC_BSD);
/*
	ergodox_led_all_on();
	for (int i = LED_BRIGHTNESS_HI; i > LED_BRIGHTNESS_LO; i--) {
		ergodox_led_all_set (i);
		wait_ms (5);
	}
	wait_ms(700);
	for (int i = LED_BRIGHTNESS_LO; i > 0; i--) {
		ergodox_led_all_set (i);
		wait_ms (10);
	}
	ergodox_led_all_off();
*/
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
        case SYMB:
            ergodox_right_led_1_on();
            break;
        case FKEY:
            ergodox_right_led_2_on();
            break;
        case NPAD:
            ergodox_right_led_3_on();
            break;
        default:
            // none
            break;
    }

};
