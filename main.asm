
section .data:
    not_found_error: db "no such key", 0

extern read_word
extern find_word
extern print_char
extern exit
extern get_value_by_item
extern print_string
global _start

%include "words.inc"

BUFFER_MAX_SIZE  equ 255

section .text
_start:
    sub rsp, BUFFER_MAX_SIZE
    mov rdi, rsp
    mov rsi, BUFFER_MAX_SIZE
        
    .loop:
        push rdi
        push rsi 

        call read_word

        pop rsi
        pop rdi

        cmp rax, 0
        je .loop
    

    mov rsi, pointer
    call find_word

    add rsp, BUFFER_MAX_SIZE

    cmp rax, 0
    je .not_found

    .found:
        mov rdi, rax
        call get_value_by_item

        mov rdi, rax
        call print_string
        
        jmp .end
        
    .not_found:
        mov rdi, not_found_error
        call print_string
    
    .end
        call exit
