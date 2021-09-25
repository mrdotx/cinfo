# path:   /home/klassiker/.local/share/repos/cinfo/config.mk
# author: klassiker [mrdotx]
# github: https://github.com/mrdotx/cinfo
# date:   2021-09-25T13:32:22+0200

# cinfo version
VERSION = 0.3.7

# paths
PREFIX	= /usr/local
BINDIR	= $(PREFIX)/bin
MANDIR	= $(PREFIX)/share/man

CPPFLAGS = -DVERSION=\"$(VERSION)\" -D_DEFAULT_SOURCE
# if your system is not POSIX, add -std=c99 to CFLAGS
CFLAGS = -Os -lpthread
LDFLAGS = -s

# compiler and linker
# CC = c99
