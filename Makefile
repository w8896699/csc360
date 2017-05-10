.phony all:
all: MFS

rsi: MFS.c
	gcc MFS.c -phread  -o rsi

.PHONY clean:
clean:
	-rm -rf *.o *.exe