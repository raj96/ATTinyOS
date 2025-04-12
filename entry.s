.extern kmain
.extern stack_start

.equ SPH, 0x3e
.equ SPL, 0x3d

.equ CLKPR, 0x26

.section .text
.global init

init:
    ; Remove clock division, if any
    ldi r16, 128
    ldi r17, 0
    out CLKPR, r16
    out CLKPR, r17

    nop

    ; Clear r1 and setup stack
    clr r1
    ldi r16, hi8(stack_start)
    out SPH, r16
    ldi r16, lo8(stack_start)
    out SPL, r16

    nop

    rjmp kmain
