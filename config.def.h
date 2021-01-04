/*
 * path:       /home/klassiker/.local/share/repos/cinfo/config.def.h
 * author:     klassiker [mrdotx]
 * github:     https://github.com/mrdotx/cinfo
 * date:       2021-01-04T11:31:10+0100
 */

/* package manager */
const char *PKGCMD = "pacman -Qq | wc -l",
           *PKGDESC = " (pacman)";

/* header */
const char *HEADERSPACER = " ",
           *HEADERBINDER = "@";

/* ascii version */
const int ASCIILINELEFTLEN = 8;
const char *ASCIILINE = "-",
           *ASCIILINEDIVIDER = "+",
           *ASCIIDIVIDER = " | ";

/* color version */
const int COLORLINELEFTLEN = 13;
const char *COLORLINE = "─",
           *COLORLINEDIVIDERTOP = "┬",
           *COLORLINEDIVIDERBOTTOM = "┴",
           *COLORDIVIDER = " │ ",
           *COLORSYMBOL = "██";

/* label */
const char *DISTROTXT = " distro",
           *MODELTXT  = "  model",
           *KERNELTXT = " kernel",
           *UPTIMETXT = " uptime",
           *PKGSTXT   = "   pkgs",
           *SHELLTXT  = "  shell",
           *CPUTXT    = "    cpu",
           *RAMTXT    = "    ram";
