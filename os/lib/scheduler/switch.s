.extern current_task
.extern cycle_count
.extern switch_at

.equ R0_OFFSET, 0
.equ R_OFFSET, 1
.equ YL_OFFSET, 27
.equ YH_OFFSET, 28
.equ ZL_OFFSET, 29
.equ ZH_OFFSET, 30
.equ PCH_OFFSET, 31
.equ PCL_OFFSET, 32
.equ SPH_OFFSET, 33
.equ SPL_OFFSET, 34
.equ SREG_OFFSET, 35

.equ SPL, 0x3D
.equ SPH, 0x3E
.equ SREG, 0x3F
.equ TCNT0, 0x32

.global c_store
.global c_load
.global load_sp_and_pc

; Task_t structure
; r0                1 byte
; r2 - r31          30 bytes
; pch - 1 byte     pcl - 1 byte
; sph - 1 byte     spl - 1 byte
; sreg - 1 byte
; queued : paused : stopped - 6 bits

; 100 cycles
c_store:
    ; Storing the current SREG in r1
    ; So it does not get affected by other operations
    in r1, SREG
    push r1

    ; Push the current ZH & ZL registers
    push ZL
    push ZH

    ; Load switch_at
    ldi ZH, hi8(switch_at)
    ldi ZL, lo8(switch_at)
    ld r1, Z

    ; Load cycle_count
    ldi ZH, hi8(cycle_count)
    ldi ZL, lo8(cycle_count)
    ld ZH, Z

    ; Compare cycle_count and switch_at
    cp r1, ZH
    breq store_regs

    ; Pop stuff and return
    pop r1
    pop r1
    pop r1

    eor r1, r1
    ret

store_regs:
    ; Caluclate the SP for the task
    in ZH, SPH
    in ZL, SPL 
    sbiw Z, 7

    ; Push the current YH & YL registers
    push YL
    push YH

    ; Push SPH and SPL of task
    push ZL
    push ZH

    ldi YH, hi8(current_task)
    ldi YL, lo8(current_task)

    ld ZL, Y+
    ld ZH, Y

    st Z+, r0
    st Z+, r2
    st Z+, r3
    st Z+, r4
    st Z+, r5
    st Z+, r6
    st Z+, r7
    st Z+, r8
    st Z+, r9
    st Z+, r10
    st Z+, r11
    st Z+, r12
    st Z+, r13
    st Z+, r14
    st Z+, r15
    st Z+, r16
    st Z+, r17
    st Z+, r18
    st Z+, r19
    st Z+, r20
    st Z+, r21
    st Z+, r22
    st Z+, r23
    st Z+, r24
    st Z+, r25
    st Z+, r26
    st Z+, r27

    ; Move to PCH in the current_task of type Task_t
    adiw Z, 4

    ; (TOP-OF-STACK) | Task SPH | Task SPL | YH | YL | ZH | ZL | SREG | Caller PCH | Caller PCL | Task PCH | Task PCL | ...
    ; We will move 10 & 11 spaces back to fetch the PC of the task
    ; when it was switched
    in YH, SPH
    in YL, SPL

    sbiw Y, 10

    ; PCH -> r1 and PCL -> r0
    ld r1, Y
    ld r0, -Y

    st Z+, r1
    st Z+, r0

    ; SPH -> r1 and SPL -> r0
    pop r1
    pop r0
    st Z+, r1
    st Z+, r0

    ldi YH, hi8(current_task)
    ldi YL, lo8(current_task)

    ld ZL, Y+
    ld ZH, Y
    ; Pop off the index register values that were pushed onto the stack
    ; YH
    pop r1
    std Z + YH_OFFSET, r1
    ; YL
    pop r1
    std Z + YL_OFFSET, r1

    ; Move address of current_task from Z to Y
    movw r28, r30

    ; ZH
    pop r1
    std Y + ZH_OFFSET, r1
    ; ZL
    pop r1
    std Y + ZL_OFFSET, r1

    ; SREG
    pop r1
    std Y + SREG_OFFSET, r1

    ; Set r1 back to 0 as per avr-gcc ABI
    eor r1, r1

    ret

c_load:
    ; We are not going to return to the caller
    ; Instead we will jump to the task directly after
    ; setting up the registers
    pop r1
    pop r1

    ; Popping the PC of the previous task
    pop r1
    pop r1

    ldi YH, hi8(current_task)
    ldi YL, lo8(current_task)

    ld ZL, Y+
    ld ZH, Y

    ld r0, Z+
    ld r2, Z+
    ld r3, Z+
    ld r4, Z+
    ld r5, Z+
    ld r6, Z+
    ld r7, Z+
    ld r8, Z+
    ld r9, Z+
    ld r10, Z+
    ld r11, Z+
    ld r12, Z+
    ld r13, Z+
    ld r14, Z+
    ld r15, Z+
    ld r16, Z+
    ld r17, Z+
    ld r18, Z+
    ld r19, Z+
    ld r20, Z+
    ld r21, Z+
    ld r22, Z+
    ld r23, Z+
    ld r24, Z+
    ld r25, Z+
    ld r26, Z+
    ld r27, Z+
    ld r28, Z+
    ld r29, Z+

    ; Jump to the SPL in Task_t
    adiw Z, 5

    ; Load SPL of the task and place it in the actual SPL
    ld r1, Z
    out SPL, r1
    ; Load SPH of the task and place it in the actual SPH
    ld r1, -Z
    out SPH, r1

    ; Load PCL of the task
    ld r1, -Z
    push r1
    ; Load PCH of the task
    ld r1, -Z
    push r1

    ; Store a 0 on stack to restore it to r1 w/o changing the Z bit in SREG
    eor r1, r1
    push r1

    ; Jump to the SREG in Task_t
    adiw Z, 4
    ; Load the SREG from Task_t and push it onto the stack
    ld r1, Z
    push r1

    ; Jump to Z in Task_t
    sbiw Z, 6
    ; Load the Z from Task_t and push it onto the stack
    ld r1, Z+
    push r1
    ld r1, Z
    push r1

    ; Stack looks like
    ; (Top of the stack) | ZH | ZL | SREG | 0 | PCH | PCL | ...

    pop r1
    mov ZH, r1
    pop r1
    mov ZL, r1
    pop r1
    out SREG, r1

    ; Set r1 to 0 as per avr-gcc ABI
    pop r1

    out TCNT0, 0x00
    nop
    reti

load_sp_and_pc:
    ldi YH, hi8(current_task)
    ldi YL, lo8(current_task)

    ld ZL, Y+
    ld ZH, Y+

    ldd r0, Z + SPH_OFFSET
    out SPH, r0
    ldd r0, Z + SPL_OFFSET
    out SPL, r0

    ldd r0, Z + PCL_OFFSET
    push r0
    ldd r0, Z + PCH_OFFSET
    push r0

    out SREG, 0

    reti
