/*
 * path:       /home/klassiker/.local/share/repos/cinfo/config.def.h
 * author:     klassiker [mrdotx]
 * github:     https://github.com/mrdotx/cinfo
 * date:       2021-01-06T17:24:53+0100
 */

/* package manager */
const char *PKG_CMD = "pacman -Qq | wc -l",
           *PKG_DESC = " [pacman]";

/* ascii version */
const int ASCII_LINELEFTLEN = 8;
const char *ASCII_LINE = "-",
           *ASCII_LINEDIVIDER = "+",
           *ASCII_DIVIDER = " | ";

/* color version */
const int COLOR_LINELEFTLEN = 13;
const char *COLOR_PRIMARY = "\033[1;37m",
           *COLOR_SECONDARY = "\033[0;37m",
           *COLOR_TABLE = "\033[1;34m",
           *COLOR_LINE = "─",
           *COLOR_LINEDIVIDERTOP = "┬",
           *COLOR_LINEDIVIDERBOTTOM = "┴",
           *COLOR_DIVIDER = " │ ",
           *COLOR_SYMBOL = "██";

/* label */
const char *DISTRO_TXT = " distro",
           *MODEL_TXT  = "  model",
           *KERNEL_TXT = " kernel",
           *UPTIME_TXT = " uptime",
           *PKGS_TXT   = "   pkgs",
           *SHELL_TXT  = "  shell",
           *CPU_TXT    = "    cpu",
           *RAM_TXT    = "    ram";
