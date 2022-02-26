# path:   /home/klassiker/.local/share/repos/cinfo/config.mk
# author: klassiker [mrdotx]
# github: https://github.com/mrdotx/cinfo
# date:   2022-02-26T19:47:35+0100

# tool info
NAME = cinfo
VERSION = 0.4.1

# paths
PREFIX = /usr/local
BINDIR = $(PREFIX)/bin
MANDIR = $(PREFIX)/share/man

CPPFLAGS = -DVERSION=\"$(VERSION)\" -D_DEFAULT_SOURCE
# if your system is not POSIX, add -std=c99 to CFLAGS
CFLAGS = -Os -lpthread
LDFLAGS = -s

# compiler and linker
# CC = c99
