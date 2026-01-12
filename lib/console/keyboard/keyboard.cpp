#include "keyboard.h"

void handle_keyboard_input(Stream &console, KeyboardBuff &buffer, int end)
{
  for (;; vTaskDelay(15))
  {
    if (!console.available())
      continue;

    int key = console.read();
    if (key >= 32 && key <= 126 && buffer.write(key))
      console.write(key);

    else if (key == KEYBOARD_ESC)
    {
      if (console.read() == '[')
      {
        key = console.read();

        if (key == 'D' && buffer.cursor_shift(-1) != 0)
          console.printf(ansi_cursor_back, 1);
        else if (key == 'C' && buffer.cursor_shift(1) != 0)
          console.printf(ansi_cursor_forward, 1);
      }
    }
    else if (key == KEYBOARD_CTRL_L)
    {
      // console.write(ansi_clear_screen);
      buffer.flush();
      buffer.reset_cursor();
      buffer.write("clear", 6);
    }
    else if (key == end)
    {
      console.write(key);
      buffer.reset_cursor();
      return;
    }
    else if (key == KEYBOARD_CTRL_C)
    {
      buffer.flush();
      buffer.reset_cursor();
      buffer.write("exit", 5);
      return;
    }
    else if ((key == KEYBOARD_BACKSPACE || key == KEYBOARD_CTRL_BACKSPACE) && buffer.available())
    {
      buffer.write(KEYBOARD_BACKSPACE);
      console.printf(ansi_cursor_back, 1);
      console.write(' ');
      console.write(KEYBOARD_BACKSPACE);
    }
  }
}
