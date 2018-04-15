PROGNM =  enlighten
PREFIX ?= /usr/local
DOCDIR ?= $(DESTDIR)$(PREFIX)/share/man
LIBDIR ?= $(DESTDIR)$(PREFIX)/lib
BINDIR ?= $(DESTDIR)$(PREFIX)/bin
ZSHDIR ?= $(DESTDIR)$(PREFIX)/share/zsh
BSHDIR ?= $(DESTDIR)$(PREFIX)/share/bash-completions

CC = clang
CFLAGS ?= -g -ggdb -O2 -fPIE -pie -flto -D_FORTIFY_SOURCE=2 -fstack-protector-strong --param=ssp-buffer-size=1 -Weverything -Werror -std=c11 -fsanitize=undefined -fsanitize-trap=undefined -Wl,-z,relro,-z,now
CFLAGS += -Wno-disabled-macro-expansion

.PHONY: all clean clang-analyze cov-build install uninstall

all: dist
	@$(CC) $(CFLAGS) src/$(PROGNM).c -o dist/$(PROGNM)

clean:
	@rm -rf -- dist cov-int $(PROGNM).tgz make.sh ./src/*.plist

dist:
	@mkdir -p ./dist

cov-build: dist
	@cov-build --dir cov-int ./make.sh
	@tar czvf $(PROGNM).tgz cov-int

clang-analyze:
	@(pushd ./src; clang-check -analyze ./*.c)

install:
	@install -Dm755 dist/$(PROGNM) $(BINDIR)/$(PROGNM)
	@install -Dm755 90-backlight.rules $(LIBDIR)/udev/rules.d/90-backlight.rules

uninstall:
	@rm -f -- $(BINDIR)/$(PROGNM)
	@rm -f -- $(LIBDIR)/udev/rules.d/90-backlight.rules
