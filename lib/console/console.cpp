#include "console.h"
#include <SPIFFS.h>
#include <linked_list.h>

console_process_t cmd_process;

bool add_command(const char *cmd, console_command_t callback)
{
  String base_path = "/";
  base_path += cmd;
  if (PSRamFS.exists(base_path))
  {
    log_e("Fail on add %s, command already in use");
    return false;
  }
  else
  {
    auto file = PSRamFS.open(base_path, "w", true);
    if (file)
    {
      file.printf("%u", callback);
      log_d("Command %s Added, func pointer: %x", cmd, callback);
      return true;
    }
  }

  return false;
}

void add_argument(char **argp, String arg)
{
  arg += '\0';
  *argp = (char *)memcpy(new char[arg.length()], arg.c_str(), arg.length());
  log_v("Arg: \"%s\" added", *argp);
}

void parse_arguments(String command, console_process_t *process)
{

  struct arg_point
  {
    int start, end;
  };

  auto buff = command.c_str();
  auto len = command.length();

  process->argc = 0;
  log_v("Parsing args...");
  bool quote = false;
  List<arg_point> points;

  for (size_t i = 0; i < 2; i++)
  {

    for (int start = 0, end = 0, c = 0; start < len && end < len;)
    {
      if (!quote)
        quote = buff[start] == '"';

      auto key_char = quote ? '"' : ' ';

      if ((buff[end] == key_char && end - start > 0) || ++end >= len)
      {
        if (i)
        {

          auto arg = command.substring(
              buff[start] == key_char ? start + 1 : start,
              quote ? end - 1 : end);

          add_argument(&process->argv[c++], arg);
        }
        else
          process->argc++;
        quote = false;
        start = end + 1;
      }
    }

    if (i == 0)
    {
      process->argv = new char *[process->argc];
      log_v("%u args", process->argc);
    }
  }

  log_v("%u args for %s", process->argc, command.c_str());
}

void console_loop(String *buffer)
{
  while (cmd_process.running)
  {
    Serial.println();
    Serial.print(CONSOLE_PROMPT);
    *buffer = get_console_input(Serial, '\n', -1);

    if (buffer->startsWith("exit"))
    {
      cmd_process.running = false;
      *buffer = "";
      log_w("Shell task ended!");
      break;
    }

    if (!run_command(*buffer))
      log_w("Fail on execute %s", buffer->c_str());
    *buffer = "";
  }
}

console_command_t get_command(console_process_t &process)
{
  if (process.argc <= 0 || process.argv == NULL || process.argv[0] == NULL)
    return NULL;

  String base_path = "/";
  auto file = PSRamFS.open(base_path + process.argv[0]);
  console_command_t command;

  if (!file)
  {
    log_e("Command %s does not exist.", process.argv[0]);
    return NULL;
  }

  String data = file.readStringUntil(EOF);
  if (data.isEmpty())
  {
    log_e("Bad read, fail on run command");
    return NULL;
  }

  command = (console_command_t)data.toInt();
  log_v("Command pointer: %x", command);
  return command;
}

bool run_command(String buffer)
{

  while (buffer.indexOf("  ") > 0)
    buffer.replace("  ", " ");

  if (buffer.isEmpty())
    return true;
  else
    parse_arguments(buffer, &cmd_process);

  auto command = get_command(cmd_process);
  if (command)
  {
    log_v("Running command %s (id: %x)", cmd_process.argv[0], command);
    command(cmd_process.argc, cmd_process.argv);

    if (cmd_process.argv != NULL)
    {
      for (size_t i = 0; i < cmd_process.argc; i++)
        if (cmd_process.argv[i] != NULL)
          vPortFree(cmd_process.argv[i]);
      vPortFree(cmd_process.argv);
    }

    return true;
  }

  return false;
}

void console_task(void *str_buffer)
{
  for (;; vTaskDelay(100))
  {
    // wait inset key to be press
    if (Serial.available() >= 2 && Serial.read() == 0x32 && Serial.read() == 0x7e)
    {
      cmd_process.running = true;
      console_loop((String *)str_buffer);
    }
  }
  vTaskDelete(NULL);
}

void start_console(String *buffer)
{
  if (!PSRamFS.begin(false, VFS_BASE_PATH, 10U, VFS_PARTITION))
  {
    log_e("Fail on start virtual file system");
    return;
  }

  add_command("free-heap", free_heap_command);
  add_command("reboot", reboot_command);
  add_command("cat", cat_command);
  add_command("rm", rm_command);
  add_command("ls", list_files_command);

  cmd_process.running = pdPASS == xTaskCreate(console_task, "cmd_task_runner", cmd_process.stack_deep, buffer, 2, &cmd_process.handle);
}

String get_console_input(HardwareSerial &console, char end_line, int timeout)
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
