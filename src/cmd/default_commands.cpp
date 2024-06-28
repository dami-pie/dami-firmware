#include "default_commands.h"

void reboot_cmd(size_t, char **) { ESP.restart(); }

void free_heap_cmd(size_t, char **)
{
  auto free = ESP.getFreeHeap();
  auto total = ESP.getHeapSize();
  uint64_t p = (10000 * free) / total;
  Serial.printf("Free Heap: %u from %u (", free, total);
  Serial.print(0.01f * p);
  Serial.println("%) bytes");
}

file_info get_info(fs::File file)
{
  file_info r;
  if (file.isDirectory())
    for (auto child = file.openNextFile(); child; child = file.openNextFile())
    {
      auto b = get_info(child);
      r.size += b.size;
      r.childs++;
    }
  else
  {
    r.size = file.available();
    r.childs = 1;
  }
  return r;
}

void ls_cmd(size_t argc, char **argv)
{
  if (argc < 2)
    return;

  fs::File file = SPIFFS.open(argv[1]);
  if (!file)
    return;

  if (file.isDirectory())
    for (auto child = file.openNextFile(); child; child = file.openNextFile())
    {
      auto info = get_info(child);
      char time[14];
      time[13] = '\0';
      const time_t _t = child.getLastWrite();
      strftime(time, 13, "%b %d %R", localtime(&_t));
      Serial.printf("[%s] %u %0000u %s %s\n",
                    info.childs != 1 ? "D" : "F",
                    info.childs,
                    info.size,
                    time,
                    child.name());
    }
  else
  {
    auto info = get_info(file);
    char time[14];
    time[13] = '\0';
    const time_t _t = file.getLastWrite();
    strftime(time, 13, "%b %d %R", localtime(&_t));
    Serial.printf("[%s] %u %0000u %s %s",
                  info.childs != 1 ? "D" : "F",
                  info.childs,
                  info.size,
                  time,
                  file.name());
  }

  file.close();
}

void cat_cmd(size_t argc, char **argv)
{
  if (argc < 2)
    return;

  auto file = SPIFFS.open(argv[1]);
  if (!file)
    return;
  while (file.available())
    Serial.write(file.read());

  file.close();
}