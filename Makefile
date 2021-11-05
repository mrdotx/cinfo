# path:   /home/klassiker/.local/share/repos/cinfo/Makefile
# author: klassiker [mrdotx]
# github: https://github.com/mrdotx/cinfo
# date:   2021-11-05T13:14:09+0100

.POSIX:

include config.mk

all: options cinfo cinfo.1

options:
	@echo cinfo build options:
	@echo "CFLAGS   = $(CFLAGS)"
	@echo "LDFLAGS  = $(LDFLAGS)"
	@echo "CC       = $(CC)"

config.h:
	cp config.def.h $@

cinfo: cinfo.c config.h

install: all
	mkdir -p $(DESTDIR)$(BINDIR) $(DESTDIR)$(MANDIR)/man1
	cp -f cinfo $(DESTDIR)$(BINDIR)
	sed -i '1,2d' cinfo.1
	sed "s/VERSION/$(VERSION)/g" < cinfo.1 > $(DESTDIR)$(MANDIR)/man1/cinfo.1

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/cinfo $(DESTDIR)$(MANDIR)/man1/cinfo.1

clean:
	rm -f cinfo cinfo-$(VERSION).tar.gz cinfo.1

dist: clean
	mkdir -p cinfo-$(VERSION)
	cp -R README.md LICENSE.md cinfo.1 Makefile config.mk config.h \
		cinfo.c \
		cinfo-$(VERSION)
	tar -cf - cinfo-$(VERSION) | gzip > cinfo-$(VERSION).tar.gz
	rm -rf cinfo-$(VERSION)

.c:
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $< -lutil

.PHONY: all options clean dist install uninstall
