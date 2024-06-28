#include "cmd.h"
#include <SPIFFS.h>

cmd_process_t cmd_process;

bool add_cmd(const char *cmd, cmd_callback callback)
{
  if (PSRamFS.exists("/usr/bin/" + String(cmd)))
  {
    log_e("Fail on add %s, command already in use");
    return false;
  }
  else
  {
    auto file = PSRamFS.open("/usr/bin/" + String(cmd), "w", true);
    if (file)
    {
      file.printf("%d", callback);
      log_d("Command %s Added, func pointer: %x", cmd, callback);
      return true;
    }
  }

  return false;
}

void parse_cmd(const char *line, size_t size)
{
  cmd_process.argc = 0;
  log_v("Counting args...");
  for (size_t i = 0, p = 0; i < size; i++)
  {
    if ((line[i] == ' ' && (i - p) >= 2) || (i - p) > 0 && i == size - 1)
    {
      cmd_process.argc++;
      p = i;
    }
  }
  log_v("%u args for %s", cmd_process.argc, line);

  cmd_process.argv = new char *[cmd_process.argc];

  log_v("Processes argv...");
  for (size_t end = 0, start = 0, c = 0; end < size && c <= cmd_process.argc; end++)
  {
    if ((line[end] == ' ' && (end - start) >= 2) || (end - start) > 0 && end == size - 1)
    {
      size_t size = end - start;
      char *arg = new char[size + 1];
      auto *ptr = line + (line[start] == ' ' ? start + 1 : start);

      arg[size] = '\0';
      cmd_process.argv[c] = (char *)memcpy(arg, ptr, size);

      log_v("Added arg: %s, size: %u bytes, len: %d", cmd_process.argv[c], size, String(cmd_process.argv[c]).length());
      start = end;
      c++;
    }
  }
}

void cmd_loop(String *buffer)
{
  while (cmd_process.running)
  {
    Serial.println();
    Serial.print(CMD_PROMPT_HEAD);
    *buffer = get_input(Serial, '\n', -1);

    if (buffer->startsWith("exit"))
    {
      cmd_process.running = false;
      *buffer = "";
      log_w("Shell task ended!");
      break;
    }

    if (!run_cmd(*buffer))
      log_w("Fail on execute %s", buffer->c_str());
    *buffer = "";
  }
}

cmd_callback get_cmd(String &cmd_name)
{
  auto file = PSRamFS.open("/usr/bin/" + cmd_name);
  cmd_callback command;

  if (!file)
  {
    log_e("Command %s does not exist.", cmd_name.c_str());
    return NULL;
  }

  String data = file.readStringUntil(EOF);
  command = (cmd_callback)data.toInt();
  log_v("Command pointer: %x", command);
  return command;
}

bool run_cmd(String buffer)
{
  auto space = buffer.indexOf(' ');
  String cmd = space > 0 ? buffer.substring(0, space) : buffer;
  auto command = get_cmd(cmd);

  if (command)
  {
    parse_cmd(buffer.c_str(), buffer.length());
    log_v("Running command %s (id: %x)", cmd.c_str(), command);
    command(cmd_process.argc, cmd_process.argv);
    for (size_t i = 0; i < cmd_process.argc; i++)
      if (cmd_process.argv[i] != NULL)
        vPortFree(cmd_process.argv[i]);

    if (cmd_process.argv != NULL)
      vPortFree(cmd_process.argv);
    cmd_process.argc = 0;
    return true;
  }

  return false;
}

void cmd_task(void *str_buffer)
{
  for (;; vTaskDelay(100))
  {
    // wait inset key to be press
    if (Serial.available() >= 2 && Serial.read() == 0x32 && Serial.read() == 0x7e)
    {
      cmd_process.running = true;
      cmd_loop((String *)str_buffer);
    }
  }
  vTaskDelete(NULL);
}

void cmd_start(String *buffer)
{
  PSRamFS.setPartitionSize(2048);
  if (!PSRamFS.begin())
  {
    log_e("Fail on start virtual file system");
    return;
  }

  add_cmd("free-heap", free_heap_cmd);

  add_cmd("reboot", reboot_cmd);

  add_cmd("cat", cat_cmd);
  add_cmd("ls", ls_cmd);

  cmd_process.running = pdPASS == xTaskCreate(cmd_task, "cmd_task_runner", cmd_process.stack_deep, buffer, 2, &cmd_process.handle);
}

void cmd_input(String &buffer, char end)
{
  for (;;)
  {
    if (Serial.available())
    {
      int ch = Serial.read();
      Serial.write(ch);
      auto buffer_len = buffer.length();
      if (ch == 0x08 && buffer_len)
      {
        Serial.write(' ');
        Serial.write(0x08);
        buffer.remove(buffer_len - 1);
      }
      else if (ch == '\n')
      {
        buffer.remove(buffer_len - 1);
        return;
      }
      else
        buffer += (char)ch;
    }
    else
      vTaskDelay(1);
  }
}

String get_input(HardwareSerial &console, char end_line, int timeout)
{
  String line;
  uint32_t start = millis(), sum;
  size_t position, size;
  auto RIGHT_COD = 0;

  for (; start + timeout < millis(); vTaskDelay(10))
  {
    int available = console.available();
    if (console.available())
    {
      char c = console.read(); // process_char(console, line, position);
      if (c == ESC_COD)
        return "exit";
      else if (c == '\n')
      {
        console.write('\n');
        return line;
      }
      else if (c == BACKSPACE_COD && line.length() > 0)
      {
        console.write(BACKSPACE_COD);
        console.write(' ');
        console.write(BACKSPACE_COD);
        line = line.substring(0, line.length() - 1);
        position--;
      }
      else if (c >= 32 && c <= 128)
      {
        console.write((char)c);
        if (position > 0 && position < line.length() - 1)
        {
          auto sub = line.substring(position);
          line = line.substring(0, position) + c + sub;
          console.write(sub.c_str());
        }
        else
          line += c;
        position++;
      }
    }
  }
  return line;
}
