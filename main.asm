
section .data:
    not_found_error: db "no such key", 0

extern read_word
extern find_word
extern print_char
extern exit
extern get_value_by_key
extern print_string
global _start

%include "words.inc"

BUFFER_MAX_SIZE  equ 255

section .text
_start:
    sub rsp, BUFFER_MAX_SIZE
    mov rdi, rsp
    mov rsi, BUFFER_MAX_SIZE
    call read_word

    mov rdi, rsp
    mov rsi, pointer
    call find_word
    add rsp, BUFFER_MAX_SIZE

    cmp rax, 0
    je .not_found
    jmp .found

    .found:
        sub rsp, BUFFER_MAX_SIZE
        mov rdi, rax
        mov rsi, rsp
        mov rdx, BUFFER_MAX_SIZE
        call get_value_by_key
        mov rdi, rsi
        call print_string
        add rsp, BUFFER_MAX_SIZE
        jmp .end
        
    .not_found:
        mov rdi, not_found_error
        call print_string
        jmp .end
    
    .end
        call exit


