/*
Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

#include "quantum.h"

// Layer 0: base
// Layer 1: auto mouse layer (AUTO_MOUSE_DEFAULT_LAYER 1) - mouse buttons only
// Layer 2: numbers (left) + F keys (right)      ... hold Enter
// Layer 3: Keyball settings / scroll mode       ... hold Lang2
// Layer 4: symbols + arrows                     ... hold Space / Esc

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_universal(
    KC_Q     , KC_W     , KC_E     , KC_R     , KC_T     ,                            KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     ,
    KC_A     , KC_S     , KC_D     , KC_F     , KC_G     ,                            KC_H     , KC_J     , KC_K     , KC_L     , KC_MINS  ,
    KC_Z     , KC_X     , KC_C     , KC_V     , KC_B     ,                            KC_N     , KC_M     , KC_COMM  , KC_DOT   , KC_SLSH  ,
    KC_LCTL  , KC_LALT  , KC_LGUI  ,LT(3,KC_LNG2),LT(4,KC_SPC),LSFT_T(KC_LNG1), KC_BSPC,LT(2,KC_ENT), KC_NO , KC_NO    , KC_NO    , LT(4,KC_ESC)
  ),

  [1] = LAYOUT_universal(
    _______  , _______  , _______  , _______  , _______  ,                            _______  , _______  , _______  , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______  ,                            SCRL_MO  , KC_BTN1  , KC_BTN3  , KC_BTN2  , _______  ,
    _______  , _______  , _______  , _______  , _______  ,                            _______  , _______  , _______  , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,      _______  , _______  , _______  , _______  , _______  , _______
  ),

  [2] = LAYOUT_universal(
    _______  , KC_7     , KC_8     , KC_9     , _______  ,                            _______  , KC_F7    , KC_F8    , KC_F9    , KC_F10   ,
    _______  , KC_4     , KC_5     , KC_6     , _______  ,                            _______  , KC_F4    , KC_F5    , KC_F6    , KC_F11   ,
    _______  , KC_1     , KC_2     , KC_3     , _______  ,                            _______  , KC_F1    , KC_F2    , KC_F3    , KC_F12   ,
    _______  , KC_0     , _______  , _______  , _______  , KC_BSPC  ,      _______  , _______  , _______  , _______  , _______  , _______
  ),

  [3] = LAYOUT_universal(
    QK_MACRO_1, AML_TO  , AML_I50  , AML_D50  , _______  ,                            _______  , _______  , SSNP_HOR , SSNP_VRT , SSNP_FRE ,
    QK_MACRO_0,QK_MACRO_3,_______  , _______  , SCRL_DVI ,                            _______  , _______  , _______  , _______  , _______  ,
    QK_MACRO_2, _______ , _______  , _______  , SCRL_DVD ,                            CPI_D1K  , CPI_D100 , CPI_I100 , CPI_I1K  , KBC_SAVE ,
    QK_BOOT  , KBC_RST  , _______  , _______  , _______  , _______  ,      _______  , _______  , _______  , _______  , _______  , QK_BOOT
  ),

  [4] = LAYOUT_universal(
    KC_EXLM  , KC_AT    , KC_HASH  , KC_DLR   , KC_PERC  ,                            KC_CIRC  , KC_AMPR  , KC_ASTR  , KC_LPRN  , KC_RPRN  ,
    KC_TAB   , KC_GRV   , KC_BSLS  , KC_LBRC  , KC_RBRC  ,                            KC_LEFT  , KC_DOWN  , KC_UP    , KC_RGHT  , KC_SCLN  ,
    KC_LSFT  , KC_TILD  , KC_PIPE  , KC_LCBR  , KC_RCBR  ,                            KC_QUOT  , KC_DQUO  , KC_EQL   , KC_PLUS  , KC_COLN  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,      _______  , _______  , _______  , _______  , _______  , _______
  ),
};
// clang-format on

layer_state_t layer_state_set_user(layer_state_t state) {
    // Auto enable scroll mode when the highest layer is 3
    keyball_set_scroll_mode(get_highest_layer(state) == 3);
    return state;
}

#ifdef OLED_ENABLE

#    include "lib/oledkit/oledkit.h"

void oledkit_render_info_user(void) {
    keyball_oled_render_keyinfo();
    keyball_oled_render_ballinfo();
    keyball_oled_render_layerinfo();
}
#endif
