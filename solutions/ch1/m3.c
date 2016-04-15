#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#define PUSH 0x1
#define POP 0x2
#define SUM 0x3
#define SUMX 0x4
#define PCALL 0x5

#define PRIM_PRINT 255

#define MAX_STACK_SIZE 10000
#define MAX_PROGRAM_SIZE 100000


typedef struct {
    int sp;
    uint8_t stack[MAX_STACK_SIZE];
} t_stack;



/* primitive operations */

uint8_t prim_print(uint8_t argv[], uint8_t argc) {
    for (uint8_t i = 0 ; i < argc ; i++)
        putchar(argv[i]);
    return 0;
}


void push(t_stack *s, uint8_t value) {
    s->stack[s->sp++] = value;
}

uint8_t pop(t_stack *s) {
    return s->stack[--s->sp];
}

void init_stack(t_stack *s) {
    s->sp = 0;
}

void sum(t_stack *s) {
    uint8_t x, y;
    push(s, pop(s) + pop(s));
}

void sumx(t_stack *s) {
    uint8_t sum = 0;
    uint8_t len = pop(s);
    for (uint8_t i = 0 ; i < len; i++) {
        sum += pop(s);
    }
    push(s, sum);
}

void pcall(t_stack *s, uint8_t id) {
    uint8_t argc = pop(s);
    uint8_t argv[argc];
    for (uint8_t i = 0 ; i < argc ; i++) {
        argv[i] = pop(s);
    }
    if (id == PRIM_PRINT) {
        push(s, prim_print(argv, argc));
    }
}

void dump(t_stack *s) {
    printf("[");
    for (uint8_t i = 0 ; i < s->sp ; i++) {
        printf("%d", s->stack[i]);
        if (i < (s->sp - 1)) {
            printf(", ");
        }
    }
    printf("]\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("ERROR: missing input file!\n");
        return 1;
    }
    t_stack s;
    init_stack(&s);

    uint16_t program[MAX_PROGRAM_SIZE], instruction;

    int fd = open(argv[1], O_RDONLY);
    int i = 0;
    while (read(fd, &instruction, 2) > 0) {
        program[i++] = instruction;
    }
    // indicates end of program
    program[i] = 0;

    for (uint16_t *ip = program ; *ip != 0 ; ip++) {
        uint8_t operator = (uint8_t)*ip;
        uint8_t operand = *((uint8_t *)ip + 1);

        if (operator == PUSH) {
            push(&s, operand);
        } else if (operator == POP) {
            pop(&s);
        } else if (operator == SUM) {
            sum(&s);
        } else if (operator == SUMX) {
            sumx(&s);
        } else if (operator == PCALL) {
            pcall(&s, operand);
        }
    }
    dump(&s);

    return 0;
}
