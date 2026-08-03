
# Stuff To Do

## Engine

### Modules

Implement the module system and modcalls. Static built-in modules will do for now. Some needed modules:

- `Sys` for controlling the engine.
	- Controlling the gc.
		- sys.gc.disable()
		- sys.gc.enable()
		- sys.gc.trigger()
	- Binary names, command line arguments, image file currently running, version.
	- Basically everything about the runtime that is controllable, can be controlled here.

- `Buffer` for byte manipulation. Internals will use this instead of string.
	- This will be used internally instead of string.

- `String` for character manipulation.
	- Strings are just buffers with extended methods.
	- Conversion between the two will result in a copy operation, 
	the reason for this is that I want strings to be mutable for performance. But if strings are mutable, it is possible for 
	threads to mess up strings as they get passed to module calls like file. This can result in bugs. The conversion won't be handled by the 
	engine but by the compiler.

- `Array` for fixed size arrays.
- `List` or `DyArray` for dynamically sized arrays.
	- Under the hood, they are just arrays, both will have exactly the same struct. Only difference being that Array might inline the data field to save a allocation call while list will always make a new allocation call to store the list data.

- `Map` for storing key-value pairs.
	- We can't really use buffers as keys here because that would lock down the map structure. Instead we just use uints as keys. 
	This means that the required type must implement an hash function but this is the compiler's responsibility.
	For now: uint -> atom is enough.

- `IO` for input/output.
	- Two primary methods, print and input needed. But I don't think direct implementation is the right choice here.
	- It maybe better to have methods and then have enums (0,1,2) for the three streams: stdout, stdin and stderr

- `File` for file IO.
	- File object needed.
	- Reading, writing, appending. We can borrow the operations C supports.

- `Filesystem` for filesystem manipulation.
	- Path handling
	- Directory creation, deletion, iteration.

- `Sockets` / `Requests` / `Http` for network based tasks.
	- Making http requests
	- Opening sockets


### Garbage Collection

Remove the current reference count approach with a tracing gc.
- Travel the entire callstack to find and mark objects.
- Each object also provides a `traverse` method to mark sub objects.
- This will obviously do stop-the-world pauses.
	- We can fix this by only triggering gc at high memory usage.
	- Allow the user to control the gc using the sys module


### New Memory Model

Current approach is naive and makes a lot of allocation calls.

#### `Current Approach`

- The manager allocates 64-slot bucket which can hold 64 object pointers.
- Each bucket is connected to the next via linked lists.
- Two stacks are maintained.
	- Buckets List (Full buckets)
	- Empty Buckets List (Partially Full buckets)
- If a bucket is full, push it to the full stack.
- If it becomes empty, push it back to the empty stack.

Advantages:

- No continuous array of objects required.
- Object allocation is still O(1) because we can pop an empty bucket instantly.

Disadvantages:

- Provides no boost to the actual object allocation. Each object still requires a malloc call.

#### `New Approach`

Instead of maintaining just two stacks. We maintain groups with predefined allocated slots.
- Bucket Stacks
	- 16 byte Bucket
		- 64 Slots of 16 bytes
	- 32 byte group
		- 64 Slots of 32 bytes
	- 64 byte group
		- ....
	- 128 byte group
		- ...
	- 256 byte group
		- ...

No empty bucket stack. There is just one bucket stack per group. Any partially filled bucket is instantly pushed to the top. 
If the top bucket is not empty, that means all buckets are already filled, thus a new bucket must be allocated.

Objects larger than 256 bytes are allocated on the heap, and their pointers are stored in buckets

Advantages:

- Reduced malloc calls
- Stack ensures empty bucket is always at the top
- Gives a lot more room to optimize like:
	- Can add a larger size group like 512 if required.
	- Can increase the bitmask from 64 to 128 by using two numbers instead of one.

Disadvantages:

- It becomes SUPER slow for GC to traverse the complicated object storage.
	- Different size groups
	- Each group has its own linked list.
	- Each node in the linked list carries 64 objects to check.
		- This can be solved by reducing the number of times that the GC is actually called.
