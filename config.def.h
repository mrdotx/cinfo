/*
 * path:       /home/klassiker/.local/share/repos/cinfo/config.def.h
 * author:     klassiker [mrdotx]
 * github:     https://github.com/mrdotx/cinfo
 * date:       2021-01-08T10:19:57+0100
 */

/* package manager */
const char *PKGS_CMD = "pacman -Qq | wc -l",
           *PKGS_DESC = " [pacman]";

/* ascii version */
const int ASCII_LINE_LEFT_LEN = 8;
const char *ASCII_LINE = "-",
           *ASCII_LINE_DIVIDER_TOP = "-",
           *ASCII_LINE_DIVIDER_BOTTOM = "-",
           *ASCII_DIVIDER = " | ";

/* color version */
const int COLOR_LINE_LEFT_LEN = 13;
const char *COLOR_PRIMARY = "\033[1;37m",
           *COLOR_SECONDARY = "\033[0;37m",
           *COLOR_TABLE = "\033[1;34m",
           *COLOR_LINE = "─",
           *COLOR_LINE_DIVIDER_TOP = "┬",
           *COLOR_LINE_DIVIDER_BOTTOM = "┴",
           *COLOR_DIVIDER = " │ ",
           *COLOR_SYMBOL = "██";

/* label */
const char *LABEL_DISTRO = " distro",
           *LABEL_MODEL  = "  model",
           *LABEL_KERNEL = " kernel",
           *LABEL_UPTIME = " uptime",
           *LABEL_PKGS   = "   pkgs",
           *LABEL_SHELL  = "  shell",
           *LABEL_CPU    = "    cpu",
           *LABEL_RAM    = "    ram";
