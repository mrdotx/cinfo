/*
 * path:   /home/klassiker/.local/share/repos/cinfo/config.def.h
 * author: klassiker [mrdotx]
 * github: https://github.com/mrdotx/cinfo
 * date:   2022-08-21T21:59:40+0200
 */

/* packages */
static const char *PKGS_PATH            = "/var/lib/pacman/local",
                  *PKGS_DESC            = " [pacman]";

/* cpu */
static const char *CPU_TEMP_PATH        = "/sys/class/hwmon",
                  *CPU_TEMP             = "'C",
                  *CPU_TEMP_INPUT[]     = {
    "k10temp",      // amd
    "coretemp",     // intel
    "cpu_thermal",  // arm
    0               // exit point
};

/* memory */
static const int MEMORY_METHOD          = 0;

static const char *MEMORY_UNIT          = "auto",
                  *MEMORY_DIVIDER       = "/";

/* shell */
static const char *SHELL_PATH           = "/bin/sh";

/* general */
static const char *INFO_DIVIDER         = " | ";

/* labels */
static const char *LABEL_DISTRO         = " distro",
                  *LABEL_MODEL          = "  model",
                  *LABEL_KERNEL         = " kernel",
                  *LABEL_UPTIME         = " uptime",
                  *LABEL_PKGS           = "   pkgs",
                  *LABEL_SHELL          = "  shell",
                  *LABEL_CPU            = "    cpu",
                  *LABEL_MEM            = " memory";

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

/* cache */
static const char *CACHE_DISTRO_PATH    = "/tmp/distro.cinfo",
                  *CACHE_PKGS_PATH      = "/tmp/pkgs.cinfo",
                  *CACHE_MODEL_PATH     = "/tmp/model.cinfo",
                  *CACHE_CPU_PATH       = "/tmp/cpu.cinfo",
                  *CACHE_CPU_TEMP_PATH  = "/tmp/cpu_temp_path.cinfo";
