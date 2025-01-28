#include "kernel/types.h"
#include "kernel/riscv.h"
#include "kernel/defs.h"
#include "kernel/param.h"
#include "kernel/memlayout.h"
#include "kernel/spinlock.h"
#include "kernel/proc.h"
#include <stddef.h>

uint64
sys_calculate(void)
{
    int x=0;
    int y=0;
    char input_op[16];
    int temp_result;
    int* result;

    argint(0,&x);
    argint(1,&y);
    argstr(2, input_op, sizeof(input_op));
    argint(3, (int*)&result);
    
    if (strncmp(input_op, "+", 1) == 0) {   
        temp_result = x + y;
    } else if (strncmp(input_op, "-", 1) == 0) {
        temp_result = x - y;
    } else if (strncmp(input_op, "*", 1) == 0) {
        temp_result = x * y;
    } else if (strncmp(input_op, "/", 1) == 0) {
        if (y == 0) {
            return -1; // divide by zero error
        }
        temp_result = x / y;
    } else {
        return -1; // Unknown opcode provided or another error occured
    }
    
    copyout(myproc()->pagetable, (uint64)result, (char*)&temp_result, sizeof(int)); 

    return 0; // Success
}
