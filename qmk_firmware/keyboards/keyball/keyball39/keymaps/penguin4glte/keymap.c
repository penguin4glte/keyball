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

// 独自の自動マウスレイヤー(AML)で使うレイヤー
#define MOUSE_LAYER  4
#define SCROLL_LAYER 3

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // keymap for default (VIA)
  [0] = LAYOUT_universal(
    KC_Q     , KC_W     , KC_E     , KC_R     , KC_T     ,                            KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     ,
    KC_A     , KC_S     , KC_D     , KC_F     , KC_G     ,                            KC_H     , KC_J     , KC_K     , KC_L     , KC_SCLN  ,
    KC_Z     , KC_X     , KC_C     , KC_V     , KC_B     ,                            KC_N     , KC_M     , KC_COMM  , KC_DOT   , KC_SLSH  ,
    KC_LCTL  , KC_LALT  , KC_LGUI  ,LT(3,KC_LNG2),LT(1,KC_SPC),LSFT_T(KC_LNG1),KC_BSPC,LT(2,KC_ENT),LSFT_T(KC_LNG2),KC_RGUI,KC_RALT, KC_ESC
  ),

  // MacはKeyballをUS配列として認識するので、記号はUS配列のキーコードで書く
  [1] = LAYOUT_universal(
    KC_EXLM  , KC_AT    , KC_HASH  , KC_DLR   , KC_PERC  ,                            KC_CIRC  , KC_AMPR  , KC_ASTR  , KC_LPRN  , KC_RPRN  ,
    KC_GRV   , KC_TILD  , KC_LT    , KC_GT    , KC_QUES  ,                            KC_LEFT  , KC_DOWN  , KC_UP    , KC_RGHT  , KC_QUOT  ,
    KC_UNDS  , KC_PLUS  , KC_LCBR  , KC_RCBR  , KC_PIPE  ,                            KC_MINS  , KC_EQL   , KC_LBRC  , KC_RBRC  , KC_BSLS  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,      _______  , _______  , _______  , _______  , _______  , _______
  ),

  [2] = LAYOUT_universal(
    KC_TAB   , KC_7     , KC_8     , KC_9     , KC_MINS  ,                            _______  , _______  , _______  , _______  , KC_BSPC  ,
    KC_ASTR  , KC_4     , KC_5     , KC_6     , KC_PLUS  ,                            _______  , KC_HOME  , KC_UP    , KC_END   , _______  ,
    KC_SLSH  , KC_1     , KC_2     , KC_3     , KC_EQL   ,                            _______  , KC_LEFT  , KC_DOWN  , KC_RGHT  , _______  ,
    KC_ESC   , KC_0     , KC_DOT   , KC_DEL   , KC_ENT   , KC_BSPC  ,      _______  , _______  , _______  , _______  , _______  , _______
  ),

  [3] = LAYOUT_universal(
    KC_F1    , KC_F2    , KC_F3    , KC_F4    , AML_TO   ,                            _______  , _______  , SSNP_HOR , SSNP_VRT , SSNP_FRE ,
    KC_F5    , KC_F6    , KC_F7    , KC_F8    , SCRL_DVI ,                            RGB_TOG  , RGB_MOD  , RGB_HUI  , RGB_SAI  , RGB_VAI  ,
    KC_F9    , KC_F10   , KC_F11   , KC_F12   , SCRL_DVD ,                            CPI_D1K  , CPI_D100 , CPI_I100 , CPI_I1K  , KBC_SAVE ,
    QK_BOOT  , KBC_RST  , _______  , _______  , _______  , _______  ,      _______  , _______  , _______  , _______  , KBC_RST  , QK_BOOT
  ),

  // マウス専用レイヤー(ボールを転がすと自動で入る)
  // ここに置いたキーはマウス操作扱いでレイヤーに留まる。
  // _______ のキーは下(レイヤー0)のキーとして動き、押すとマウスレイヤーを抜ける。
  [4] = LAYOUT_universal(
    _______  , _______  , _______  , _______  , _______  ,                            _______  , _______  , _______  , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______  ,                            _______  , KC_MS_BTN1  , KC_PGUP  , KC_MS_BTN2  , _______  ,
    _______  , _______  , _______  , _______  , _______  ,                            _______  , _______  , KC_PGDN  , KC_MS_BTN3  , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,      _______  , _______  , _______  , _______  , _______  , _______
  ),
};
// clang-format on

