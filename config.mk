# path:   /home/klassiker/.local/share/repos/cinfo/config.mk
# author: klassiker [mrdotx]
# url:    https://github.com/mrdotx/cinfo
# date:   2025-08-14T03:54:11+0200

# tool info
NAME = cinfo
VERSION = 0.5.12

# paths
PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin
MANDIR ?= $(PREFIX)/share/man

CPPFLAGS += -DVERSION=\"$(VERSION)\" -D_DEFAULT_SOURCE
# if your system is not POSIX, add -std=c99 to CFLAGS
CFLAGS += -O2
LDFLAGS ?= -s -lpthread

# compiler and linker
# CC = c99
