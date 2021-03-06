.globl put32
put32:
    str     w1, [x0]
    ret

.globl get32
get32:
    ldr     w0, [x0]
    ret

.globl delay
delay:
    subs    x0, x0, #1
    bne delay
    ret

.globl run_program
run_program:
    msr     elr_el1, x0

    //mov     x0, 0x3c0
    mov     x1, 0x340
    msr     spsr_el1, x1
    ldr     x0, =0x2000000
    msr     sp_el0, x0

    eret

.globl get_el
get_el:
    mrs x0, CurrentEL
    lsr x0, x0, #2
    ret