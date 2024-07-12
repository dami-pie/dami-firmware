#if !defined(__default_commands_h__)
#define __default_commands_h__

#include <Arduino.h>
#include <SPIFFS.h>
#include <ESP32Ping.h>
// #include "lwip/inet.h"
// #include "lwip/netdb.h"
// #include "ping/ping_sock.h"

void reboot_command(size_t, char **);

void free_heap_command(size_t, char **);

typedef struct
{
  size_t size;
  size_t childs;
  const char *path;
} file_info;

void list_files_command(size_t argc, char **argv);

void cat_command(size_t argc, char **argv);

void rm_command(size_t argc, char **argv);

void exit_command(size_t argc, char **argv);

void clear_command(size_t argc, char **argv);

void ping_command(size_t, char **);
#endif // __default_commands_h__
