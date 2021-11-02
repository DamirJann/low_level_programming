section .text

SYS_NUM_READ  equ 0
SYS_NUM_WRITE equ 1
SYS_NUM_EXIT  equ 60

FD_STDIN  equ 0
FD_STDOUT equ 1
FD_STDERR equ 2

ASCII_SPACE equ 0x20
ASCII_TABULATION equ 0x9
ASCII_LINE_FEED equ 0xA
ASCII_NULL equ 0

; Принимает код возврата и завершает текущий процесс
exit: 
    mov     rax, SYS_NUM_EXIT          ; 'exit' syscall number
    xor     rdi, rdi

; Принимает указатель на нуль-терминированную строку, возвращает её длину
; arg1 - rdi(pointer to string)
; ret val - rax(size of string)
string_length:
    mov rcx, 0
    .loop:
        ; [rdi+rax]  ~ *(s + i)
        .check_if_end_of_string:
            cmp [rdi + rcx], byte ASCII_NULL
            je .end
        
        inc rcx

        jmp .loop

    .end:
        mov rax, rcx
        ret

; Принимает указатель на нуль-терминированную строку, выводит её в stdout
; arg1 - rdi(pointer to string)
print_string:
    push rdi
    call string_length 
    pop rdi


    mov rdx, rax                 ; take string length and put it for write call
    mov rsi, rdi                 ; put pointer to string for write call
    mov rax, SYS_NUM_WRITE       ; syscall number(for write = 1)
    mov rdi, FD_STDOUT           ; stdout descriptor
    
    syscall
    
    ret

; Принимает код символа и выводит его в stdout
print_char:
    .allocate_mem_for_buffer
        sub rsp, 1

    mov rdx, 1 
    mov [rsp], dil
    mov rsi, rsp                    ; put pointer to string for write call
    mov rax, SYS_NUM_WRITE          ; syscall number(for write = 1)
    mov rdi, FD_STDOUT              ; stdout descriptor

    syscall
    
    .free_mem_for_buffer:
        add rsp, 1

    .end 
        ret

; Переводит строку (выводит символ с кодом 0xA)
print_newline:
    mov rdi, ASCII_LINE_FEED
    call print_char
    ret

; Выводит беззнаковое 8-байтовое число в десятичном формате 
; Совет: выделите место в стеке и храните там результаты деления
; Не забудьте перевести цифры в их ASCII коды.
print_uint:

    mov r9, 0
    mov r8, 10 
    mov rax, rdi
    mov rdi, rsp

    .allocate_buffer:
        sub rsp, 21

    push rdi
    push r8
    push r9
    mov rdi, rax
    call transfer_to_uint
    pop r9
    pop r8
    pop rdi

    .add_null_symbol_to_result:
        dec rdi
        mov [rdi], byte ASCII_NULL

    .loop:
        ; volume(r8) = 64 bits 
        ; dividend contains in RDX:RAX
        ; rax - quotient, rdx - remainder
        .prepare_and_divide:
            mov rdx, 0
            idiv r8
        
        .add_new_digit_to_result:
            dec rdi
            add rdx, '0'
            mov [rdi], dl
            
        .check_if_zero:
            cmp rax, 0
            jne .loop



    call print_string

    .free_buffer:
        add rsp, 21

    .end:
        ret



; return 0 if number in interval [left border, right border)
; rdi - left border 
; rsi - right border
; rdx - number
; rax - return value, 0 - false, 1 - true

transfer_to_uint:
    mov rax, 0
    add rax, rdi
    .end
        ret
is_in_interval:
    ; check if number < left_border
    cmp rdx, rdi
    jl .end_with_false

    ; check if number >= rigth boreder
    cmp rdx, rsi
    jge .end_with_false

    .end_with_true:
        mov rax, 1
        ret

    .end_with_false:
        mov rax, 0
        ret
     

; Выводит знаковое 8-байтовое число в десятичном формате 
print_int:

    cmp rdi, 0
    jl .handle_negative
    jmp .print

    .handle_negative:

        .multiply_by_minus_one: 
            mov rax, rdi
            mov rdi, 0
            sub rdi, rax

        .print_negative_sign:
            push rdi
            push rax 
            mov dil, '-'
            call print_char 
            pop rax
            pop rdi
  

    .print:
        call print_uint
        
    .end
        ret 



