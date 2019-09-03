#include QMK_KEYBOARD_H
#include "pincontrol.h"
#include "manzel.h"

extern keymap_config_t keymap_config;

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
  SOLENOID_TOG = NEW_SAFE_RANGE,
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


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_BASE] = LAYOUT_wrapper(
  //+-----------------------------------------------------+                          +-----------------------------------------------------+
     KC_ESC,  ________________NUMBER_LEFT________________,                            ________________NUMBER_RIGHT_______________, KC_DEL,
  //+--------+--------+--------+--------+--------+--------¦                          +--------+--------+--------+--------+--------+--------¦
     KC_TAB,  _________________QWERTY_L1_________________,                            _________________QWERTY_R1_________________, KC_BSPC,
  //+--------+--------+--------+--------+--------+--------¦                          +--------+--------+--------+--------+--------+--------¦
     KC_LCTL, _________________QWERTY_L2_________________,                            _________________QWERTY_R2_________________, KC_QUOT,
  //+--------+--------+--------+--------+--------+--------+--------+        +--------+--------+--------+--------+--------+--------+--------¦
     KC_LSFT, _________________QWERTY_L3_________________, KC_HOME,          KC_END,  _________________QWERTY_R3_________________, KC_RSFT,
  //+--------------------------------------------------------------+        +--------------------------------------------------------------+
                                    KC_LGUI, LOWER,   KC_ENT,                    KC_SPC,  RAISE,   KC_LALT
                                // +--------------------------+                 +--------------------------+
  ),

  [_LOWER] = LAYOUT_wrapper(
  //+-----------------------------------------------------+                          +-----------------------------------------------------+
     KC_TILD, _________________LOWER_L0__________________,                            _________________LOWER_R0__________________, KC_BSPC,
  //+--------+--------+--------+--------+--------+--------¦                          +--------+--------+--------+--------+--------+--------¦
     RESET,   _________________LOWER_L1__________________,                            _________________LOWER_R1__________________, _______,
  //+--------+--------+--------+--------+--------+--------¦                          +--------+--------+--------+--------+--------+--------¦
     KC_DEL,  _________________LOWER_L2__________________,                            _________________LOWER_R2__________________, KC_PIPE,
  //+--------+--------+--------+--------+--------+--------+--------+        +--------+--------+--------+--------+--------+--------+--------¦
     BL_STEP, _________________LOWER_L3__________________, KC_LPRN,          KC_RPRN, _________________LOWER_R3__________________, _______,
  //+--------------------------------------------------------------+        +--------------------------------------------------------------+
                                    _______, _______, KC_DEL,                    KC_DEL,  _______, KC_P0
                                // +--------------------------+                 +--------------------------+
  ),

  [_RAISE] = LAYOUT_wrapper(
  //+-----------------------------------------------------+                          +-----------------------------------------------------+
      KC_F12, _________________RAISE_L0__________________,                            _________________RAISE_R0__________________,  KC_F11,
  //+--------+--------+--------+--------+--------+--------¦                          +--------+--------+--------+--------+--------+--------¦
     _______, _________________RAISE_L1__________________,                            _________________RAISE_R1__________________, KC_MNXT,
  //+--------+--------+--------+--------+--------+--------¦                          +--------+--------+-------+--------+--------+--------¦
     _______, _________________RAISE_L2__________________,                            _________________RAISE_R2__________________, KC_BSLS,
  //+--------+--------+--------+--------+--------+--------+--------+        +--------+--------+--------+--------+--------+--------+--------¦
     _______, _________________RAISE_L3__________________, _______,          KC_PLUS, _________________RAISE_R3__________________, _______,
  //+--------------------------------------------------------------+        +--------------------------------------------------------------+
                                    _______, _______, _______,                  _______,  _______, _______
                                // +--------------------------+                 +--------------------------+
  ),

  [_ADJUST] = LAYOUT_wrapper(
  //,--------+--------+--------+--------+--------+--------.                          ,--------+--------+--------+--------+--------+--------.
      _______, _________________ADJUST_L0_________________,                            _________________ADJUST_R0_________________, _______,
  //|--------+--------+--------+--------+--------+--------|                          |--------+--------+--------+--------+--------+--------|
      _______, _________________ADJUST_L1_________________,                            SOL_TOG, SOL_BON, SOL_BOF, SOL_DWD, SOL_DWI, _______,
  //|--------+--------+--------+--------+--------+--------|                          |--------+--------+--------+--------+--------+--------|
      DEBUG,   _________________ADJUST_L2_________________,                            _________________ADJUST_R2_________________, _______,
  //|--------+--------+--------+--------+--------+--------+--------.        ,--------|--------+--------+--------+--------+--------+--------|
      BL_STEP, _________________ADJUST_L3_________________, _______,          _______, _________________ADJUST_R3_________________, _______,
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
