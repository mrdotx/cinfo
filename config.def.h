/*
 * path:   /home/klassiker/.local/share/repos/cinfo/config.def.h
 * author: klassiker [mrdotx]
 * github: https://github.com/mrdotx/cinfo
 * date:   2021-06-12T08:35:06+0200
 */

/* package manager */
static const char *PKGS_CMD = "pacman -Qq | wc -l",
                  *PKGS_DESC = " [pacman]";

/* general */
static const char *INFO_DIVIDER = " | ";

/* shell */
static const char *SHELL_PATH = "/bin/sh",
                  *SHELL_DIVIDER = " -> ";

/* cpu */
static const char *CPU_TEMPERATURE = "C";

/* memory */
static const int MEMORY_HTOP_METHOD = 1;

static const char *MEMORY_DIVIDER = "/";

/* color version */
static const int COLOR_LEFT_LEN = 13;

static const char *COLOR_LINE = "─",
                  *COLOR_DIVIDER_TOP = "┬",
                  *COLOR_DIVIDER = " │ ",
                  *COLOR_DIVIDER_BOTTOM = "┴",
                  *COLOR_SYMBOL = "██",
                  *COLOR_PRIMARY = "\033[1;37m",
                  *COLOR_SECONDARY = "\033[0;37m",
                  *COLOR_TABLE = "\033[1;34m";

/* ascii version */
static const int ASCII_LEFT_LEN = 8;

static const char *ASCII_LINE = "-",
                  *ASCII_DIVIDER_TOP = "-",
                  *ASCII_DIVIDER = " | ",
                  *ASCII_DIVIDER_BOTTOM = "-";

/* labels */
static const char *LABEL_DISTRO = " distro",
                  *LABEL_MODEL  = "  model",
                  *LABEL_KERNEL = " kernel",
                  *LABEL_UPTIME = " uptime",
                  *LABEL_PKGS   = "   pkgs",
                  *LABEL_SHELL  = "  shell",
                  *LABEL_CPU    = "    cpu",
                  *LABEL_MEM    = " memory";
