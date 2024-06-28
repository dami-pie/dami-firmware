#if !defined(__command_line_h__)
#define __command_line_h__
#include <Arduino.h>
#include <hash.h>
#include <PSRamFS.h>
#include "default_commands.h"

#if !defined(CMD_HASH_SIZE)
#define CMD_HASH_SIZE 37
#endif // CMD_HASH_SIZE
#if !defined(CMD_HASH_DEEP)
#define CMD_HASH_DEEP 7
#endif // CMD_HASH_DEEP

#if !defined(CMD_PROMPT_HEAD)
#define CMD_PROMPT_HEAD "admin@dami-pi:\n> "
#endif // CMD_PROMPT_HEAD

typedef void (*cmd_callback)(size_t argc, char **argv);

extern struct cmd_process_t
{
  bool running = false;
  TaskHandle_t handle;
  uint32_t stack_deep = 16384U;
  size_t argc = 0;
  char **argv = NULL;
} cmd_process;

// extern cmd_callback commands[CMD_HASH_SIZE][CMD_HASH_DEEP];

bool add_cmd(const char *cmd, cmd_callback callback);
bool run_cmd(String cmd_name);

void cmd_loop(String *buffer);
void cmd_input(String &buffer, char end = '\n');
void cmd_task(void *);
void cmd_start(String *buffer);
String get_input(HardwareSerial &console = Serial, char end_line = ENTER_COD, int timeout = -1);
#endif // __command_line_h__
