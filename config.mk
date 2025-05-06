# path:   /home/klassiker/.local/share/repos/cinfo/config.mk
# author: klassiker [mrdotx]
# github: https://github.com/mrdotx/cinfo
# date:   2025-05-06T07:14:27+0200

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
