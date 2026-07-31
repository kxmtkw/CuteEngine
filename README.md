# Cute (,,>﹏<,,)

**Cute** is a cute little language runtime that I am working on.

## Arch

The project is divided into two parts:
- Instr
- Engine
- Assembler

### `Instr`

Defines the core instruction set and acts as a mediator between the `Engine` and `Assembler`.

### `Engine`

The main runtime that executes the bytecode, written purely in C.

Some features of the runtime:
+ Register-based
+ Typed instruction set
+ Objects supported along with reference counting GC


### `Assembler`

A simple assembler that turns human readable instructions into bytecode. Written in C++.




