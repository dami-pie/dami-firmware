#pragma once

#define ansi_text_low "\e[2m"
#define ansi_text_bold "\e[1m"
#define ansi_text_blink "\e[5m"
#define ansi_text_normal "\e[0m"
#define ansi_text_reverse "\e[7m";
#define ansi_text_underline "\e[4m"
#define ansi_text_invisible "\e[8m"
#define ansi_text_blink_fast "\e[6m"
#define ansi_text_strike_through "\e[9m"

#define ansi_clear_line "\e[%uK"
#define ansi_clear_screen "\e[2J\e[H"

#define ansi_go_home "\e[H"
#define ansi_goto_xy "\e[%u;%uH"

#define ansi_cursor_up "\e[%uA"
#define ansi_cursor_back "\e[%uD"
#define ansi_cursor_down "\e[%uB"
#define ansi_cursor_forward "\e[%uC"

#define ansi_device_type "\e[0c"

#define ansi_read_cursor_position "\e[6n"

#define ansi_set_80_columns "\e[?3l"
#define ansi_set_132_columns "\e[?3h"
#define ansi_set_jump_scroll "\e[?4l"
#define ansi_set_smooth_scroll "\e[?4h"

#define ansi_move_window_up "\eD"
#define ansi_move_window_down "\eM"

#define ansi_print_screen "\e[i"
#define ansi_print_mode_on "\e[5i"
#define ansi_print_mode_off "\e[4i"
#define ansi_print_line "\e[1i"

#define ansi_reset_screen "\ebc"

#define ansi_color8 "\e[%u;%u;%um"
#define ansi_RGB_foreground "\e[38;2;%c;%c;%cm"
#define ansi_RGB_background "\e[48;2;%c;%c;%cm"
