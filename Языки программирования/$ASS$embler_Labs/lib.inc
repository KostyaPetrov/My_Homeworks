section .text

;Когда-нибудь на лекциях ВТ будут давать достаточно информации,
;чтобы делать лабы по проге не с 300 вкладками в firefox...

; СДЕЛАНО
exit:
    mov rax, 60
    xor rdi, rdi
    syscall


; СДЕЛАНО
string_length:
    xor rcx, rcx
.count:
    cmp byte[rdi+rcx], 0
    je .end
    inc rcx
    jmp .count
.end:
    mov rax, rcx
    ret
    


; СДЕЛАНО
print_string:
    call string_length
    mov rdx, rax; data register with string length
    mov rax, 1 ; accumulator with syscall number
    mov rsi, rdi ;string start
    mov rdi, 1 ;string destination
    syscall
    ret

; СДЕЛАНО
print_char:
    push rdi
    mov rsi, rsp
    mov rax, 1
    mov rdi, 1 
    mov rdx, 1
    syscall
    pop rdi
    ret

; СДЕЛАНО
print_newline:
    mov rax, 10
    mov rdi, rax
    call print_char
    ret

;СДЕЛАНО, ЧЁРТ ПОБЕРИ!
print_uint:
    xor rax, rax 
    mov r9, 10 ;делитель
    mov r10, rsp ; сохраняем указатель на стек
    mov rax, rdi;Идём по порядку 0, 6, 5, 2, а потом выводим 
    push 0
.cycle:
        xor rdx,rdx ;используем rdx из-за особенностей работы div
        ;в нём хранится остаток от деления
        ;обнуляем, чтобы избежать переполнения формата(особенность деления rdx:rax)
        div r9 ;целая часть в rax
        add rdx, 0x30 ;переводим символ в ASCII
        dec rsp ;'вручную' кладём элемент в стек, чтобы длина шага у всех была равна 1
        mov byte[rsp], dl ;записываем остаток в вершину стека
        cmp rax,0 ;если конец выходим, в ином случае продолжаем 
        je .end
        jmp .cycle

.end:
        mov rdi, rsp ;записываем адрес первого символа и вызываем print_string
        push r10
        call print_string
        pop r10
        mov rsp, r10 ; восстанавливаем sp
        ret

;СДЕЛАНО
print_int:
    cmp rdi,0
    jl .negative
    call print_uint
    ret
.negative:
    push rdi ; полагаю, в комментариях не нуждается
    mov rdi, '-' ;WSL Ubuntu почему-то не хочет нормально знак минуса выводить...
    call print_char;сначала выводим минус, а потом уже число
    pop rdi
    neg rdi
    call print_uint ;выводим на экран
    ret
;СДЕЛАНО
string_equals:
    xor rax, rax
    xor rcx, rcx ;counter
    xor r9, r9
    xor r10, r10
.cycle:
        mov r9b, byte[rdi+rcx];используем 8 бит
        mov r10b, byte[rsi+rcx]
        cmp r9b,r10b 
        jne .false
        cmp r9b, 0
        je .true
        inc rcx ;counter
        jmp .cycle
    
.false:
        mov rax,0
        ret
.true:
        mov rax, 1
        ret

;СДЕЛАНО
read_char:
    xor rax, rax ;stdin дескриптор 0
    xor rdi,rdi
    mov rdx, 1
    push 0 ;кладем пустой символ
    mov rsi, rsp ;
    syscall
    pop rax ;загружаем символ со стека в rax
    ret

; Принимает: адрес начала буфера, размер буфера
; Читает в буфер слово из stdin, пропуская пробельные символы в начале, .
; Пробельные символы это пробел 0x20, табуляция 0x9 и перевод строки 0xA.
; Останавливается и возвращает 0 если слово слишком большое для буфера
; При успехе возвращает адрес буфера в rax, длину слова в rdx.
; При неудаче возвращает 0 в rax
; Эта функция должна дописывать к слову нуль-терминатор

;СДЕЛАНО
read_word:
    xor rcx, rcx
    xor rax, rax
    mov r9, rdi ;адрес начала
    mov r10, rsi ;размер  буфера
.cycle:
    push rcx
    push r10
    push r9
    call read_char ;читаем символ
    pop r9
    pop r10
    pop rcx
    cmp rax, 0 ;если конец, переход на .end
    je .end
    cmp rax, ' ';куча проверок на пробельные символы
    je .skip
    cmp rax, '	'
    je .skip
    cmp rax, '\n'
    je .skip
    mov [r9+rcx], rax
    inc rcx
    cmp rcx, r10
    jge .no
    jmp .cycle
.skip:
    cmp rcx,0 ;смотрим, началось ли уже слово
    je .cycle
    jmp .end
.no:
    xor rax, rax
    xor rdx, rdx
    ret
.end:
    xor rax, rax
    mov [r9+rcx], rax ;нулевой символ
    mov rax, r9
    mov rdx, rcx ;длина
    ret

; Принимает указатель на строку, пытается
; прочитать из её начала беззнаковое число.
; Возвращает в rax: число, rdx : его длину в символах
; rdx = 0 если число прочитать не удалось

;СДЕЛАНО(на часах 6:24, хочется сдохнуть...)
parse_uint:
    xor rax, rax
    xor rcx, rcx ;counter
    mov r9, 10 ;основание системы
.cycle:
    movzx r10, byte[rdi+rcx]
    cmp r10,0 ;если r9==0 end
    je .end
    cmp r10b, 0x30 ;проверка на то что цифра. то есть код меньше 9 и больше 0.
    jl .end     ;если не цифра, дальше не парсим.
    cmp r10b, 0x39
    jg .end
    mul r9 
    sub r10b, 0x30 ;получаем цифру
    add rax, r10 ;сдвигаем на рязряд и добавляем цифру
    inc rcx
    jmp .cycle

.end:
        mov rdx, rcx ;записываем в data register длину
        ret

; Принимает указатель на строку, пытается
; прочитать из её начала знаковое число.
; Если есть знак, пробелы между ним и числом не разрешены.
; Возвращает в rax: число, rdx : его длину в символах (включая знак, если он был) 
; rdx = 0 если число прочитать не удалось

;Сделано!
parse_int:
    xor rax, rax
    xor rdx, rdx
    mov rcx, rdi 
    cmp byte[rcx], '-'
    je .negative ;похожий алгоритм с распечатыванием - если не отрицательное, то просто парсим как беззнаковое
    jmp .positive
.negative:
        inc rcx ;тут вроде всё понятно, надеюсь...
        mov rdi, rcx
        push rcx
        call parse_uint
        pop rcx
        neg rax
        inc rdx 
        ret
.positive:
        mov rdi, rcx
        call parse_uint
        ret

; Принимает указатель на строку, указатель на буфер и длину буфера
; Копирует строку в буфер
; Возвращает длину строки если она умещается в буфер, иначе 0

;Сделано
string_copy:
    xor rax, rax; сохраняем все регистры с важной информацией
    xor rcx, rcx ;counter
    push rsi
    push rdi
    push rcx
    push rdx
    call string_length;узнаём длину строки
    pop rdx
    pop rcx
    pop rdi
    pop rsi
    mov r9, rax ;длина строки
    cmp rdx, r9 ;сверяем длину буфера и длину строки
    jl .exception
.cycle:
    cmp rcx, r9;
    jg .end
    mov r10,[rdi+rcx]
    mov [rsi+rcx], r10
    inc rcx; i++
    jmp .cycle
.exception:
    mov rax, 0 ;возвращаем 0
    ret
.end:
    mov rax, r9 ;возврат длины
    ret

;Я очень устал и хочу спать...