//////////////////////////////////////////////////////////////////////////////
// 独自の自動マウスレイヤー(AML)
//
// 参考: negokaz/keyball PR#2「マウスキーを押すまで Auto Mouse Layer に留まり誤爆を減らす」
//       魚住惇「Keyball39の自動マウスレイヤーで、如何に誤爆を少なくするか」
//
// 1. レイヤー0でボールを転がすとマウスレイヤー(4)に入る。時間経過でもクリックでも抜けない。
//    (クリック後に時間で抜ける方式だと、止まってから再クリックしたときにレイヤー0の j が入力される)
// 2. マウスレイヤーで _______ (=レイヤー0のキー) を押したら即座に抜け、そのキーとして働く。
//    - 左親指 LNG2 タップ → IMEオフでレイヤー0 / LNG1 タップ → IMEオンでレイヤー0
//    - Space タップ → スペースを打ってレイヤー0
//    - LT(1)/LT(2) ホールド → そのレイヤーに入り、離すとレイヤー0に戻る(AMLには戻らない)
// 3. 次の場合はマウスレイヤーに留まる(マウス操作の補助なので)。
//    - Ctrl/Cmd/Alt などの修飾キー、Shift(LSFT_T)のホールド … Cmd+クリックなど
//    - LT(3) のホールド … スクロールモード
//////////////////////////////////////////////////////////////////////////////

static bool aml_enabled = true;
static bool aml_active  = false;

static void aml_enter(void) {
    aml_active = true;
    layer_on(MOUSE_LAYER);
}

static void aml_exit(void) {
    aml_active = false;
    layer_off(MOUSE_LAYER);
}

// マウスレイヤーに留まったまま使ってよいキーか
static bool aml_keeps_layer(uint16_t keycode, keyrecord_t *record) {
    // マウスレイヤーに明示的に置いたキー(マウスボタン、PgUp/PgDnなど)
    if (layer_switch_get_layer(record->event.key) == MOUSE_LAYER) {
        return true;
    }
    // 単体の修飾キー(Ctrl/Cmd/Alt/Shift)
    if (IS_MODIFIER_KEYCODE(keycode)) {
        return true;
    }
    // Mod-Tap のホールド(Shift+クリックなど)
    if (IS_QK_MOD_TAP(keycode) && record->tap.count == 0) {
        return true;
    }
    // スクロール用レイヤーへの Layer-Tap ホールド
    if (IS_QK_LAYER_TAP(keycode) && record->tap.count == 0 && QK_LAYER_TAP_GET_LAYER(keycode) == SCROLL_LAYER) {
        return true;
    }
    // スクロール/設定系のキーボード固有キー
    if (keycode >= QK_KB_0 && keycode <= QK_KB_MAX) {
        return true;
    }
    return false;
}

report_mouse_t pointing_device_task_user(report_mouse_t report) {
    if ((report.x != 0 || report.y != 0) && !aml_active && aml_enabled && get_highest_layer(layer_state) == 0 && !keyball_get_scroll_mode()) {
        aml_enter();
    }
    return report;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // QMK標準AMLの代わりに独自AMLを操作する
    if (keycode == AML_TO) {
        if (record->event.pressed) {
            aml_enabled = !aml_enabled;
            if (!aml_enabled && aml_active) {
                aml_exit();
            }
        }
        return false;
    }

    if (aml_active && record->event.pressed && !aml_keeps_layer(keycode, record)) {
        // キーボードとして使い始めたので抜ける。キー自体はレイヤー0のキーとして処理される。
        aml_exit();
    }
    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    // Auto enable scroll mode when layer 3 is on
    // (マウスレイヤーと同時にONになるので highest ではなく ON/OFF で判定)
    keyball_set_scroll_mode(layer_state_cmp(state, SCROLL_LAYER));
    if (!layer_state_cmp(state, MOUSE_LAYER)) {
        aml_active = false;
    }
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
