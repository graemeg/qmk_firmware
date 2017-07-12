/* jedit settings  :elasticTabstops=true: */

#include "ergodox.h"
#include "debug.h"
#include "action_layer.h"
/* #include "keymap_extras/keymap_uk.h" */
#include "process_unicode.h"
#include "version.h"

#define PREVENT_STUCK_MODIFIERS

#define BASE 0 // default layer
#define SYMB 1 // symbols
#define FKEY 2 // function keys
#define QWER 3 // QWERTY


/*
  Author: Graeme Geldenhuys <graeme@geldenhuys.co.uk>
  Ver 1.12   (2017-05-12)
  Based on the default Dvorak keymap, and applied some
  tweaks from the TypeMatrix 2030 design. Some key locations are also
  designed to work well with my programming environment (FPC +Lazarus).
*/

#define CUT      LCTL(KC_X)      // C-x Cut
#define COPY     LCTL(KC_C)      // C-c Copy
#define PASTE    LCTL(KC_V)      // C-v Paste
#define COMPI    LCTL(KC_F9)     // Ctrl+F9 used for compiling while developing with Object Pascal IDEs.

/* Custom keycodes */

enum {
  NONE = 0, // SAFE_RANGE, // can always be here

  // useful shortcuts
  MACRO_TODO,
  MACRO_ISEQUALS,
  MACRO_PARENTHESE,
  MDBL0,
  EPRM,
  VRSN,
  POUND,
  EURO
};

// If it accepts an argument (i.e, is a function), it doesn't need KC_.
// Otherwise, it needs KC_*
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[0] = KEYMAP(
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
	// left hand
	KC_ESC,	KC_1,	KC_2,	KC_3,	KC_4,	KC_5,	COMPI,
	KC_TAB,	KC_QUOT,	KC_COMM,	KC_DOT,	KC_P,	KC_Y,	KC_DELT,
	SFT_T(KC_EQL),	KC_A,	SFT_T(KC_O),	CTL_T(KC_E),	ALT_T(KC_U),	KC_I,
	KC_LSFT,	CTL_T(KC_SCLN),	KC_Q,	KC_J,	KC_K,	KC_X,	M(MACRO_ISEQUALS),
	KC_LCTRL,	KC_LALT,	CUT,	COPY,	PASTE,

	KC_CAPS,	KC_RCTRL,
	ALT_T(KC_APP),
	MO(1),	KC_SPC,	KC_PSCR,

	// right hand
	KC_INS,	KC_6,	KC_7,	KC_8,	KC_9,	KC_0,	KC_BSLS,
	KC_BSPC,	KC_F,	KC_G,	KC_C,	KC_R,	KC_L,	KC_SLSH,
	KC_D,	ALT_T(KC_H),	CTL_T(KC_T),	SFT_T(KC_N),	KC_S,	SFT_T(KC_MINS),
	KC_ENT,	KC_B,	KC_M,	KC_W,	KC_V,	CTL_T(KC_Z),	KC_RSFT,
	KC_LEFT,	KC_UP,	KC_DOWN,	KC_RIGHT,	KC_RCTRL,

	KC_HOME,	KC_END,
	KC_PGUP,
	KC_PGDN,	MO(2),	KC_SPC
),

