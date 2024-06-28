#if !defined(__default_commands_h__)
#define __default_commands_h__

#include <Arduino.h>
#include <SPIFFS.h>

void reboot_cmd(size_t, char **);

void free_heap_cmd(size_t, char **);

typedef struct
{
  size_t size;
  size_t childs;
} file_info;

file_info get_info(fs::File file);
void ls_cmd(size_t argc, char **argv);

void cat_cmd(size_t argc, char **argv);

#endif // __default_commands_h__
