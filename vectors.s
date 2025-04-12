.extern init
.extern timer0_ovf_isr

.section .vectors
    rjmp init   ; RESET handler
    rjmp init   ; External
    rjmp init   ; Pin Change
    rjmp init   ; TC1 CMA
    rjmp init   ; TC1 OVF
    rjmp timer0_ovf_isr   ; TC0 OVF 
    rjmp init   ; EEPROM ready
    rjmp init   ; Analog comparator
    rjmp init   ; ADC Conversion complete
    rjmp init   ; TC1 CMB
    rjmp init   ; TC0 CMA
    rjmp init   ; TC0 CMB 
    rjmp init   ; WDT
    rjmp init   ; USI Start
    rjmp init   ; USI OVF