[1] = KEYMAP(
/* Keymap 1: Symbol Layer (red)
 *
 * ╭────────┬──────┬──────┬──────┬──────┬──────┬──────╮           ╭──────┬──────┬──────┬──────┬──────┬──────┬────────╮
 * │        │      │      │      │      │      │      │           │ QWER │      │      │      │      │      │        │
 * ├────────┼──────┼──────┼──────┼──────┼──────┼──────┤           ├──────┼──────┼──────┼──────┼──────┼──────┼────────┤
 * │   |    │  !   │  @   │  {   │  }   │  +   │ TODO │           │      │      │ Home │  ▲   │  End │      │  PgUp  │
 * ├────────┼──────┼──────┼──────┼──────┼──────┤      │     1     │      ├──────┼──────┼──────┼──────┼──────┼────────┤
 * │   `    │  :   │  $   │  (   │  ) • │  =   ├──────┤           ├──────┤      │•  ◀  │  ▼   │   ▶  │      │  PgDn  │
 * ├────────┼──────┼──────┼──────┼──────┼──────┤  ( ) │           │      ├──────┼──────┼──────┼──────┼──────┼────────┤
 * │        │  #   │  ^   │  [   │  ]   │  ~   │      │           │      │      │      │      │      │      │        │
 * ╰─┬──────┼──────┼──────┼──────┼──────┼──────┴──────╯           ╰──────┴──────┼──────┼──────┼──────┼──────┼──────┬─╯
 *   │      │  £   │  €   │      │      │                                       │      │      │      │      │ SLCK │
 *   ╰──────┴──────┴──────┴──────┴──────╯                                       ╰──────┴──────┴──────┴──────┴──────╯
 *                                        ╭──────┬──────╮       ╭──────┬────────╮
 *                                        │      │      │       │      │        │
 *                                 ╭──────┼──────┼──────┤       ├──────┼────────┼──────╮
 *                                 │      │      │      │       │      │        │      │
 *                                 │      │      ├──────┤       ├──────┤        │      │
 *                                 │      │      │      │       │      │        │      │
 *                                 ╰──────┴──────┴──────╯       ╰──────┴────────┴──────╯
 */
	// left hand  POUND = UC(0x00A3)
	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,
	KC_PIPE,	KC_EXLM,	KC_AT,	KC_LCBR,	KC_RCBR,	KC_PLUS,	M(MACRO_TODO),
	KC_GRV,	KC_COLN,	KC_DLR,	KC_LPRN,	KC_RPRN,	KC_EQL,
	KC_TRNS,	KC_HASH,	KC_CIRC,	KC_LBRC,	KC_RBRC,	KC_TILD,	M(MACRO_PARENTHESE),
	KC_TRNS,	M(POUND),	M(EURO),	KC_TRNS,	KC_TRNS,
	KC_TRNS,	KC_TRNS,
	KC_TRNS,
	KC_TRNS,	KC_TRNS,	KC_TRNS,

	// right hand
	KC_FN3,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,
	KC_TRNS,	KC_TRNS,	KC_HOME,	KC_UP,	KC_END,	KC_TRNS,	KC_PGUP,
		KC_TRNS,	KC_LEFT,	KC_DOWN,	KC_RIGHT,	KC_TRNS,	KC_PGDN,
	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,
	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_SLCK,
	KC_TRNS,	KC_TRNS,
	KC_TRNS,
	KC_TRNS,	KC_TRNS,	KC_TRNS
),

