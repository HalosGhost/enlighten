PROGNM =  enlighten
PREFIX ?= /usr/local
DOCDIR ?= $(DESTDIR)$(PREFIX)/share/man
LIBDIR ?= $(DESTDIR)$(PREFIX)/lib
BINDIR ?= $(DESTDIR)$(PREFIX)/bin
ZSHDIR ?= $(DESTDIR)$(PREFIX)/share/zsh
BSHDIR ?= $(DESTDIR)$(PREFIX)/share/bash-completions
MKDIR  ?= mkdir -p

include Makerules

.PHONY: all bin clean scan-build cov-build complexity doc verify install uninstall

all: dist bin check doc

bin: dist
	@$(CC) $(CFLAGS) src/*.c -DVERSION="\"$(VER)\n\"" -o dist/$(PROGNM)

check: bin
	@./test-suite

clean:
	@rm -rf -- dist cov-int $(PROGNM).tgz make.sh ./src/*.plist

dist:
	@$(MKDIR) ./dist

doc: dist
	@(cd doc; \
		sphinx-build -b man -Dversion=$(VER) \
			-d doctree -E . ../dist $(PROGNM).rst; \
		rm -r -- doctree; \
	)

cov-build: dist
	@cov-build --dir cov-int ./make.sh
	@tar czvf $(PROGNM).tgz cov-int

complexity: bin
	complexity -h ./src/*

scan-build:
	@scan-build --use-cc=$(CC) make bin

verify:
	@frama-c $(FMFLAGS) src/*.c

install:
	@install -Dm755 dist/$(PROGNM) $(BINDIR)/$(PROGNM)
	@install -Dm755 dist/$(PROGNM).1 $(DOCDIR)/man1/$(PROGNM).1
	@install -Dm755 90-backlight.rules $(LIBDIR)/udev/rules.d/90-backlight.rules

uninstall:
	@rm -f -- $(BINDIR)/$(PROGNM)
	@rm -f -- $(DOCDIR)/$(PROGNM).1
	@rm -f -- $(LIBDIR)/udev/rules.d/90-backlight.rules
