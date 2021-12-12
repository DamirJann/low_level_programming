
all: build

build: dict.o lib.o main.o
	ld -o build lib.o dict.o main.o

dict.o: dict.asm
	nasm -f elf64 -o dict.o dict.asm 

lib.o: lib.asm
	nasm -f elf64 -o lib.o lib.asm

main.o: main.asm
	nasm -f elf64 -o main.o main.asm