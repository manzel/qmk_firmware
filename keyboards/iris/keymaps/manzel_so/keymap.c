#include QMK_KEYBOARD_H
#include "pincontrol.h"

extern keymap_config_t keymap_config;

enum keymap_layers {
  _BASE = 0,
  _LOWER,
  _RAISE,
  _ADJUST,
};

#define SOLENOID_DEFAULT_DWELL 12
#define SOLENOID_MAX_DWELL 100
#define SOLENOID_MIN_DWELL 4
#define SOLENOID_PIN C6

bool solenoid_enabled = false;
bool solenoid_on = false;
bool solenoid_buzz = false;
bool solenoid_buzzing = false;
uint16_t solenoid_start = 0;
uint8_t solenoid_dwell = SOLENOID_DEFAULT_DWELL;

void solenoid_buzz_on(void) {
  solenoid_buzz = true;
}

void solenoid_buzz_off(void) {
  solenoid_buzz = false;
}

void solenoid_dwell_minus(void) {
  if (solenoid_dwell > 0) solenoid_dwell--;
}

void solenoid_dwell_plus(void) {
  if (solenoid_dwell < SOLENOID_MAX_DWELL) solenoid_dwell++;
}

void solenoid_toggle(void) {
  solenoid_enabled = !solenoid_enabled;
}

void solenoid_stop(void) {
  digitalWrite(SOLENOID_PIN, PinLevelLow);
  solenoid_on = false;
  solenoid_buzzing = false;
}

void solenoid_fire(void) {
  if (!solenoid_enabled) return;

  if (!solenoid_buzz && solenoid_on) return;
  if (solenoid_buzz && solenoid_buzzing) return;

  solenoid_on = true;
  solenoid_buzzing = true;
  solenoid_start = timer_read();
  digitalWrite(SOLENOID_PIN, PinLevelHigh);
}

void solenoid_check(void) {
  uint16_t elapsed = 0;

  if (!solenoid_on) return;

  elapsed = timer_elapsed(solenoid_start);

  //Check if it's time to finish this solenoid click cycle
  if (elapsed > solenoid_dwell) {
    solenoid_stop();
    return;
  }

  //Check whether to buzz the solenoid on and off
  if (solenoid_buzz) {
    if (elapsed / SOLENOID_MIN_DWELL % 2 == 0){
      if (!solenoid_buzzing) {
        solenoid_buzzing = true;
        digitalWrite(SOLENOID_PIN, PinLevelHigh);
      }
    }
    else {
      if (solenoid_buzzing) {
        solenoid_buzzing = false;
        digitalWrite(SOLENOID_PIN, PinLevelLow);
      }
    }
  }

}

void solenoid_setup(void) {
  pinMode(SOLENOID_PIN, PinDirectionOutput);
}

void matrix_init_keymap(void) {
  solenoid_setup();
}

void matrix_scan_keymap(void) {
  solenoid_check();
}

enum custom_keycodes {
  SOLENOID_TOG = SAFE_RANGE,
  SOLENOID_DWELL_MINUS,
  SOLENOID_DWELL_PLUS,
  SOLENOID_BUZZ_ON,
  SOLENOID_BUZZ_OFF
};

#define SOL_TOG SOLENOID_TOG
#define SOL_DWD SOLENOID_DWELL_MINUS
#define SOL_DWI SOLENOID_DWELL_PLUS
#define SOL_BON SOLENOID_BUZZ_ON
#define SOL_BOF SOLENOID_BUZZ_OFF

#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)
#define ADJUST MO(_ADJUST)

#define KC_RESET RESET
#define KC_RST KC_RESET

