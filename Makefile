.POSIX:

include config.mk

all: cinfo

config.h:
	cp config.def.h config.h

cinfo: cinfo.c config.h

install: cinfo
	mkdir -p $(DESTDIR)$(BINDIR) $(DESTDIR)$(MANDIR)/man1
	cp -f cinfo $(DESTDIR)$(BINDIR)
	sed "s/VERSION/$(VERSION)/g" < cinfo.1 > $(DESTDIR)$(MANDIR)/man1/cinfo.1

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/cinfo $(DESTDIR)$(MANDIR)/man1/cinfo.1

clean:
	rm -f cinfo cinfo-$(VERSION).tar.gz

dist: clean
	mkdir -p cinfo-$(VERSION)
	cp -R README.md LICENSE.md cinfo.1 Makefile config.mk config.h \
		cinfo.c \
		cinfo-$(VERSION)
	tar -cf - cinfo-$(VERSION) | gzip > cinfo-$(VERSION).tar.gz
	rm -rf cinfo-$(VERSION)

.c:
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $< -lutil

.PHONY: all dist clean install uninstall
