# path:   /home/klassiker/.local/share/repos/cinfo/Makefile
# author: klassiker [mrdotx]
# url:    https://github.com/mrdotx/cinfo
# date:   2025-08-14T03:54:16+0200

.POSIX:

include config.mk

SRC = $(NAME).c util.c
OBJ = $(SRC:.c=.o)

all: options $(NAME)

options:
	@printf "$(NAME) -> build options:\n"
	@printf "  CPPFLAGS = $(CPPFLAGS)\n"
	@printf "  CFLAGS   = $(CFLAGS)\n"
	@printf "  LDFLAGS  = $(LDFLAGS)\n"
	@printf "  CC       = $(CC)\n"
	@printf "$(NAME) -> build:\n"

$(OBJ): config.h $(NAME).1

config.h:
	@printf "  $@\n"
	@cp config.def.h $@

$(NAME).1:
	@printf "  $@\n"
	@pandoc -s --to man $@.md -o $@
	@sed -i '/^.\\\"/d' $@

.c.o:
	@printf "  $@\n"
	@$(CC) -c $(CFLAGS) $(CPPFLAGS) $<

$(NAME): $(OBJ)
	@printf "  $@\n"
	@$(CC) $(LDFLAGS) -o $@ $(OBJ)

dist: clean man all
	@printf "  $(NAME)-$(VERSION).tar.gz\n"
	@mkdir -p $(NAME)-$(VERSION)
	@cp -R \
		LICENSE.md \
		README.md \
		Makefile \
		config.mk \
		config.def.h \
		util.h \
		$(SRC) \
		$(NAME).1 \
		$(NAME).h \
		$(NAME)-$(VERSION)
	@tar -cf - $(NAME)-$(VERSION) | gzip > $(NAME)-$(VERSION).tar.gz
	@rm -rf $(NAME)-$(VERSION)

install: all
	@printf "$(NAME) -> install:\n"
	@printf "  $(DESTDIR)$(MANDIR)/man1/$(NAME).1\n"
	@mkdir -p $(DESTDIR)$(BINDIR) $(DESTDIR)$(MANDIR)/man1
	@sed "s/VERSION/$(VERSION)/g" < $(NAME).1 > $(DESTDIR)$(MANDIR)/man1/$(NAME).1
	@printf "  $(DESTDIR)$(BINDIR)/$(NAME)\n"
	@cp -f $(NAME) $(DESTDIR)$(BINDIR)

uninstall:
	@printf "$(NAME) -> uninstall:\n"
	@printf "  $(DESTDIR)$(BINDIR)/$(NAME)\n"
	@rm -f $(DESTDIR)$(BINDIR)/$(NAME)
	@printf "  $(DESTDIR)$(MANDIR)/man1/$(NAME).1\n"
	@rm -f $(DESTDIR)$(MANDIR)/man1/$(NAME).1

clean:
	@printf "$(NAME) -> clean up build directory:\n"
	@printf "  $(NAME)-$(VERSION).tar.gz\n"
	@rm -f $(NAME)-$(VERSION).tar.gz
	@printf "  $(NAME)\n"
	@rm -f $(NAME)
	@printf "  $(OBJ)\n"
	@rm -f $(OBJ)

man:
	@printf "  $(NAME).1\n"
	@rm -f $(NAME).1

.PHONY: all
	install uninstall dist clean man
