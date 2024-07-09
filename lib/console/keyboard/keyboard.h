#pragma once

#include <Arduino.h>
#include <cbuf.h>
#include "keyboard/key_map.h"
#include "ansi/ansi_codes.h"

class KeyboardBuff : public Stream, cbuf
{
public:
  KeyboardBuff(size_t buffer_size) : cbuf(buffer_size){};

  int available() override { return cbuf::available(); }
  int read() override { return cbuf::read(); }
  int peek() override { return cbuf::peek(); }

  size_t write(char c)
  {
    if (c != KEYBOARD_BACKSPACE)
      cbuf::write(c);
    else if (cbuf::available())
      cbuf::_end--;
  }

  size_t write(const char *src, size_t size) { cbuf::write(src, size); }
};

void handle_keyboard_input(Stream &console, KeyboardBuff &buffer, int end = KEYBOARD_ENTER)
{
  for (;; vTaskDelay(15))
  {
    if (!console.available())
      continue;
    int key = console.read();
    if (key == KEYBOARD_ESC && console.available())
    {
      int key_sequence[] = {-1, -1};
      key_sequence[0] = console.read();
      if (console.available())
        key_sequence[1] = console.read();
    }
    else if (key == end)
      return;
    else if (key == KEYBOARD_CTRL_L)
      console.write(ansi_clear_screen);
    else if (key == KEYBOARD_CTRL_C)
    {
      buffer.write("exit", 5);
      return;
    }
    else if (key == KEYBOARD_BACKSPACE)
    {

      buffer.write(key);
      console.printf(ansi_cursor_back, 1);
      console.write(' ');
      console.write(key);
    }
    else if (key >= 32 && key <= 128)
    {
      buffer.write(key);
      console.write(key);
    }
  }
}
