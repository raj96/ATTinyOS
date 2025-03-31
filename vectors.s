.extern init
.extern timer0_ovf_isr

.section .vectors
    rjmp init   ; RESET handler
    rjmp init   ; External
    rjmp init   ; Pin Change
    rjmp init   ; TC1 CMA
    rjmp init   ; TC1 OVF
    rjmp init   ; TC0 OVF 
    rjmp init   ; RESET handler
    rjmp init   ; External
    rjmp init   ; Pin Change
    rjmp init   ; TC1 CMA
    rjmp init   ; TC1 OVF
    rjmp init   ; TC0 OVF 
    rjmp init   ; TC1 CMA
    rjmp init   ; TC1 OVF
    rjmp init   ; TC0 OVF 
