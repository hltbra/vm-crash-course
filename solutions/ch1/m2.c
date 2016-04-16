#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#define PUSH 0x1
#define POP 0x2
#define SUM 0x3
#define SUMX 0x4

#define MAX_STACK_SIZE 10000
#define MAX_PROGRAM_SIZE 100000


typedef struct {
    int i;
    uint8_t stack[MAX_STACK_SIZE];
} t_stack;


void push(t_stack *s, uint8_t value) {
    s->stack[s->i++] = value;
}

uint8_t pop(t_stack *s) {
    return s->stack[--s->i];
}

void init_stack(t_stack *s) {
    s->i = 0;
}

void sum(t_stack *s) {
    uint8_t x, y;
    push(s, pop(s) + pop(s));
}

void sumx(t_stack *s) {
    uint8_t sum = 0;
    uint8_t len = pop(s);
    for (int i = 0 ; i < len; i++) {
        sum += pop(s);
    }
    push(s, sum);
}


void dump(t_stack *s) {
    printf("[");
    for (int i = 0 ; i < s->i ; i++) {
        printf("%d", s->stack[i]);
        if (i < (s->i - 1)) {
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
        uint8_t instruction = (uint8_t)*ip;
        if (instruction == PUSH) {
            push(&s, *((uint8_t *)ip + 1));
        } else if (instruction == POP) {
            pop(&s);
        } else if (instruction == SUM) {
            sum(&s);
        } else if (instruction == SUMX) {
            sumx(&s);
        }
    }
    dump(&s);

    return 0;
}
