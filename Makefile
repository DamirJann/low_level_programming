
all: word

	nasm -f elf64 -o preprocessed_word -e  word.inc

word: 
	nasm -f elf64 -o word.O word.inc

preprocessed_word:
	nasm -f elf64 -o preprocessed_word -e  word.inc