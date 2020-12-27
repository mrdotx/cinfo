.POSIX:

include config.mk

all: cinfo

cinfo: cinfo.c

install: cinfo
	mkdir -p $(DESTDIR)$(BINDIR)
	cp -f cinfo $(DESTDIR)$(BINDIR)

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/cinfo

clean:
	rm -f cinfo

.c:
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $< -lutil

.PHONY: all install uninstall clean
