#ifndef USERSPACE
#define USERSPACE

#include "quantum.h"
#include "version.h"
#include "eeprom.h"

// Define layer names
enum userspace_layers {
  _BASE = 0,
  _LOWER,
  _RAISE,
  _FUNCTION,
  _SETTINGS,
  _ADJUST,
};

//define modifiers
#define MODS_SHIFT_MASK  (MOD_BIT(KC_LSHIFT)|MOD_BIT(KC_RSHIFT))
#define MODS_CTRL_MASK  (MOD_BIT(KC_LCTL)|MOD_BIT(KC_RCTRL))
#define MODS_ALT_MASK  (MOD_BIT(KC_LALT)|MOD_BIT(KC_RALT))
#define MODS_GUI_MASK  (MOD_BIT(KC_LGUI)|MOD_BIT(KC_RGUI))

// RGB color codes are no longer located here anymore.  Instead, you will want to
// head to https://github.com/qmk/qmk_firmware/blob/master/quantum/rgblight_list.h

#ifdef RGBLIGHT_ENABLE
void rgblight_sethsv_default_helper(uint8_t index);
#endif // RGBLIGHT_ENABLE

#define EECONFIG_USERSPACE (uint8_t *)19

typedef union {
  uint8_t raw;
  struct {
    // bool     clicky_enable    :1;
    bool     rgb_layer_change :1;
    // bool     is_overwatch     :1;
    // bool     nuke_switch      :1;
  };
} userspace_config_t;

// Define all of
enum userspace_custom_keycodes {
  PLACEHOLDER = SAFE_RANGE, // can always be here
  EPRM, 					// Resets EEPROM to defaults (as in eeconfig_init)
  VRSN,              		// Prints QMK Firmware and board info
  KC_MAKE,           		// Run keyboard's customized make command
  KC_RGB_T,          		// Toggles RGB Layer Indication mode
  KC_CCCV,           		// Hold to copy, tap to paste
  KC_NUKE,           		// NUCLEAR LAUNCH DETECTED!!!

#ifdef UNICODE_ENABLE
  UC_FLIP,           // Table flip (not working?)
#endif //UNICODE_ENABLE

  NEW_SAFE_RANGE     //use "NEW_SAFE_RANGE" for keymap specific codes
};

#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)
#define ADJUST MO(_ADJUST)
#define FUNCT MO(_FUNCTION)
#define SETTING TG(_SETTINGS)

#define KC_RESET RESET
#define KC_RST KC_RESET

#define KC_AE RALT(KC_A)
#define KC_OE RALT(KC_O)
#define KC_UE RALT(KC_U)
#define KC_SS RALT(KC_S)

#define FN_CAPS LT(_FUNCTION, KC_CAPS)

// Since our quirky block definitions are basically a list of comma separated
// arguments, we need a wrapper in order for these definitions to be
// expanded before being used as arguments to the LAYOUT_xxx macro.
#if (!defined(LAYOUT) && defined(KEYMAP))
#define LAYOUT KEYMAP
#endif

#define KEYMAP_wrapper(...)                  LAYOUT(__VA_ARGS__)
#define LAYOUT_wrapper(...)                  LAYOUT(__VA_ARGS__)
#define LAYOUT_60_wrapper(...)               LAYOUT_60(__VA_ARGS__)
#define LAYOUT_split_bs_rshift_wrapper(...)  LAYOUT_split_bs_rshift(__VA_ARGS__)

// Blocks for each of the four major keyboard layouts
// Organized so we can quickly adapt and modify all of them
// at once, rather than for each keyboard, one at a time.
// And this allows for much cleaner blocks in the keymaps.
// For instance Tap/Hold for Control on all of the layouts

// NOTE: These are all the same length.  If you do a search/replace
//       then you need to add/remove underscores to keep the
//       lengths consistent.

#define ________________________FROW________________________  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12
#define ______________________NUMBERS_______________________  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL
#define ______________________QWERTY_1______________________  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Z,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC
#define _________________QWERTY_2__________________  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT
#define _____________QWERTY_3_____________  KC_Y,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH

#define _____________DBLBLANK_____________  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
#define _____________DBLNONE______________  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX

#define _______________________FUNC_0_______________________  ________________________FROW________________________
#define _______________________FUNC_1_______________________  XXXXXXX, XXXXXXX, KC_PGUP, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_MPRV, KC_MNXT, KC_MPLY, KC_UP,   XXXXXXX
#define __________________FUNC_2___________________  XXXXXXX, KC_HOME, KC_PGDN, KC_END,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_LEFT, KC_RGHT
#define ______________FUNC_3______________  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_MUTE, KC_VOLD, KC_VOLU, KC_DOWN

