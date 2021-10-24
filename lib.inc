section .text
 
 
; Принимает код возврата и завершает текущий процесс
exit: 
    xor rax, rax,
    ret 

; Принимает указатель на нуль-терминированную строку, возвращает её длину
; arg1 - rdi(pointer to string)
; ret val - rax(size of string)
string_length:
    xor rax, rax
    .loop:
        ; [rdi+rax]  ~ *(s + i)
        cmp byte [rdi + rax], 0
        je .end

        inc rax
        jmp .loop

    .end:
        ret

; Принимает указатель на нуль-терминированную строку, выводит её в stdout
; print_string - caller-save. It means that we need to save rdi and rax? before execution string_length
; arg1 - rdi(pointer to string)
print_string:
    push rdi

    call string_length 

    pop rdi


    mov rdx, rax        ; take string length and put it for write call
    mov rsi, rdi        ; put pointer to string for write call
    mov rax, 1          ; syscall number(for write = 1)
    mov rdi, 1          ; stdout descriptor
    
    syscall
    
    ret

; Принимает код символа и выводит его в stdout
print_char:

    mov rdx, 1  
    
    ; TODO 
    ; change it. Could I use pointer to r8 as a buffer? Why stack? 
    push rdi
    lea rsi, [rsp]       ; put pointer to string for write call
    pop r8
    
    mov rax, 1          ; syscall number(for write = 1)
    mov rdi, 1          ; stdout descriptor
    syscall
    
    ret

; Переводит строку (выводит символ с кодом 0xA)
print_newline:
    xor rax, rax
    ret

; Выводит беззнаковое 8-байтовое число в десятичном формате 
; Совет: выделите место в стеке и храните там результаты деления
; Не забудьте перевести цифры в их ASCII коды.
print_uint:
    ; le
    mov rdi, 23
    mov r8, 10 ; volume(r8d) = 32 bits 
               ; dividend contains in RDX:RAX
               ; rax - quotient, rdx - remainder
        
    push rdi

    .loop:
        pop rdi
        
        cmp rdi, 0
        je .end

        mov rax, edi
        sar rdi, 32
        mov rdx, edi

        div r8

        push rax

        mov rdi, rdx
        call print_char

        jmp .loop

    .end:
        ret


; return 0 if number in interval [left border, right border)
; rdi - left border 
; rsi - right border
; rdx - number
; rax - return value, 0 - false, 1 - true
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
    mov rdx, 8 
    ; TODO 
    ; change it. Could I use pointer to r8 as a buffer? Why stack? 
    push rdi
    lea rsi, [rsp]       ; put pointer to string for write call
    pop r8
    
    mov rax, 1          ; syscall number(for write = 1)
    mov rdi, 1          ; stdout descriptor
    syscall
    
    ret

; Принимает два указателя на нуль-терминированные строки, возвращает 1 если они равны, 0 иначе
string_equals:
    xor rax, rax
    ret

; Читает один символ из stdin и возвращает его. Возвращает 0 если достигнут конец потока
read_char:
    
    
    syscall
    
    ret 

; Принимает: адрес начала буфера, размер буфера
; Читает в буфер слово из stdin, пропуская пробельные символы в начале, .
; Пробельные символы это пробел 0x20, табуляция 0x9 и перевод строки 0xA.
; Останавливается и возвращает 0 если слово слишком большое для буфера
; При успехе возвращает адрес буфера в rax, длину слова в rdx.
; При неудаче возвращает 0 в rax
; Эта функция должна дописывать к слову нуль-терминатор

read_word:
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
        ; TODO 
        ; find out why it doesn't work with ja
        jg .end
       
        mov byte r8, [rdi + rcx]
        mov [rsi + rcx], byte r8
    
        inc rcx
        jmp .loop 

    
    .end_with_zero:
        mov rax, 0
        jmp .end
    
    .end:
        ret



    