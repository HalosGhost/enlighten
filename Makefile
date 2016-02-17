PROGNM =  enlighten
PREFIX ?= /usr/local
DOCDIR ?= $(DESTDIR)$(PREFIX)/share/man
LIBDIR ?= $(DESTDIR)$(PREFIX)/lib
BINDIR ?= $(DESTDIR)$(PREFIX)/bin
ZSHDIR ?= $(DESTDIR)$(PREFIX)/share/zsh
BSHDIR ?= $(DESTDIR)$(PREFIX)/share/bash-completions

.PHONY: all clean cov-build install uninstall

all:
	@mkdir -p ./dist
	@tup upd

clean:
	@rm -rf -- dist cov-int $(PROGNM).tgz make.sh

cov-build: clean
	@tup generate make.sh
	@mkdir -p ./dist
	@cov-build --dir cov-int ./make.sh
	@tar czvf $(PROGNM).tgz cov-int

install:
	@install -Dm755 dist/enlighten  $(BINDIR)/enlighten
	@install -Dm644 dist/zsh        $(ZSHDIR)/site-functions/_enlighten

uninstall:
	@rm -f $(BINDIR)/enlighten
	@rm -f $(ZSHDIR)/site-functions/_enlighten
