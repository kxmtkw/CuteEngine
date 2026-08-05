
# Module: Buffer

Module for byte manipulation. Provides the `Buffer` object which allows the engine to manipulate individual bytes.

A buffer is explicitly resized meaning that it's size won't automatically change if more space is required. It must be explicitly resized.

|                  |           |
| --------         | --------  |
| Module id        | 0         | 
| Internal Repr    | "buffer"  |
| Info             | Byte Manipulation |
| Method Count     | 18        |
| Depends on       | _         |

## Methods


### `0` new
- Create a new buffer object with the given size. The capacity exactly fits the initial size specified. The buffer bytes are set to 0.
- *Arguments* -> 1
	- `uint` -> Size of the buffer.
- *Returns*
	- `buffer-object` -> New buffer.

### `1` size
- Get the current size of the buffer
- *Arguments* -> 1
	- `buffer-object` -> Buffer.
- *Returns*
	- `uint` -> Size. 

### `2` capacity
- Get the current capacity of the buffer
- *Arguments* -> 1
	- `buffer-object` -> Buffer.
- *Returns*
	- `uint` -> Capacity. 

### `3` resize
- Resize a buffer object. Truncates if the new size is smaller. Capacity is only changed if new size is greater than current capacity. 
If that is the case, the new capacity matches the required size.
- *Arguments* -> 1
	- `buffer-object` -> Buffer.
	- `uint` -> New size in bytes.
- *Returns*
	- `none`

### `4` reserve
- Reserve capacity for the buffer. Skips if the new capacity is less then the current one.
- *Arguments* -> 1
	- `buffer-object` -> Buffer.
	- `uint` -> New size in bytes.
- *Returns*
	- `none`

### `5` truncate
- Truncate the buffer capacity so it fits the size.
- *Arguments* -> 1
	- `buffer-object` -> Buffer.
- *Returns*
	- `none`

### `6` get
- Retrieve a byte at a specific index from the buffer.
- *Arguments* -> 2
	- `buffer object` -> Target buffer instance.
	- `uint` -> Zero-based byte index.
- *Returns*
	- `uint` -> Byte at specified index. 

### `7` set
- Set a byte at a specific index in the buffer.
- *Arguments* -> 3
	- `buffer object` -> Target buffer instance.
	- `uint` -> Zero-based byte index.
	- `uint` -> Byte value to write.
- *Returns*
	- `none`

### `8` set-buffer
- Copy another buffer's contents into the target buffer starting at an index.
- *Arguments* -> 3
	- `buffer object` -> Target destination buffer instance.
	- `uint` -> Starting destination index.
	- `buffer object` -> Source buffer instance.
- *Returns*
	- `uint` -> The index in the target buffer upto which the data was written.

### `9` push
- Push a byte to the end of the buffer.
- *Arguments* -> 2
	- `buffer object` -> Buffer
	- `byte` -> Byte to append.
- *Returns*
	- `none`

### `10` append
- Append another buffer to the target buffer, automatically resizing it to fit.
- *Arguments* -> 2
	- `buffer object` -> Target destination buffer instance.
	- `buffer object` -> Source buffer instance to append.
- *Returns*
	- `none`

### `11` slice
- Slice a sub-buffer from a buffer.
- *Arguments* -> 3
	- `buffer object` -> Target destination buffer instance.
	- `uint` -> Index to start the slice from.
	- `uint` -> Length of the slice.
- *Returns*
	- `buffer-object` -> Sliced buffer.

### `12` copy
- Create a copy of a buffer.
- *Arguments* -> 1
	- `buffer object` -> Source buffer to copy from.
- *Returns*
	- `buffer-object` -> Copied buffer.


### `13` fill
- Set every byte in the buffer to a specific value.
- *Arguments* -> 2
	- `buffer object` -> Target buffer instance.
	- `uint` -> Byte value to fill.
- *Returns*
	- `none`


### `14` clear
- Clear the data in a buffer by setting all bytes to zero.
- *Arguments* -> 1
	- `buffer object` -> Target buffer instance.
- *Returns*
	- `none`


### `15` find-byte
- Find a byte inside the buffer. Returns the first index found.
- *Arguments* -> 3
	- `buffer object` -> Target buffer instance.
	- `uint` -> Starting index
	- `byte` -> The byte to locate.
- *Returns*
	- `uint` -> The index of the byte. UINT32_MAX (or -1) if byte not found. **STILL NEEDS UPDATING** 


### `16` find-buffer
- Find a sub buffer inside the buffer. Returns the first index found.
- *Arguments* -> 3
	- `buffer object` -> Target buffer instance.
	- `uint` -> Starting index.
	- `buffer object` -> The sub buffer to locate.
- *Returns*
	- `uint` -> The index of the byte. UINT32_MAX (or -1) if byte not found. **STILL NEEDS UPDATING** 


### `17` equals
- Check if two buffers are equal. For two buffers to be equal:
	1. They must have the same size.
	2. All bytes must be equal.
- *Arguments* -> 2
	- `buffer object` -> Buffer.
	- `buffer object` -> Buffer to compare against.
- *Returns*
	- `bool` -> Whether they are equal or not.