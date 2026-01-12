// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.2.0
// LVGL VERSION: 8.2.0
// PROJECT: SquareLine_Project

#ifndef _SQUARELINE_PROJECT_UI_H
#define _SQUARELINE_PROJECT_UI_H

#ifdef __cplusplus
extern "C"
{
#endif

#if defined __has_include
#if __has_include("lvgl.h")
#include "lvgl.h"
#elif __has_include("lvgl/lvgl.h")
#include "lvgl/lvgl.h"
#else
#include "lvgl.h"
#endif
#else
#include "lvgl.h"
#endif

  extern lv_obj_t *ui_LoginScreen;
  extern lv_obj_t *ui_EcompLogo;
  extern lv_obj_t *ui_LoginLabel;
  extern lv_obj_t *ui_QRCodeLogin;
  extern lv_obj_t *ui_WifiPanel;
  extern lv_obj_t *ui_WifiLabel;
  extern lv_obj_t *ui_TimeLabel;
  extern lv_obj_t *ui_DateLabel;

  // LV_IMG_DECLARE(ui_img_dami_png); // assets\Dami.png

  void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
