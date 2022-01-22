%include "dict.inc"

extern string_equals
extern string_length
extern string_copy

section .text
POINTER_SIZE equ 8

; @@ Declaration : void* find_word(char* s, void* list)
; @@ Uses:
; @@   rdi: pointer to null-terminated string 
; @@   rsi: pointer to beginning of the list
; @@ Output:
; @@   rax: pointer to item of list if it exists, else 0
find_word:
    cmp rsi, 0
    jne .loop

    .not_found: 
        mov rax, 0
        jmp .end

    .loop:
        push rdi
        push rsi
        add rsi, POINTER_SIZE
        
        call string_equals

        pop rsi
        pop rdi

        cmp rax, 1
        je .found

        .go_to_next:
            mov rsi, [rsi]
            cmp rsi, 0
            je .not_found
            jmp .loop  

        .found:
            mov rax, rsi

    .end:
        ret 


; @@ Declaration : void* get_value_by_item(void * item)
; @@ Uses:
; @@   rdi: pointer to item
; @@ Output:
; @@   rax: return pointer to value
get_value_by_item:
    add rdi, POINTER_SIZE
    push rdi
    call string_length
    pop rdi
    add rdi, rax
    inc rdi
    mov rax, rdi
    ret
