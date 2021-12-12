%include "dict.inc"

extern string_equals
extern string_length
extern string_copy

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
    .loop:
        push rdi
        push rsi
        add rsi, POINTER_SIZE
        
        call string_equals

        pop rsi
        pop rdi

        cmp rax, 1
        je .found
        jmp .go_to_next

        .found:
            mov rax, rsi
            jmp .end

        .go_to_next:
            mov rsi, [rsi]
            cmp sil, ASCII_NULL
            je .not_found
            jmp .loop

    .not_found: 
        mov rax, ASCII_NULL
        jmp .end

    .end:
        ret 


; @@ Declaration : void* get_value_by_key(char* s, int buffer_size, void* item)
; @@ Uses:
; @@   rdi: pointer to item
; @@   rsi: pointer to buffer
; @@   rdx: size of buffer
; @@ Output:
; @@   rax: return size of string value, else 0
get_value_by_key:
    add rdi, POINTER_SIZE
    push rdi
    call string_length
    pop rdi
    add rdi, rax
    inc rdi

    call string_copy 

    ret