#define _____________________SETTINGS_0_____________________  RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, KC_RGB_T, BL_STEP, BL_DEC,  BL_INC
#define _____________________SETTINGS_1_____________________  XXXXXXX, XXXXXXX, XXXXXXX, RESET,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
#define ________________SETTINGS_2_________________  XXXXXXX, _____________DBLNONE______________
#define ____________SETTINGS_3____________  XXXXXXX, XXXXXXX, XXXXXXX, VRSN,    XXXXXXX, XXXXXXX, KC_MAKE, XXXXXXX, XXXXXXX, XXXXXXX


#define _________________QWERTY_L1_________________        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T
#define _________________QWERTY_L2_________________        KC_A,    KC_S,    KC_D,    KC_F,    KC_G
#define _________________QWERTY_L3_________________        KC_Y,    KC_X,    KC_C,    KC_V,    KC_B

#define _________________QWERTY_R1_________________        KC_Z,    KC_U,    KC_I,    KC_O,    KC_P
#define _________________QWERTY_R2_________________        KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN
#define _________________QWERTY_R3_________________        KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLASH


#define ________________NUMBER_LEFT________________       KC_1,    KC_2,    KC_3,    KC_4,    KC_5
#define ________________NUMBER_RIGHT_______________       KC_6,    KC_7,    KC_8,    KC_9,    KC_0
#define _________________FUNC_LEFT_________________       KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5
#define _________________FUNC_RIGHT________________       KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10

#define ___________________BLANK___________________        _______, _______, _______, _______, _______


#define _________________LOWER_L0__________________        KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC
#define _________________LOWER_L1__________________        _______, _______, KC_UP,   _______, _______
#define _________________LOWER_L2__________________        _______, KC_LEFT, KC_DOWN, KC_RGHT, KC_LBRC
#define _________________LOWER_L3__________________        _______, _______, _______, _______, KC_LCBR

#define _________________LOWER_R0__________________        KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN
#define _________________LOWER_R1__________________        _______, KC_P7,   KC_P8,   KC_P9,   _______
#define _________________LOWER_R2__________________        KC_RBRC, KC_P4,   KC_P5,   KC_P6,   KC_PLUS
#define _________________LOWER_R3__________________        KC_RCBR, KC_P1,   KC_P2,   KC_P3,   KC_MINS



#define _________________RAISE_L0__________________        _________________FUNC_LEFT_________________
#define _________________RAISE_L1__________________        ___________________BLANK___________________
#define _________________RAISE_L2__________________        KC_AE,   KC_SS,   _______, KC_PGUP, KC_UNDS
#define _________________RAISE_L3__________________        _______, _______, _______, KC_PGDN, KC_MINS

#define _________________RAISE_R0__________________        _________________FUNC_RIGHT________________
#define _________________RAISE_R1__________________        _______, KC_UE,   _______, KC_OE,   KC_MPLY
#define _________________RAISE_R2__________________        KC_EQL,  KC_HOME, KC_END,  _______, _______
#define _________________RAISE_R3__________________        _______, KC_MUTE, KC_VOLD, KC_VOLU, _______



#define _________________ADJUST_L0_________________        ___________________BLANK___________________
#define _________________ADJUST_L1_________________        RGB_MOD, RGB_HUI, RGB_SAI, RGB_VAI, RGB_TOG
#define _________________ADJUST_L2_________________        ___________________BLANK___________________
#define _________________ADJUST_L3_________________        RGB_RMOD,RGB_HUD,RGB_SAD, RGB_VAD, KC_RGB_T

#define _________________ADJUST_R0_________________        ___________________BLANK___________________
#define _________________ADJUST_R1_________________        _______, _______, KC_MPRV, KC_MNXT, KC_MPLY
#define _________________ADJUST_R2_________________        ___________________BLANK___________________
#define _________________ADJUST_R3_________________        ___________________BLANK___________________


// Since we have 4 default layouts (QWERTY, DVORAK, COLEMAK and WORKMAN),
// this allows us to quickly modify the bottom row for all of the layouts
// so we don't have to alter it 4 times and hope that we haven't missed
// anything
#define ___________ERGODOX_BOTTOM_LEFT_____________       OSM(MOD_MEH), OSM(MOD_LGUI), KC_LBRC, KC_RBRC
#define ___________ERGODOX_BOTTOM_RIGHT____________       KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT


#define ___________ORTHODOX_THUMP_TOP_____________           ALT_APP, OS_LGUI,         KC_LALT, OS_RGUI
#define ___________________ORTHODOX_THUMB_BOTTOM____________________ LOWER,   KC_SPACE,KC_BSPC,         KC_DEL,  KC_ENT,  RAISE


#define __________________ERGODOX_THUMB_CLUSTER_____________________           ALT_T(KC_APP), OSM(MOD_LGUI),           OSM(MOD_RGUI), CTL_T(KC_ESCAPE), \
                                                                                              KC_HOME,                 KC_PGUP, \
                                                                LT(_LOWER, KC_SPACE),KC_BSPC, KC_END,                  KC_PGDN, KC_DEL,  LT(_RAISE, KC_ENTER)





#endif