/*
 * path:       /home/klassiker/.local/share/repos/cinfo/config.def.h
 * author:     klassiker [mrdotx]
 * github:     https://github.com/mrdotx/cinfo
 * date:       2021-01-08T20:48:57+0100
 */

/* package manager */
const char *PKGS_CMD = "pacman -Qq | wc -l",
           *PKGS_DESC = " [pacman]";

/* ascii version */
const int ASCII_LEFT_LEN = 8;

const char *ASCII_LINE = "-",
           *ASCII_DIVIDER_TOP = "-",
           *ASCII_DIVIDER = " | ",
           *ASCII_DIVIDER_BOTTOM = "-";

/* color version */
const int COLOR_LEFT_LEN = 13;

const char *COLOR_LINE = "─",
           *COLOR_DIVIDER_TOP = "┬",
           *COLOR_DIVIDER = " │ ",
           *COLOR_DIVIDER_BOTTOM = "┴",
           *COLOR_SYMBOL = "██",
           *COLOR_PRIMARY = "\033[1;37m",
           *COLOR_SECONDARY = "\033[0;37m",
           *COLOR_TABLE = "\033[1;34m";

/* label */
const char *LABEL_DISTRO = " distro",
           *LABEL_MODEL  = "  model",
           *LABEL_KERNEL = " kernel",
           *LABEL_UPTIME = " uptime",
           *LABEL_PKGS   = "   pkgs",
           *LABEL_SHELL  = "  shell",
           *LABEL_CPU    = "    cpu",
           *LABEL_RAM    = "    ram";
