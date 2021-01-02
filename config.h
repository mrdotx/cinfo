/*
 * path:       /home/klassiker/.local/share/repos/cinfo/config.h
 * author:     klassiker [mrdotx]
 * github:     https://github.com/mrdotx/cinfo
 * date:       2021-01-02T23:20:34+0100
 */

#define PKGCMD "pacman -Qq | wc -l"
#define PKGDESC " (pacman)"

#define HEADERSPACER " "
#define HEADERBINDER "@"

#define ASCIILINELEFTLEN 8
#define ASCIILINE "-"
#define ASCIILINEDIVIDER "+"
#define ASCIIDIVIDER " | "

#define COLORLINELEFTLEN 13
#define COLORLINE "─"
#define COLORLINEDIVIDERTOP "┬"
#define COLORLINEDIVIDERBOTTOM "┴"
#define COLORDIVIDER " │ "
#define COLORSYMBOL "██"

#define DISTROTXT   " distro"
#define MODELTXT    "  model"
#define KERNELTXT   " kernel"
#define UPTIMETXT   " uptime"
#define PKGSTXT     "   pkgs"
#define SHELLTXT    "  shell"
#define CPUTXT      "    cpu"
#define RAMTXT      "    ram"

#define HELP "cinfo [-h] -- a minimal system information tool\n\
              for arch-based distributions\n\
  Usage:\n\
    cinfo [-a]\n\n\
  Settings:\n\
    without given settings, start colored version\n\
    [-a] = start plain ascii version\n\n\
  Examples:\n\
    cinfo\n\
    cinfo -a"

#define RESET "\033[0m"

#define BLACK "\033[0;30m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m"

#define BBLACK "\033[1;30m"
#define BRED "\033[1;31m"
#define BGREEN "\033[1;32m"
#define BYELLOW "\033[1;33m"
#define BBLUE "\033[1;34m"
#define BMAGENTA "\033[1;35m"
#define BCYAN "\033[1;36m"
#define BWHITE "\033[1;37m"
