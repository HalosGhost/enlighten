.PHONY: init

init:
	@cp skeleton ../Makefile
	@cp configure ..
	@sed -e "s/programname/$(PROGNM)/" -i ../Makefile
