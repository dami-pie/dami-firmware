#pragma once

#include <Arduino.h>
#include <cbuf.h>
#include "keyboard/map/key_map.h"
#include "ansi/ansi_codes.h"

class KeyboardBuff : public cbuf
{
public:
  KeyboardBuff(size_t buffer_size) : cbuf(buffer_size){};

  size_t write(char c)
  {
    if (c != KEYBOARD_BACKSPACE)
      return cbuf::write(c);
    else if (cbuf::available() >= 1)
      cbuf::_end--;

    return 1;
  }

  size_t write(const char *src, size_t size) { cbuf::write(src, size); }
};

void handle_keyboard_input(Stream &console, KeyboardBuff &buffer, int end = KEYBOARD_ENTER);