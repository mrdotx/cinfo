/*
 * path:   /home/klassiker/.local/share/repos/cinfo/config.def.h
 * author: klassiker [mrdotx]
 * github: https://github.com/mrdotx/cinfo
 * date:   2022-03-08T12:49:43+0100
 */

/* packages */
static const char *PKGS_PATH            = "/var/lib/pacman/local",
                  *PKGS_DESC            = " [pacman]";

/* general */
static const char *INFO_DIVIDER         = " | ";

/* shell */
static const char *SHELL_PATH           = "/bin/sh";

/* cpu */
static const char *CPU_TEMPERATURE_PATH = "/sys/class/hwmon/hwmon1/temp1_input",
                  *CPU_TEMPERATURE      = "'C";

/* memory */
static const int MEMORY_METHOD          = 2;

static const char *MEMORY_DIVIDER       = "/";

/* color version */
static const int COLOR_LEFT_LEN         = 13;

static const char *COLOR_LINE           = "─",
                  *COLOR_DIVIDER_TOP    = "┬",
                  *COLOR_DIVIDER        = " │ ",
                  *COLOR_DIVIDER_BOTTOM = "┴",
                  *COLOR_SYMBOL         = "██",
                  *COLOR_PRIMARY        = "\x1b[1;37m",
                  *COLOR_SECONDARY      = "\x1b[0;37m",
                  *COLOR_TABLE          = "\x1b[1;34m";

/* ascii version */
static const int ASCII_LEFT_LEN         = 8;

static const char *ASCII_LINE           = "-",
                  *ASCII_DIVIDER_TOP    = "-",
                  *ASCII_DIVIDER        = " | ",
                  *ASCII_DIVIDER_BOTTOM = "-";

/* labels */
static const char *LABEL_DISTRO         = " distro",
                  *LABEL_MODEL          = "  model",
                  *LABEL_KERNEL         = " kernel",
                  *LABEL_UPTIME         = " uptime",
                  *LABEL_PKGS           = "   pkgs",
                  *LABEL_SHELL          = "  shell",
                  *LABEL_CPU            = "    cpu",
                  *LABEL_MEM            = " memory";

/* cache */
static const char *CACHE_DISTRO_PATH    = "/tmp/distro.cinfo",
                  *CACHE_MODEL_PATH     = "/tmp/model.cinfo",
                  *CACHE_CPU_PATH       = "/tmp/cpu.cinfo";
