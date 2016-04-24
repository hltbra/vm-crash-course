# Machine #6

**Goal**: Extend [Machine 5](exercise_05.md) to perform better at calls.

### Instruction format

This machine introduces changes the semantics of `CALL`:


- `PUSH <ARG>`: 0x1
- `POP`:  0x2
- `SUM`:  0x3
- `SUMX`: 0x4
- `PCALL <ID>`: 0x5
- `CALL <addr>`: 0x6

  `CALL <addr>` transfers the execution to the user-defined subroutine in address `addr`. When the subroutine finishes execution, it's resulting value should be at the top of the stack.

- `RET`: 0x7
- `PUSH_ARG <ARG>`: 0x8

#### Instruction set

The same as `Machine #5`

### Machine memory

The same as `Machine #5`

### Execution

The same as `Machine #5`

### Programs


A program for `Machine #6` has similar format of programs for `Machine #4`
with two slight differences in the `header` and `body`.

#### header

The format of the header is:

```
[ main_addr: 16 bits]
```

The `main_addr` is the offset in the program where the main procedure starts -- the position of the `main` instruction (the first instruction of routine 0).

#### body

The `body` of a program is just a concatenation of user-defined subroutine
bodies -- a subroutine body is just a string of instructions.

All subroutine bodies should end with `RET <VAL>` instruction.

### Usage

Same as `Machine #5`.

### Loading programs

Programs are loaded by reading the `header` and looking for the `main` entry
in the object file. Upon finding such entry, the VM should start executing the
code pointed by the header's `main_addr`.

### Bonus

Create an assembler `a6`, in python, to build programs for `m6`. The assembler
should be able to create binary programs given a source code. The source code
is just a collection of subroutines whose instructions are per line. Example
of source code:

```
routine 7 { //  1+2
   push 1
   push 2
   sum
   ret
}

routine 0 { // print(7()+10)
  push 10
  call 7
  sum
  push 1
  pcall 255
  ret
}
```

Alternate syntax are welcome, if they could make parsing easier (these are not exercises on compilation :).

Assembled, the source code above should generate the following object file:

```
//header
0x0: 0x9  //main_addr
//body
0x1:  1 1    //push 1  -- begin subroutine 7
0x3:  1 2    //push 2
0x5:  3 0    //sum
0x7:  7 0    //ret
0x9:  1 10   //push 10 -- begin subroutine 0
0xB:  6 0x1  //call 0x1 -- calls subroutine 7
0xD:  3 0    //sum
0xF:  1 1    //push 1
0x11: 5 255  //pcall 255
0x13: 7 0    //ret
```

This program should print `13` and exit.