; Принимает два указателя на нуль-терминированные строки, возвращает 1 если они равны, 0 иначе
string_equals:
    xor rax, rax
    ret

; Читает один символ из stdin и возвращает его. Возвращает 0 если достигнут конец потока
read_char:
    .allocate_buffer:
        sub rsp, 1
        ; TODO read_word doesn't work without this line
        mov [rsp], byte ASCII_NULL
    
    mov rdx, 1  
    mov rsi, rsp                  ; put pointer to string for read call
    mov rax, SYS_NUM_READ         ; syscall number(for read = 0)
    mov rdi, FD_STDIN             ; stdout descriptor
    syscall
    
    mov al, [rsi]

    
    .free_allocated_mem_for_buffer:
        add rsp, 1

    .end
        ret 

; Принимает: адрес начала буфера, размер буфера
; Читает в буфер слово из stdin, пропуская пробельные символы в начале, .
; Пробельные символы это пробел 0x20, табуляция 0x9 и перевод строки 0xA.
; Останавливается и возвращает 0 если слово слишком большое для буфера
; При успехе возвращает адрес буфера в rax, длину слова в rdx.
; При неудаче возвращает 0 в rax
; Эта функция должна дописывать к слову нуль-терминатор
read_word:
    .skip_spaces_at_beginning:
        .space_loop:
            push rdi
            push rsi
            push rdx
            call read_char
            pop rdx
            pop rsi
            pop rdi 

            cmp rax, ASCII_SPACE
            je .space_loop
            cmp rax, ASCII_TABULATION
            je .space_loop
            cmp rax, ASCII_LINE_FEED
            je .space_loop

    mov rcx, 0
    .loop:
       

        .add_to_buffer:
            mov [rdi + rcx], al
    
        .break_if_end_of_word:
            cmp al, ASCII_NULL
            je .ok
            cmp al, ASCII_SPACE
            je .ok
        
        .check_if_overflow:
            cmp rcx, rsi
            je .err

        .read_next: 
            push rdi
            push rcx
            push rsi
            push rdx
            call read_char
            pop rdx
            pop rsi
            pop rcx
            pop rdi

            inc rcx
            jmp .loop

    .err: 
        mov rax, 0
        jmp .end
    
    .ok:
        mov rax, rdi
        mov rdx, rcx

        jmp .end

    .end
        ret
 

; Принимает указатель на строку, пытается
; прочитать из её начала беззнаковое число.
; Возвращает в rax: число, rdx : его длину в символах
; rdx = 0 если число прочитать не удалось
parse_uint:
    xor rax, rax
    ret




; Принимает указатель на строку, пытается
; прочитать из её начала знаковое число.
; Если есть знак, пробелы между ним и числом не разрешены.
; Возвращает в rax: число, rdx : его длину в символах (включая знак, если он был) 
; rdx = 0 если число прочитать не удалось
parse_int:
    xor rax, rax
    ret 

; Принимает указатель на строку, указатель на буфер и длину буфера
; Копирует строку в буфер
; Возвращает длину строки если она умещается в буфер, иначе 0
; arg1 - rdi(pointer to string)
; arg2 - rsi(pointer to buffer)
; arg3 - rdx(length of buffer);
; ret val1 - rax(length of buffer it is ok, otherwise, zero)
string_copy:

    push rdi
    call string_length
    pop rdi

    ;string size > buffer size
    cmp rax, rdx
    ja .end_with_zero

    mov rcx, 0
    .loop: 
        cmp rcx, rax
        jg .end
       
        mov r8, [rdi + rcx]
        mov [rsi + rcx], r8
    
        inc rcx
        jmp .loop 

    
    .end_with_zero:
        mov rax, 0
        jmp .end
    
    .end:
        ret


global _start

; _start: 
;     .allocate_buffer:
;         sub rsp, 1
    
;     mov rdx, 1  
;     mov rsi, rsp        ; put pointer to string for read call
;     mov rax, 1         ; syscall number(for read = 0)
;     mov rdi, 0         ; stdout descriptor
;     mov [rsp], byte '!'
;     syscall
    
;     mov al, [rsi]

;     .free_allocated_mem_for_buffer:
;         add rsp, 1

;     call read_char



;     ; sub rsp, 21
;     ; mov rdi, rsp
;     ; mov rsi, 10
;     ; call read_word

;     ; mov rdi, rax

;     ; call print_string


;     ; add rsp, 21


;     call exit
