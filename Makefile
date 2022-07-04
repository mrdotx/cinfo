# path:   /home/klassiker/.local/share/repos/cinfo/Makefile
# author: klassiker [mrdotx]
# github: https://github.com/mrdotx/cinfo
# date:   2022-07-04T21:58:48+0200

.POSIX:

include config.mk

all: options $(NAME)

$(NAME): config.h $(NAME).c $(NAME).1

options:
	@printf "build options:\n"
	@printf "  CPPFLAGS = $(CPPFLAGS)\n"
	@printf "  CFLAGS   = $(CFLAGS)\n"
	@printf "  LDFLAGS  = $(LDFLAGS)\n"
	@printf "  CC       = $(CC)\n"
	@printf "build:\n"

config.h:
	@printf "  $@\n"
	@cp config.def.h $@

$(NAME).1:
	@printf "  $@\n"
	@pandoc -s --to man $@.md -o $@
	@sed -i '/^.\\\"/d' $@

.c:
	@printf "  $@\n"
	@$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $<

dist: clean man all
	@printf "  $(NAME)-$(VERSION).tar.gz\n"
	@mkdir -p $(NAME)-$(VERSION)
	@cp -R \
		$(NAME).1 \
		$(NAME).c \
		config.h \
		config.mk \
		LICENSE.md \
		Makefile \
		README.md \
		$(NAME)-$(VERSION)
	@tar -cf - $(NAME)-$(VERSION) | gzip > $(NAME)-$(VERSION).tar.gz
	@rm -rf $(NAME)-$(VERSION)

install: all
	@printf "install:\n"
	@printf "  $(DESTDIR)$(BINDIR)/man1/$(NAME).1\n"
	@mkdir -p $(DESTDIR)$(BINDIR) $(DESTDIR)$(MANDIR)/man1
	@sed "s/VERSION/$(VERSION)/g" < $(NAME).1 > $(DESTDIR)$(MANDIR)/man1/$(NAME).1
	@printf "  $(DESTDIR)$(BINDIR)/$(NAME)\n"
	@cp -f $(NAME) $(DESTDIR)$(BINDIR)

uninstall:
	@printf "uninstall:\n"
	@printf "  $(DESTDIR)$(BINDIR)/$(NAME)\n"
	@rm -f $(DESTDIR)$(BINDIR)/$(NAME)
	@printf "  $(DESTDIR)$(MANDIR)/man1/$(NAME).1\n"
	@rm -f $(DESTDIR)$(MANDIR)/man1/$(NAME).1

clean:
	@printf "remove:\n"
	@printf "  $(NAME)-$(VERSION).tar.gz\n"
	@rm -f $(NAME)-$(VERSION).tar.gz
	@printf "  $(NAME)\n"
	@rm -f $(NAME)

man:
	@printf "  $(NAME).1\n"
	@rm -f $(NAME).1

.PHONY: all
	install uninstall dist clean man
