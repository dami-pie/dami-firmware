#include "default.h"
#include <ansi/ansi_codes.h>

void reboot_command(size_t, char **) { ESP.restart(); }

void free_heap_command(size_t, char **)
{
  auto free = ESP.getFreeHeap();
  auto total = ESP.getHeapSize();
  uint64_t p = (10000 * free) / total;
  Serial.printf("Free Heap: %u from %u (", free, total);
  Serial.print(0.01f * p);
  Serial.println("%) bytes");
}

void print_file_info(fs::File file, String base_path)
{

  // base_path += "/";
  // base_path += file.name();
  // base_path += '\0';
  // base_path.replace("//", "/");

  char time[14];
  time[13] = '\0';

  const time_t _t = file.getLastWrite();
  strftime(time, 13, "%b %d %R", localtime(&_t));

  Serial.printf("-%s %05d %s %s\n",
                file.isDirectory() ? "d" : "-",
                file.available(),
                time,
                file.path());
}

void list_files_command(size_t argc, char **argv)
{
  if (argc < 2)
    return;

  fs::File file = SPIFFS.open(argv[1]);
  if (!file)
    return;

  if (file.isDirectory())
  {
    for (auto child = file.openNextFile(); child; child = file.openNextFile())
      print_file_info(child, argv[1]);
  }
  else
    print_file_info(file, argv[1]);

  file.close();
}

void cat_command(size_t argc, char **argv)
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

void rm_command(size_t argc, char **argv)
{
  if (argc < 2)
    return;

  if (!SPIFFS.exists(argv[1]))
    Serial.printf("Erro: %s does not exist\n", argv[1]);
  else if (!SPIFFS.remove(argv[1]))
    Serial.println("Erro: fail on delete file");
  else
    Serial.println("File excluded");
}
