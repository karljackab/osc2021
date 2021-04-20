#include "mini_uart.h"
#include "stdint.h"
#include "str_tool.h"


void print_state(){
    uint64_t spsr, elr, esr;
    asm volatile("mrs %0, spsr_el1 \n":"=r"(spsr):);
    asm volatile("mrs %0, elr_el1 \n":"=r"(elr):);
    asm volatile("mrs %0, esr_el1 \n":"=r"(esr):);

    uart_puts("spsr_el1 = ");
    uart_puts(itoa(spsr, 16));
    uart_puts("\r\nelr_el1 = ");
    uart_puts(itoa(elr, 16));
    uart_puts("\r\nesr_el1 = ");
    uart_puts(itoa(esr, 16));
    uart_puts("\r\n");
    
    asm volatile("mov x3, 1 \n");   // enable time interrupt
    asm volatile("msr cntp_ctl_el0, x3 \n");
}

void lowerSync64_handler(){
    print_state();
}

void lowerIRQ64_handler(){
    asm volatile("mrs x0, cntfrq_el0 \n");
    asm volatile("add x0, x0, x0 \n");
    asm volatile("msr cntp_tval_el0, x0 \n");
    
    uint64_t cntpct, cntfrq;
    asm volatile("mrs %0, cntpct_el0 \n":"=r"(cntpct):);
    asm volatile("mrs %0, cntfrq_el0 \n":"=r"(cntfrq):);

    uint64_t tmp = cntpct*10/cntfrq;
    uart_puts("\r\n------------\r\n");
    uart_puts("Time: ");
    uart_puts(itoa(tmp/10, 10));
    uart_puts(".");
    uart_puts(itoa(tmp%10, 10));
    uart_puts("\r\n------------\r\n");
}

extern char nextTimeoutMessage[1000];
void curIRQ_handler(){
    uart_puts(nextTimeoutMessage);
    uart_puts("\r\n");
    asm volatile("mov x0, 0 \n");
    asm volatile("msr cntp_ctl_el0, x0 \n");
}

void non_implement_handler(){
    uart_puts("This instruction has not been implemented!\r\n");
    while(1);
}