[2] = KEYMAP(
/* Keymap 2: Function keys (green) & shortcuts. Gui key (aka Super key). Meta key (aka Alt key).
 * MEH:   Alt+Control+Shift
 * HYPER: Alt+Control+Shift+Gui
 * ╭────────┬──────┬──────┬──────┬──────┬──────┬──────╮           ╭──────┬──────┬──────┬──────┬──────┬──────┬────────╮
 * │        │      │      │      │      │      │      │           │      │      │      │      │      │      │        │
 * ├────────┼──────┼──────┼──────┼──────┼──────┼──────┤           ├──────┼──────┼──────┼──────┼──────┼──────┼────────┤
 * │        │ Hyp-1│ Hyp-2│ Hyp-3│  H + │      │      │           │      │      │  F1  │  F2  │  F3  │  F4  │        │
 * ├────────┼──────┼──────┼──────┼──────┼──────┤      │     2     │      ├──────┼──────┼──────┼──────┼──────┼────────┤
 * │        │ Hyp-4│ Hyp-5│ Hyp-6│  H -•│      ├──────┤           ├──────┤      │• F5  │  F6  │  F7  │  F8  │        │
 * ├────────┼──────┼──────┼──────┼──────┼──────┤      │           │      ├──────┼──────┼──────┼──────┼──────┼────────┤
 * │        │ Hyp-7│ Hyp-8│ Hyp-9│      │      │      │           │      │      │  F9  │  F10 │  F11 │  F12 │        │
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
	// left hand
	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,
	KC_TRNS,	MEH(KC_1),	HYPR(KC_2),	HYPR(KC_3),	ALL_T(KC_KP_PLUS),	KC_TRNS,	KC_TRNS,
	KC_TRNS,	MEH(KC_4),	HYPR(KC_5),	HYPR(KC_6),	ALL_T(KC_KP_MINUS),	KC_TRNS,
	KC_TRNS,	HYPR(KC_7),	HYPR(KC_8),	HYPR(KC_9),	KC_TRNS,	KC_TRNS,	KC_TRNS,
	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,

	KC_TRNS,	KC_TRNS,
	KC_TRNS,
	KC_TRNS,	KC_TRNS,	KC_TRNS,

	// right hand
	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,
	KC_TRNS,	KC_TRNS,	KC_F1,	KC_F2,	KC_F3,	KC_F4,	KC_TRNS,
	KC_TRNS,	KC_F5,	KC_F6,	KC_F7,	KC_F8,	KC_TRNS,
	KC_TRNS,	KC_TRNS,	KC_F9,	KC_F10,	KC_F11,	KC_F12,	KC_TRNS,
	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,	KC_TRNS,

	KC_TRNS,	KC_TRNS,
	KC_TRNS,
	KC_TRNS,	KC_TRNS,	KC_TRNS
),

[3] = KEYMAP(
/* Keymap 1: QWERTY layer (games)
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |      |      |      |      |      |  L0  |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |   Q  |   W  |   E  |   R  |   T  |      |           |      |   Y  |   U  |   I  |   O  |   P  |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |   ;  |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |   Z  |   X  |   C  |   V  |   B  |      |           |      |   N  |   M  |      |      |      |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      |      |      |                                       |      |      |      |      | RESET|
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+--------+------.
 *                                 |      |      |      |       |      |        |      |
 *                                 |      |      |------|       |------|        |      |
 *                                 |      |      | VER  |       |      |        |      |
 *                                 `--------------------'       `----------------------'
 */
       KC_TRNS,        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_FN3,
       KC_TRNS,        KC_Q,       KC_W,       KC_E,       KC_R,       KC_T,       KC_TRNS,
       KC_TRNS,        KC_A,       KC_S,       KC_D,       KC_F,       KC_G,
       KC_TRNS,        KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,       KC_TRNS,
       KC_TRNS,        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,

                       KC_TRNS,    KC_TRNS,
                                   KC_TRNS,
       KC_TRNS,        KC_TRNS,    M(VRSN),

    // right hand
       KC_TRNS,        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
       KC_TRNS,        KC_Y,       KC_U,       KC_I,       KC_O,       KC_P,       KC_TRNS,
                       KC_H,       KC_J,       KC_K,       KC_L,       KC_SCLN,    KC_TRNS,
       KC_TRNS,        KC_N,       KC_M,       KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
                                   KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    RESET,
       KC_TRNS,        KC_TRNS,
       KC_TRNS,
       KC_TRNS,        KC_TRNS,    KC_TRNS
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
    [3] = ACTION_LAYER_TAP_TOGGLE(QWER)                 // FN3 - Momentary Layer 3 (Numpad)
};


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
		case EPRM:
			if (record->event.pressed) { // For resetting EEPROM
				eeconfig_init();
			}
			break;
		case VRSN:
			if (record->event.pressed) {
				SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
			}
			break;
		case POUND:
			if (record->event.pressed) {
				return MACRO( D(LCTRL), D(LSHIFT), T(U), U(LSHIFT), U(LCTRL), T(0), T(0), T(A), T(3), T(ENTER), END);
			}
			break;
		case EURO:
			if (record->event.pressed) {
				return MACRO( D(LCTRL), D(LSHIFT), T(U), U(LSHIFT), U(LCTRL), T(2), T(0), T(A), T(C), T(ENTER), END);
			}
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

//    static uint8_t state;
    uint8_t layer = biton32(layer_state);

    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();

    //reduce LED on time to 1/6th because LEDs are too strong
//    if (++state < 6) return;
//    state = 0;

    switch (layer) {
        case 1:
            ergodox_right_led_1_on();
            break;
        case 2:
            ergodox_right_led_2_on();
            break;
        case 3:
            ergodox_right_led_3_on();
            break;
        case 4:
            ergodox_right_led_1_on();
            ergodox_right_led_2_on();
            break;
        case 5:
            ergodox_right_led_1_on();
            ergodox_right_led_3_on();
            break;
        case 6:
            ergodox_right_led_2_on();
            ergodox_right_led_3_on();
            break;
        case 7:
            ergodox_right_led_1_on();
            ergodox_right_led_2_on();
            ergodox_right_led_3_on();
            break;
        default:
            break;
    }

};
