ASM=nasm
ASMFLAGS=-f elf64
LD=ld

.PHONY: all clean run

all: build clean

build: dict.o lib.o main.o
	$(LD) -o build $^

%.o: %.asm
	$(ASM) $(ASMFLAGS) -o $@ $^

run: build
	./$<

clean: 
	rm *.o
