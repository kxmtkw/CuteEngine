
# Cute Image

A `cute` image, represented by `.cute` files are compiled versions of `csm` files.

### Format

```
[Header]
	[Magic Number](32)
	[Version](32)
	[Procedure Count](32)
	[Instruction Count](32)
[Procedure Table]
	[Procedure]
		[Bytecode Index](32)
		[Arg Count](32)
	...
[Instruction Pool]
	[Byte]
	...
```

- The ID of a procedure corresponds to it's index in the procedure table.
- Version is define by lib CuteInstr and is used by both assembler and engine.