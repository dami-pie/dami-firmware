#if !defined(__command_line_h__)
#define __command_line_h__
#include <Arduino.h>
#include <hash.h>
#include <PSRamFS.h>
#include "default/default.h"

#if !defined(CONSOLE_HOSTNAME)
#define CONSOLE_HOSTNAME "dami-pi"
#endif // CONSOLE_HOSTNAME

#if !defined(CONSOLE_SUFFIX)
#define CONSOLE_SUFFIX "\n>"
#endif // CONSOLE_SUFFIX

#define VFS_BASE_PATH "/bin"
#define VFS_PARTITION "bin"

#define CONSOLE_PROMPT (CONSOLE_HOSTNAME ":" CONSOLE_SUFFIX " ")

typedef void (*console_command_t)(size_t argc, char **argv);

extern struct console_process_t
{
  bool running = false;
  TaskHandle_t handle;
  uint32_t stack_deep = 16384U;
  size_t argc = 0;
  char **argv = NULL;
} cmd_process;

// extern console_command_t commands[CMD_HASH_SIZE][CMD_HASH_DEEP];

bool add_command(const char *cmd, console_command_t callback);
bool run_command(String cmd_name);

void console_loop(String *buffer);
void console_task(void *);
void start_console(String *buffer);
String get_console_input(Stream &console = Serial, char end_line = ENTER_COD, int timeout = -1);
#endif // __command_line_h__
