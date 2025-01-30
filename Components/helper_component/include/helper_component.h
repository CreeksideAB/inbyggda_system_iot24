#pragma once

#define ANSI_RED "\033[0;31m"
#define ANSI_GREEN "\033[0;32m"
#define ANSI_YELLOW "\033[0;33m"
#define ANSI_BLUE "\033[0;34m"
#define ANSI_MAGENTA "\033[0;35m"
#define ANSI_CYAN "\033[0;36m"
#define ANSI_RESET "\033[0m"
#define NEW_LINE "\n"

#define PRINTF_COLOR(color, format_string, ...) \
    printf("%s" fomrat_string "%s", color, ##__VA_ARGS__, ANSI_RESET)

#define PRINTF_COLOR_NL(color, format_string, ...) \
    printf("%s" fomrat_string "%s" NEW_LINE color, ##__VA_ARGS__, ANSI_RESET)

#define PRINT_GROUP_COLOR(group_color, group, color, format_string, ...) \
    printf("[%s%s:%d%s] ", group_color, __FILE__, __LINE__, ANSI_RESET); \
    printf("%s" group "%s ", group_color, ANSI_RESET);                   \
    printf("%s" format_string "%s", color, ##__VA_ARGS__, ANSI_RESET)

#define PRINT_GROUP_COLOR_NL(group_color, group, color, format_string, ...) \
    printf("[%s%s:%d%s] ", group_color, __FILE__, __LINE__, ANSI_RESET);    \
    printf("%s" group "%s ", group_color, ANSI_RESET);                      \
    printf("%s" format_string "%s" NEW_LINE, color, ##__VA_ARGS__, ANSI_RESET)

namespace inbyggda_iot24
{
    int func(void);
}
