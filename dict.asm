%include "dict.inc"

extern string_equals

section .text
ASCII_NULL equ 0
POINTER_SIZE equ 8

; @@ Declaration : void* find_word(char* s, void* list)
; @@ Uses:
; @@   rdi: pointer to null-terminated string 
; @@   rsi: pointer to beginning of the list
; @@ Output:
; @@   rax: pointer to item of list if it exists, else 0
find_word:
    mov rax, ASCII_NULL

    .loop:
        cmp rsi, ASCII_NULL
        je .end

        push rsi
        push rdi
        add rsi, POINTER_SIZE
        
        call string_equals

        pop rdi
        pop rsi


        cmp rax, 1
        je .found
        jmp .go_to_next

        .found:
            mov rax, rsi
            jmp .end

        .go_to_next:
            mov rdi, [rdi]
            jmp .loop

    .end:
        ret 

    