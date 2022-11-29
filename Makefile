none:
	@echo "To make a Atari	target binary, type:"
	@echo "   make atari"

all: atari

clean:
	-rm potion.xex
	-rm -rf local target smalltarget

atari:
	make -f Makefile.atari

.PHONY: all clean
