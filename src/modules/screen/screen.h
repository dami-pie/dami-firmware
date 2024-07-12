#ifndef _SCREEN_H
#define _SCREEN_H
#define LGFX_AUTODETECT // Autodetect board
#define LGFX_USE_V1     // set to use new version of library

#include "ui/ui.h"
#include "lv_conf.h"
#include <lvgl.h>
#include <LovyanGFX.hpp>

#include <Arduino.h>
#include <WiFi.h>
#include <time.h>

#define NTP_SERVER_1 "a.st1.ntp.br"
#define NTP_SERVER_2 "b.st1.ntp.br"
#define BLACK_COLOR 0x000000
#define WHITE_COLOR 0xFCFBF7
#define GREEN_COLOR 0x9DF393
#define BROWN_COLOR 0xF4B896
#define BLUE_COLOR 0x9393F8
#define RED_COLOR 0xF81212

static LGFX lcd;                   // declare display variable
static const int gmtOffset = (-3); // GMT-03 [Brasilia]
extern QueueHandle_t ui_render_queue;
/*------------------- LVGL CONFIG --------------------*/
// 1.Define screen resolution for LVGL
// static const uint16_t screenWidth = 320, screenHeight = 480;
#define screenWidth 320
#define screenHeight 480

// 2.Define screen buffer for LVGL
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * 10];
static lv_color_t buf2[screenWidth * 10];

/*----------------- DISPLAY WORKERS ------------------*/
// 1.Display callbac+ to flush the buffer to screen
void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *buf);

// 2. Touchpad callback to read the touchpad
// void touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);

void codeUpdate(String code);

void setup_screen(byte brightness = 50);

void show_layout(const char *text, uint32_t color);

void ui_task(void *);

#endif
