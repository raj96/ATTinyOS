.equ STACK_SIZE, 128
.global STACK_SIZE

.equ HEAP_SIZE, 64
.global HEAP_SIZE

.equ MAX_TASK, 4
.global MAX_TASK

.extern kmain
.extern stack_start

.equ SPH, 0x3e
.equ SPL, 0x3d

.equ CLKPR, 0x26

.section .text
.global init

init:
    ; Remove clock division, if any
    ; ldi r16, 128
    ; out CLKPR, r16
    ; ldi r16, 0
    ; out CLKPR, r16

    ; Clear r1 and setup stack
    clr r1
    ldi r16, hi8(stack_start)
    out SPH, r16
    ldi r16, lo8(stack_start)
    out SPL, r16

    rjmp kmain
