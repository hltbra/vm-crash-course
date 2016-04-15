#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <assert.h>

#define PUSH 0x1
#define POP 0x2
#define SUM 0x3
#define SUMX 0x4
#define PCALL 0x5
#define CALL 0x6
#define RET 0x7

#define PRIM_PRINT 255

#define MAX_STACK_SIZE 10000
#define MAX_PROGRAM_SIZE 100000
#define MAX_ENTRIES 10000


typedef struct {
    uint32_t *sp;
    uint32_t *ip;
    uint32_t *fp;
    uint32_t stack[MAX_STACK_SIZE];

    uint16_t num_entries;
    uint32_t entries[MAX_ENTRIES];
    uint32_t program[MAX_PROGRAM_SIZE];
    /*
    *
    * program: all instructions in memory (16 bits operator + 16 bits operand, concatenated)
    *
    * num_entries: number of routines in `program`
    *
    * entries: id and the address of all routines defined in `program`
    *
    */
} vm_t;



uint16_t left_16b(uint32_t *x) {
    return *(uint16_t *)x;
}

uint16_t right_16b(uint32_t *x) {
    return *(((uint16_t *)x) + 1);
}

/* primitive operations */

uint32_t prim_print(uint32_t argv[], uint32_t argc) {
    for (uint32_t i = 0 ; i < argc ; i++)
        putchar(argv[i]);
    return 0;
}


void push(vm_t *vm, uint32_t value) {
    *(vm->sp++) = value;
}

uint32_t pop(vm_t *vm) {
    return *(--vm->sp);
}

void sum(vm_t *vm) {
    push(vm, pop(vm) + pop(vm));
}

void sumx(vm_t *vm) {
    uint32_t sum = 0;
    uint32_t len = pop(vm);
    for (uint32_t i = 0 ; i < len; i++) {
        sum += pop(vm);
    }
    push(vm, sum);
}

void pcall(vm_t *vm, uint32_t id) {
    uint32_t argc = pop(vm);
    uint32_t argv[argc];
    for (uint32_t i = 0 ; i < argc ; i++) {
        argv[i] = pop(vm);
    }
    if (id == PRIM_PRINT) {
        push(vm, prim_print(argv, argc));
    }
}

void dump(vm_t *vm) {
    printf("[");
    for (uint32_t *i = vm->stack ; i < vm->sp ; i++) {
        printf("%d", *i);
        if (i < (vm->sp - 1)) {
            printf(", ");
        }
    }
    printf("]\n");
}

void init_vm(vm_t *vm) {
    vm->sp = vm->stack;
    vm->fp = vm->sp;
    vm->ip = NULL;
}

int find_routine(uint32_t id, vm_t *vm) {
    for (int i = 0 ; i < vm->num_entries ; i++) { 
        uint16_t routine_id = left_16b(vm->entries + i);
        uint16_t addr = right_16b(vm->entries + i);
        if (routine_id == id) {
            return addr;
        }
    }
    assert(0); // cant find routine
}

void read_program(const char *fname, vm_t *vm) {
    int fd = open(fname, O_RDONLY);

    read(fd, &vm->num_entries, sizeof(vm->num_entries));

    for (uint32_t i = 0 ; i < vm->num_entries; i++) {
        read(fd, (vm->entries + i), sizeof(vm->entries[0]));
    }

    for (uint32_t i = 0, instruction; read(fd, &instruction, sizeof(instruction)) > 0; i++) {
        vm->program[i] = instruction;
    }

    close(fd);
}

int stack_empty(vm_t *vm) {
    return vm->fp == vm->stack;
}

void start_program(vm_t *vm) {
    // routine 0 is the program entry point
    vm->ip = vm->program + find_routine(0, vm);
}

void save_state(vm_t *vm) {
    push(vm, vm->fp - vm->stack);
    push(vm, vm->ip - vm->program + 1);
    vm->fp = vm->sp;
}

void restore_state(vm_t *vm) {
    vm->sp = vm->fp;
    vm->ip = vm->program + pop(vm);
    vm->fp = vm->stack + pop(vm);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("ERROR: missing input file!\n");
        return 1;
    }
    vm_t vm;
    init_vm(&vm);

    read_program(argv[1], &vm);
    start_program(&vm);

    save_state(&vm);

    while (!stack_empty(&vm)) {
        uint16_t operator = left_16b(vm.ip);
        uint16_t operand = right_16b(vm.ip);

        if (operator == PUSH) {
            push(&vm, operand);
            vm.ip++;
        } else if (operator == POP) {
            pop(&vm);
            vm.ip++;
        } else if (operator == SUM) {
            sum(&vm);
            vm.ip++;
        } else if (operator == SUMX) {
            sumx(&vm);
            vm.ip++;
        } else if (operator == PCALL) {
            pcall(&vm, operand);
            vm.ip++;
        } else if (operator == CALL) {
            save_state(&vm);
            vm.ip = vm.program + find_routine(operand, &vm);
        } else if (operator == RET) {
            // this is the return value of the current routine
            uint32_t result = pop(&vm);
            restore_state(&vm);
            push(&vm, result);
        }
    }
    dump(&vm);

    return 0;
}
