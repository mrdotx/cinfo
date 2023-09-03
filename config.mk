# path:   /home/klassiker/.local/share/repos/cinfo/config.mk
# author: klassiker [mrdotx]
# github: https://github.com/mrdotx/cinfo
# date:   2023-09-02T16:33:35+0200

# tool info
NAME = cinfo
VERSION = 0.5.3

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
