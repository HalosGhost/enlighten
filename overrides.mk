.PHONY: install uninstall

install: bin doc
	@install -Dm755 $(BLDDIR)/$(PROGNM) $(BINDIR)/$(PROGNM)
	@install -Dm644 '90-backlight.rules' $(LIBDIR)/udev/rules.d/'90-backlight.rules'
	@(for i in $(shell basename $(wildcard $(BLDDIR)/*.[0-9])); do \
		section="$${i: -1}"; \
		install -Dm755 $(BLDDIR)/"$$i" $(DOCDIR)/man"$$section"/"$$i"; \
	done)

uninstall: doc
	@rm -f -- $(BINDIR)/$(PROGNM)
	@rm -f -- $(LIBDIR)/udev/rules.d/'90-backlight.rules'
	@(for i in $(shell basename $(wildcard $(BLDDIR)/*.[0-9])); do \
		section="$${i: -1}"; \
		rm -f -- $(DOCDIR)/man"$$section"/"$$i"; \
	done)
