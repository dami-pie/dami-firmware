#pragma once

#include <Arduino.h>
#include <cbuf.h>
#include "keyboard/map/key_map.h"
#include "ansi/ansi_codes.h"

class KeyboardBuff : public cbuf
{
  char *cursor;

public:
  KeyboardBuff(size_t buffer_size) : cbuf(buffer_size), cursor(_end){};

  void reset_cursor() { cursor = _end; }

  int cursor_shift(int amount)
  {
    auto new_c = cursor + amount;
    if (new_c > _begin && new_c < _end)
      cursor = new_c;
    else
      return 0;
    return amount;
  }

  char *set(size_t index, char c)
  {
    if (index >= cbuf::_size)
      return NULL;

    cbuf::_buf[index] = c;
    return cbuf::_buf + index;
  }

  size_t write(char c)
  {
    size_t total_written = 0;

    if (c != KEYBOARD_BACKSPACE)
    {
      if (_end == cursor)
      {
        total_written = cbuf::write(c);
        cursor = _end;
      }
      else
      {
        *cursor = c;
        total_written = cursor_shift(1);
      }
    }
    else if (cbuf::available() >= 1)
    {
      cbuf::_end--;
      cursor = _end;
    }

    return total_written;
  }

  size_t write(const char *src, size_t size)
  {
    size_t total_written = 0;

    total_written = cbuf::write(src, size);
    cursor += total_written;
    return total_written;
  }

  size_t pipe(Stream &source, int limit = -1)
  {
    if (limit < 0)
    {
      auto _d = source.readString();
      source.write(_d.c_str(), _d.length());
      return write(_d.c_str(), _d.length());
    }

    size_t size = source.available() > limit ? limit : source.available();
    char _buf[size];
    source.readBytes(_buf, size);
    source.write(_buf, size);
    return write(_buf, size);
  }
};

void handle_keyboard_input(Stream &console, KeyboardBuff &buffer, int end = KEYBOARD_ENTER);