#define KC_AE RALT(KC_A)
#define KC_OE RALT(KC_O)
#define KC_UE RALT(KC_U)
#define KC_SS RALT(KC_S)


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_BASE] = LAYOUT(
  //+-----------------------------------------------------+                          +-----------------------------------------------------+
     KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                            KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_DEL,
  //+--------+--------+--------+--------+--------+--------¦                          +--------+--------+--------+--------+--------+--------¦
     KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                            KC_Z,    KC_U,    KC_I,    KC_O,    KC_P, KC_BSPC,
  //+--------+--------+--------+--------+--------+--------¦                          +--------+--------+--------+--------+--------+--------¦
     KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                            KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
  //+--------+--------+--------+--------+--------+--------+--------+        +--------+--------+--------+--------+--------+--------+--------¦
     KC_LSFT, KC_Y,    KC_X,    KC_C,    KC_V,    KC_B, KC_HOME,          KC_END,  KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLASH, KC_RSFT,
  //+--------------------------------------------------------------+        +--------------------------------------------------------------+
                                    KC_LGUI, LOWER,   KC_ENT,                    KC_SPC,  RAISE,   KC_LALT
                                // +--------------------------+                 +--------------------------+
  ),

  [_LOWER] = LAYOUT(
  //+-----------------------------------------------------+                          +-----------------------------------------------------+
     KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                            KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSPC,
  //+--------+--------+--------+--------+--------+--------¦                          +--------+--------+--------+--------+--------+--------¦
     RESET,   _______, _______, KC_UP,   _______, _______,                            _______, KC_P7,   KC_P8,   KC_P9,   _______, _______,
  //+--------+--------+--------+--------+--------+--------¦                          +--------+--------+--------+--------+--------+--------¦
     KC_DEL,  _______, KC_LEFT, KC_DOWN, KC_RGHT, KC_LBRC,                            KC_RBRC, KC_P4,   KC_P5,   KC_P6,   KC_PLUS, KC_PIPE,
  //+--------+--------+--------+--------+--------+--------+--------+        +--------+--------+--------+--------+--------+--------+--------¦
     BL_STEP, _______, _______, _______, _______, KC_LCBR, KC_LPRN,          KC_RPRN, KC_RCBR, KC_P1,   KC_P2,   KC_P3,   KC_MINS, _______,
  //+--------------------------------------------------------------+        +--------------------------------------------------------------+
                                    _______, _______, KC_DEL,                    KC_DEL,  _______, KC_P0
                                // +--------------------------+                 +--------------------------+
  ),

  [_RAISE] = LAYOUT(
  //+-----------------------------------------------------+                          +-----------------------------------------------------+
      KC_F12, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                            KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
  //+--------+--------+--------+--------+--------+--------¦                          +--------+--------+--------+--------+--------+--------¦
     _______, _______, _______, _______, _______, _______,                            _______, KC_UE,   _______, KC_OE,   KC_MPLY, KC_MNXT,
  //+--------+--------+--------+--------+--------+--------¦                          +--------+--------+-------+--------+--------+--------¦
     _______, KC_AE,   KC_SS,   _______, KC_PGUP, KC_UNDS,                            KC_EQL,  KC_HOME, KC_END,  _______, _______, KC_BSLS,
  //+--------+--------+--------+--------+--------+--------+--------+        +--------+--------+--------+--------+--------+--------+--------¦
     _______, _______, _______, _______, KC_PGDN, KC_MINS, _______,          KC_PLUS, _______, KC_MUTE, KC_VOLD, KC_VOLU, _______, _______,
  //+--------------------------------------------------------------+        +--------------------------------------------------------------+
                                    _______, _______, _______,                  _______,  _______, _______
                                // +--------------------------+                 +--------------------------+
  ),

  [_ADJUST] = LAYOUT(
  //,--------+--------+--------+--------+--------+--------.                          ,--------+--------+--------+--------+--------+--------.
      _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------|                          |--------+--------+--------+--------+--------+--------|
      _______, _______, _______, _______, _______, _______,                            SOL_TOG, SOL_BON, SOL_BOF, SOL_DWD, SOL_DWI, _______,
  //|--------+--------+--------+--------+--------+--------|                          |--------+--------+--------+--------+--------+--------|
      DEBUG,   _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------+--------.        ,--------|--------+--------+--------+--------+--------+--------|
      BL_STEP, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______,
  //`--------+--------+--------+--------+--------+--------+--------/        \--------+--------+--------+--------+--------+--------+--------'
                                      _______, _______, _______,                  _______, _______, _______
  //                                `--------+--------+--------'                `--------+--------+--------'
  )

};

#ifdef AUDIO_ENABLE
float tone_qwerty[][2]     = SONG(QWERTY_SOUND);
#endif

void persistent_default_layer_set(uint16_t default_layer) {
  eeconfig_update_default_layer(default_layer);
  default_layer_set(default_layer);
}

bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {

  if (record->event.pressed) {
    solenoid_fire();
  }

  switch (keycode) {
    case LOWER:
      if (record->event.pressed) {
        layer_on(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case ADJUST:
      if (record->event.pressed) {
        layer_on(_ADJUST);
      } else {
        layer_off(_ADJUST);
      }
      return false;
      break;
    case SOLENOID_TOG:
      if (record->event.pressed) {
        solenoid_toggle();
      }
      break;
    case SOLENOID_DWELL_MINUS:
      if (record->event.pressed) {
        solenoid_dwell_minus();
      }
      break;
    case SOLENOID_DWELL_PLUS:
      if (record->event.pressed) {
        solenoid_dwell_plus();
      }
      break;
    case SOLENOID_BUZZ_ON:
      if (record->event.pressed) {
        solenoid_buzz_on();
      }
      break;
    case SOLENOID_BUZZ_OFF:
      if (record->event.pressed) {
        solenoid_buzz_off();
      }
  }
  return true;
}
