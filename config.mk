# path:   /home/klassiker/.local/share/repos/cinfo/config.mk
# author: klassiker [mrdotx]
# github: https://github.com/mrdotx/cinfo
# date:   2021-05-22T12:57:24+0200

# cinfo version
VERSION = 0.3.5

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
