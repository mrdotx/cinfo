/*
 * path:   /home/klassiker/.local/share/repos/cinfo/config.def.h
 * author: klassiker [mrdotx]
 * github: https://github.com/mrdotx/cinfo
 * date:   2024-05-15T18:23:22+0200
 */

/* packages */
/* example values for some distros:

| Distro  | PKGS_CMD                                | PKGS_DESC  |
| :------ | :-------------------------------------- | :--------- |
| Arch    | ls -p /var/lib/pacman/local | grep -c / |  [pacman]  |
| Red Hat | rpm -qa | wc -l                         |  [rpm]     |
| Red Hat | dnf list installed | wc -l              |  [dnf]     |
| Red Hat | yum list installed | wc -l              |  [yum]     |
| Debian  | dpkg -l | grep -c '^ii'                 |  [apt]     |
| Gentoo  | qlist -I | wc -l                        |  [portage] |

The result of the shell command in PKGS_CMD must be an integer!
*/
static const char *PKGS_CMD               = "ls -p /var/lib/pacman/local | grep -c /",
                  *PKGS_DESC              = " [pacman]";

/* cpu */
static const char *CPU_TEMP               = "'C",
                  *CPU_TEMP_PATH          = "/sys/class/hwmon",
                  *CPU_TEMP_INPUT_FILE    = "temp1_input",
                  *CPU_TEMP_INPUT[]       = {
                                            "k10temp",      // amd
                                            "coretemp",     // intel
                                            "cpu_thermal"   // arm
                  };

/* memory */
/* values for MEMORY_METHOD:

| Value | Calculation                             | Comparable                  |
| :---: | :-------------------------------------- | :-------------------------- |
| 0     | total - available                       | htop = 3.1.0, btop, polybar |
| 1     | total + shared - free - buffer - cached | htop < 3.1.0, neofetch      |
| 2     | total - free - buffer - cached          | htop > 3.1.0, free          |
*/
static const int MEMORY_METHOD            = 0;

/* values for MEMORY_UNIT:

| Value | Condition         | Description                 |
| :---- | :---------------- | :-------------------------- |
| auto  | total => 1024 MiB | Displays result in Gibibyte |
|       | total < 1024 MiB  | Displays result in Mebibyte |
| GiB   |                   | Displays result in Gibibyte |
| MiB   |                   | Displays result in Mebibyte |
*/
static const char *MEMORY_UNIT            = "auto",
                  *MEMORY_DIVIDER         = "/";

/* shell */
static const char *SHELL_PATH             = "/bin/sh";

/* general */
static const char *INFO_DIVIDER           = " | ";

/* labels */
static const char *LABEL_DISTRO           = " distro",
                  *LABEL_MODEL            = "  model",
                  *LABEL_KERNEL           = " kernel",
                  *LABEL_UPTIME           = " uptime",
                  *LABEL_PKGS             = "   pkgs",
                  *LABEL_SHELL            = "  shell",
                  *LABEL_CPU              = "    cpu",
                  *LABEL_MEM              = " memory";

/* color version */
static const int COLOR_LEFT_LEN           = 13;

static const char *COLOR_LINE             = "─",
                  *COLOR_DIVIDER_TOP      = "┬",
                  *COLOR_DIVIDER          = " │ ",
                  *COLOR_DIVIDER_BOTTOM   = "┴",
                  *COLOR_SYMBOL           = "██",
                  *COLOR_HEADER_PRIMARY   = "\033[1;37m",
                  *COLOR_HEADER_SECONDARY = "\033[0;37m",
                  *COLOR_PRIMARY          = "\033[0;37m",
                  *COLOR_SECONDARY        = "\033[1;37m",
                  *COLOR_TABLE            = "\033[1;34m";

/* ascii version */
static const int ASCII_LEFT_LEN           = 8;

static const char *ASCII_LINE             = "-",
                  *ASCII_DIVIDER_TOP      = "-",
                  *ASCII_DIVIDER          = " | ",
                  *ASCII_DIVIDER_BOTTOM   = "-";

/* cache */
static const char *CACHE_DISTRO_PATH      = "/tmp/distro.cinfo",
                  *CACHE_PKGS_PATH        = "/tmp/pkgs.cinfo",
                  *CACHE_MODEL_PATH       = "/tmp/model.cinfo",
                  *CACHE_CPU_PATH         = "/tmp/cpu.cinfo",
                  *CACHE_CPU_TEMP_PATH    = "/tmp/cpu_temp_path.cinfo";
