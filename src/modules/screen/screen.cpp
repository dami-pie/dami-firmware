#include "screen.h"
#include "modules/ntp/ntp.h"
extern tm timeInfo;

/*----------------- DISPLAY WORKERS ------------------*/
// 1.Display callbac+ to flush the buffer to screen
void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *buf)
{
  uint32_t width = (area->x2 - area->x1 + 1), height = (area->y2 - area->y1 + 1);

  lcd.startWrite();
  lcd.setAddrWindow(area->x1, area->y1, width, height);
  lcd.writePixels((lgfx::rgb565_t *)&buf->full, width * height);
  lcd.endWrite();

  lv_disp_flush_ready(disp);
}

void codeUpdate(String code)
{
  lv_qrcode_update(ui_QRCodeLogin, code.c_str(), code.length());
}

// void getUpdate(std::tuple<String, String> *_data)
// {
//   auto data = timeUpdate();
//   lv_label_set_text(ui_TimeLabel, std::get<0>(data).c_str());
//   lv_label_set_text(ui_DateLabel, std::get<1>(data).c_str());
//   if (_data != nullptr)
//     *_data = data;
// }

void setup_screen(byte brightness)
{
  /*------------------- LCD CONFIG --------------------/
   1. Initialize LovyanGFX
   2. Setting display Orientation and Brightness
  ----------------------------------------------------*/
  lcd.init();
  lcd.setRotation(lcd.getRotation() ^ (screenWidth > screenHeight ? 1 : 0));
  lcd.setBrightness(brightness);

  /*------------------- LVGL CONFIG --------------------/
   1. Initialize LVGL
   2. LVGL : Setting up buffer to use for display
   3. LVGL : Setup & Initialize the display device driver
   4. Change the following line to your display resolution
   5. LVGL : Setup & Initialize the input device driver
   ----------------------------------------------------*/
  lv_init();
  lv_disp_draw_buf_init(&draw_buf, buf, buf2, screenWidth * 10);

  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.draw_buf = &draw_buf,
  disp_drv.flush_cb = display_flush,
  disp_drv.hor_res = screenWidth,
  disp_drv.ver_res = screenHeight;
  lv_disp_drv_register(&disp_drv);

  ui_init();
}

void show_layout(const char *text, uint32_t color)
{
  lv_label_set_text(ui_WifiLabel, text);
  lv_obj_set_style_bg_color(ui_WifiPanel, lv_color_hex(color), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_color(ui_WifiPanel, lv_color_hex(color), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_clear_flag(ui_TimeLabel, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(ui_DateLabel, LV_OBJ_FLAG_HIDDEN);
}

void ui_task(void *arg)
{
  setup_screen();
  lv_label_set_text(ui_TimeLabel, "--:--");
  lv_label_set_text(ui_DateLabel, "00/00/0000");
  show_layout(LV_SYMBOL_REFRESH "\tConectando", BLUE_COLOR);
  codeUpdate("0000");
  lv_timer_handler();

  char time[6];
  char date[11];
  auto code = (String *)arg;

  for (char _;; xQueueReceive(ui_render_queue, &_, lv_timer_handler() % 1001))
  {
    codeUpdate(*code);
    strftime(time, 6, "%R", &timeInfo);
    strftime(date, 11, "%d/%m/%Y", &timeInfo);
    lv_label_set_text(ui_TimeLabel, time);
    lv_label_set_text(ui_DateLabel, date);
  }

  log_w("ui_task terminated!");
  vTaskDelete(NULL);
}
