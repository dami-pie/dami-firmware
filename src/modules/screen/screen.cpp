#include "screen.h"

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

void getUpdate(std::tuple<String, String> *_data)
{
  auto data = timeUpdate();
  lv_label_set_text(ui_TimeLabel, std::get<0>(data).c_str());
  lv_label_set_text(ui_DateLabel, std::get<1>(data).c_str());
  if (_data != nullptr)
    *_data = data;
}

void getUpdate()
{
  getUpdate(nullptr);
}

void setup_screen()
{
  log_i("Turning on LCD display");
  File config_file = load_file("/configs/ui/brightness");
  byte brightness = static_cast<byte>(
      config_file.readStringUntil(EOF).toInt());
  log_i("Screen brightness set to %u", brightness);

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