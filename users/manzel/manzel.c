#include "manzel.h"
#include "rgb_stuff.h"

userspace_config_t userspace_config;

// Add reconfigurable functions here, for keymap customization
// This allows for a global, userspace functions, and continued
// customization of the keymap.  Use _keymap instead of _user
// functions in the keymaps
__attribute__ ((weak))
void matrix_init_keymap(void) {}

__attribute__ ((weak))
void startup_keymap(void) {}

__attribute__ ((weak))
void shutdown_keymap(void) {}

__attribute__ ((weak))
void suspend_power_down_keymap(void) {}

__attribute__ ((weak))
void suspend_wakeup_init_keymap(void) {}

__attribute__ ((weak))
void matrix_scan_keymap(void) {}

__attribute__ ((weak))
bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {
  return true;
}

__attribute__ ((weak))
bool process_record_secrets(uint16_t keycode, keyrecord_t *record) {
  return true;
}


__attribute__ ((weak))
uint32_t layer_state_set_keymap (uint32_t state) {
  return state;
}

__attribute__ ((weak))
uint32_t default_layer_state_set_keymap (uint32_t state) {
  return state;
}

__attribute__ ((weak))
void led_set_keymap(uint8_t usb_led) {}



// Call user matrix init, set default RGB colors and then
// call the keymap's init function
void matrix_init_user(void) {

// #ifdef BOOTLOADER_CATERINA
//   DDRD &= ~(1<<5);
//   PORTD &= ~(1<<5);

//   DDRB &= ~(1<<0);
//   PORTB &= ~(1<<0);
// #endif


#if (defined(UNICODE_ENABLE) || defined(UNICODEMAP_ENABLE) || defined(UCIS_ENABLE))
  set_unicode_input_mode(UC_WINC);
#endif //UNICODE_ENABLE
  matrix_init_keymap();
}

void startup_user (void) {
  #ifdef RGBLIGHT_ENABLE
    matrix_init_rgb();
  #endif //RGBLIGHT_ENABLE
  startup_keymap();
}

void shutdown_user (void) {
#ifdef RGBLIGHT_ENABLE
  rgblight_enable_noeeprom();
  rgblight_mode_noeeprom(1);
  rgblight_setrgb_red();
#endif // RGBLIGHT_ENABLE
#ifdef RGB_MATRIX_ENABLE
  rgb_led led;
  for (int i = 0; i < DRIVER_LED_TOTAL; i++) {
    led = g_rgb_leds[i];
    if (led.matrix_co.raw < 0xFF) {
      rgb_matrix_set_color( i, 0xFF, 0x00, 0x00 );
    }
  }
#endif //RGB_MATRIX_ENABLE
  shutdown_keymap();
}

void suspend_power_down_user(void)
{
    suspend_power_down_keymap();
}

void suspend_wakeup_init_user(void)
{
  suspend_wakeup_init_keymap();
}


// No global matrix scan code, so just run keymap's matrix
// scan function
void matrix_scan_user(void) {
  static bool has_ran_yet;
  if (!has_ran_yet) {
    has_ran_yet = true;
    startup_user();
  }

#ifdef TAP_DANCE_ENABLE  // Run Diablo 3 macro checking code.
  run_diablo_macro_check();
#endif // TAP_DANCE_ENABLE

#ifdef RGBLIGHT_ENABLE
  matrix_scan_rgb();
#endif // RGBLIGHT_ENABLE

  matrix_scan_keymap();
}

// Defines actions tor my global custom keycodes. Defined in manzel.h file
// Then runs the _keymap's record handier if not processed here
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
  case KC_MAKE:
    if (!record->event.pressed) {
      SEND_STRING("make " QMK_KEYBOARD ":" QMK_KEYMAP
#if  (defined(BOOTLOADER_DFU) || defined(BOOTLOADER_LUFA_DFU) || defined(BOOTLOADER_QMK_DFU))
       ":dfu "
#elif defined(BOOTLOADER_HALFKAY)
      ":teensy "
#elif defined(BOOTLOADER_CATERINA)
       ":avrdude "
#endif
        SS_TAP(X_ENTER));
    }
    return false;
    break;

    case EPRM: // Resets EEPROM
      if (record->event.pressed) {
        eeconfig_init();
        default_layer_set(1UL<<eeconfig_read_default_layer());
        layer_state_set(layer_state);
      }
      return false;
      break;
    case VRSN: // Prints firmware version
      if (record->event.pressed) {
        send_string_with_delay_P(PSTR(QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION ", Built on: " QMK_BUILDDATE), MACRO_TIMER);
      }
      return false;
      break;
#ifdef UNICODE_ENABLE
  case UC_FLIP: // (╯°□°)╯ ︵ ┻━┻
    if (record->event.pressed) {
      register_code(KC_RSFT);
      tap(KC_9);
      unregister_code(KC_RSFT);
      process_unicode((0x256F | QK_UNICODE), record); // Arm
      process_unicode((0x00B0 | QK_UNICODE), record); // Eye
      process_unicode((0x25A1 | QK_UNICODE), record); // Mouth
      process_unicode((0x00B0 | QK_UNICODE), record); // Eye
      register_code(KC_RSFT);
      tap(KC_0);
      unregister_code(KC_RSFT);
      process_unicode((0x256F | QK_UNICODE), record); // Arm
      tap(KC_SPC);
      process_unicode((0x0361 | QK_UNICODE), record); // Flippy
      tap(KC_SPC);
      process_unicode((0x253B | QK_UNICODE), record); // Table
      process_unicode((0x2501 | QK_UNICODE), record); // Table
      process_unicode((0x253B | QK_UNICODE), record); // Table
    }
    return false;
    break;
#endif // UNICODE_ENABLE

  }
  return process_record_keymap(keycode, record) &&
#ifdef RGBLIGHT_ENABLE
    process_record_user_rgb(keycode, record) &&
#endif // RGBLIGHT_ENABLE
    process_record_secrets(keycode, record);
}

// Runs state check and changes underglow color and animation
// on layer change, no matter where the change was initiated
// Then runs keymap's layer change check
uint32_t layer_state_set_user(uint32_t state) {
  state = update_tri_layer_state(state, _RAISE, _LOWER, _ADJUST);
#ifdef RGBLIGHT_ENABLE
  state = layer_state_set_rgb(state);
#endif // RGBLIGHT_ENABLE
  return layer_state_set_keymap (state);
}


uint32_t default_layer_state_set_kb(uint32_t state) {
  return default_layer_state_set_keymap (state);
}


// Any custom LED code goes here.
// So far, I only have keyboard specific code,
// So nothing goes here.
void led_set_user(uint8_t usb_led) {
  led_set_keymap(usb_led);
}