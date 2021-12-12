%include "words.inc"

global _start
external read_word
external find_word

_start:
    call read_word

    mov rdi, pointer

    call find_word


