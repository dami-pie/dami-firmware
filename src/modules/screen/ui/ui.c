// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.2.0
// LVGL VERSION: 8.2.0
// PROJECT: PT3

#define WHITE_COLOR 0xFCFBF7
#define GREEN_COLOR 0x9DF393
#define YELLOW_COLOR 0xFDDC8B
#define BLACK_COLOR 0x000000

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////
lv_obj_t *ui_LoginScreen;
// lv_obj_t *ui_EcompLogo;
lv_obj_t *ui_LoginLabel;
lv_obj_t *ui_QRCodeLogin;
lv_obj_t *ui_WifiPanel;
lv_obj_t *ui_WifiLabel;
lv_obj_t *ui_TimeLabel;
lv_obj_t *ui_DateLabel;

///////////////////// SCREENS ////////////////////
void ui_LoginScreen_screen_init(void)
{

  // ui_LoginScreen
  ui_LoginScreen = lv_obj_create(NULL);
  lv_obj_clear_flag(ui_LoginScreen, LV_OBJ_FLAG_SCROLLABLE); /// Flags
  lv_obj_set_style_bg_color(ui_LoginScreen, lv_color_hex(WHITE_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_LoginScreen, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

  // ui_EcompLogo
  // ui_EcompLogo = lv_img_create(ui_LoginScreen);
  // lv_img_set_src(ui_EcompLogo, &ui_img_dami_png);
  // lv_obj_set_width(ui_EcompLogo, LV_SIZE_CONTENT);  ///     28
  // lv_obj_set_height(ui_EcompLogo, LV_SIZE_CONTENT); /// 64
  // lv_obj_set_x(ui_EcompLogo, 16);
  // lv_obj_set_y(ui_EcompLogo, 16);
  // lv_obj_add_flag(ui_EcompLogo, LV_OBJ_FLAG_ADV_HITTEST);  /// Flags
  // lv_obj_clear_flag(ui_EcompLogo, LV_OBJ_FLAG_SCROLLABLE); /// Flags

  // ui_LoginLabel
  ui_LoginLabel = lv_label_create(ui_LoginScreen);
  lv_obj_set_width(ui_LoginLabel, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_LoginLabel, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(ui_LoginLabel, -16);
  lv_obj_set_y(ui_LoginLabel, 40);
  lv_obj_set_align(ui_LoginLabel, LV_ALIGN_TOP_RIGHT);
  lv_label_set_text(ui_LoginLabel, "DAMI©");
  lv_obj_set_style_text_font(ui_LoginLabel, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

  // ui_QRCodeLogin
  ui_QRCodeLogin = lv_qrcode_create(ui_LoginScreen, 296, lv_color_hex(BLACK_COLOR), lv_color_hex(WHITE_COLOR));
  lv_obj_align(ui_QRCodeLogin, LV_ALIGN_CENTER, 0, 8);
  lv_obj_clear_flag(ui_QRCodeLogin, LV_OBJ_FLAG_SCROLLABLE);

  // ui_WifiPanel
  ui_WifiPanel = lv_obj_create(ui_LoginScreen);
  lv_obj_set_width(ui_WifiPanel, 168);
  lv_obj_set_height(ui_WifiPanel, 48);
  lv_obj_set_x(ui_WifiPanel, 16);
  lv_obj_set_y(ui_WifiPanel, -16);
  lv_obj_set_align(ui_WifiPanel, LV_ALIGN_BOTTOM_LEFT);
  lv_obj_clear_flag(ui_WifiPanel, LV_OBJ_FLAG_SCROLLABLE); /// Flags
  lv_obj_set_style_radius(ui_WifiPanel, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_WifiPanel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

  // ui_WifiLabel
  ui_WifiLabel = lv_label_create(ui_WifiPanel);
  lv_obj_set_width(ui_WifiLabel, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_WifiLabel, LV_SIZE_CONTENT); /// 1
  lv_obj_set_align(ui_WifiLabel, LV_ALIGN_LEFT_MID);
  lv_obj_set_x(ui_WifiLabel, 4);

  // ui_TimeLabel
  ui_TimeLabel = lv_label_create(ui_LoginScreen);
  lv_obj_set_width(ui_TimeLabel, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_TimeLabel, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(ui_TimeLabel, -16);
  lv_obj_set_y(ui_TimeLabel, -32);
  lv_obj_set_align(ui_TimeLabel, LV_ALIGN_BOTTOM_RIGHT);
  lv_obj_set_style_text_font(ui_TimeLabel, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

  // ui_DateLabel
  ui_DateLabel = lv_label_create(ui_LoginScreen);
  lv_obj_set_width(ui_DateLabel, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_height(ui_DateLabel, LV_SIZE_CONTENT); /// 1
  lv_obj_set_x(ui_DateLabel, -16);
  lv_obj_set_y(ui_DateLabel, -16);
  lv_obj_set_align(ui_DateLabel, LV_ALIGN_BOTTOM_RIGHT);
}

void ui_init(void)
{
  lv_disp_t *dispp = lv_disp_get_default();
  lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                            false, LV_FONT_DEFAULT);
  lv_disp_set_theme(dispp, theme);
  ui_LoginScreen_screen_init();
  lv_disp_load_scr(ui_LoginScreen);
}
