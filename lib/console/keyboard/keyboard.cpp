#include "keyboard.h"

void handle_keyboard_input(Stream &console, KeyboardBuff &buffer, int end)
{
  for (;; vTaskDelay(15))
  {
    if (!console.available())
      continue;

    int key = console.read();
    if (key >= 32 && key <= 128)
    {
      buffer.write(key);
      console.write(key);
    }
    else if (key == KEYBOARD_ESC)
    {
      // int key_sequence[] = {-1, -1};
      // key_sequence[0] = console.read();
      // if (console.available())
      //   key_sequence[1] = console.read();
    }
    else if (key == KEYBOARD_CTRL_L)
      console.write(ansi_clear_screen);
    else if (key == end)
    {
      // console.write(';');
      return;
    }
    else if (key == KEYBOARD_CTRL_C)
    {
      buffer.write("exit", 5);
      return;
    }
    else if (key == KEYBOARD_BACKSPACE)
    {

      buffer.write(key);
      console.write(key);
      console.write(' ');
      console.write(key);
    }
  }
}
