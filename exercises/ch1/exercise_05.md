# Machine #5

**Goal**: Extend [Machine 4](exercise_04.md) to support user-defined routines with parameters.

### Instruction format

(The same as `Machine #4`)

Let:

- Word size: `sizeof(void*)` bits
- operation: `sizeof(void*)/2` bits
- operand: `sizeof(void*)/2` bits

Instruction format: `[ operation | operand ]`

All instructions are `sizeof(void*)` bits long, even if the operation does not have arguments (in that case, operand should be ignored).

#### Instruction set

This machine introduces `PUSH_ARG` opcode:


- `PUSH <ARG>`: 0x1
- `POP`:  0x2
- `SUM`:  0x3
- `SUMX`: 0x4
- `PCALL <ID>`: 0x5
- `CALL <ID>`: 0x6
- `RET`: 0x7
- `PUSH_ARG <ARG>`: 0x8

    this operation pushes the argument `<ARG>` to the top of the stack, where `<ARG>` refers to the `n-th`
    argument passed to the current subroutine via stack in the reverse order they were introduced on the it.

    For example, given the stack `[..., 7, 10]` right before `CALL`ing a subroutine of arity 2,
    `PUSH_ARG 0` should push 10 to the stack, and `PUSH_ARG 1` should push 7.

    Notice that at any point of a subroutine body, where it could already have pushed some values to operate on,
    `PUSH_ARG` will still refer to the arguments. For example, if `[..., 7, 10]` is the stack before `CALL`ing a subroutine `X`, that subroutine might `PUSH 100`, and later decide to ``PUSH_ARG 1`. In this case, the result stacking should be `[..., 7, 10, 100, 7]`.

### Machine memory

```
[ ........ program code ...... program stack ....] // memory
             ^                      ^      ^
             IP                     FP     SP
```


Same as `Machine #4`


### Execution

The activation record format should be: `[... prog_data, args, FP, ret_addr, prog_data ...]`, where:

- `args` are the values `PUSH`ed by the call site.
- `FP` is the current value stored in `FP`.
- `ret_addr` is a pointer to the next instruction of the call site.
- `prog_data` is all data a subroutine pushes to operate on.

For example, consider the following code containing a call to some subroutine `X` (as in `X(10, 20)`) and assume routine X executes `PUSH_ARG 0` and `PUSH_ARG 1`:

```
0xA: PUSH 10
0xB: PUSH 20
0xC: CALL X
0xD: PUSH 40
```

Upon executing `CALL X`:

- The value in `FP` should be pushed on the stack.
- `FP` should be set to `SP` of the call site
- the address of the next instruction (`0xD`) should be pushed on the stack.

Then, the stack contents should be:

```
0x03: ...                  //prog_data
0x04: 10                   //arg#1
0x05: 20                   //arg#2
0x06: <previous FP value>  //FP
0x07: 0xD                  //ret_addr
```

And the register contents should be:

```
FP = 0x06 //previous FP
SP = 0x07
```

In this way, `FP` always points to the previous `FP` (recursively), `FP-n`
points to the `n-th` argument passed by the caller, and `FP+1` points to the
next instruction of the call site.

When `RET` is executed, `FP` should be used to:

- reset `IP` to the address of the next instruction to be executed using `FP` (ie. `0xD`)
- clear the stack from local data pushed during `X` execution, reseting `SP` to it's previous value (which is `FP-1`).
- reset the previous value of `FP`.

### Programs

A program for `Machine #5` is the same as for `Machine #4`.

### Usage

Same as `Machine #4`.

### Loading programs

Same as `Machine #4`.
