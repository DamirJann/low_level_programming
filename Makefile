ASM=nasm
ASMFLAGS=-f elf64
LD=ld

all: build clean

build: dict lib main
	$(LD) -o build lib.o dict.o main.o

%: %.asm
	$(ASM) $(ASMFLAGS) -o $@.o $@.asm

run: build
	./$<

clean: 
	rm *.o