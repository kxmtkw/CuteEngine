
# Module: Buffer

Module for byte manipulation. Provides the `Buffer` object which allows the engine to manipulate individual bytes.

A buffer is explicitly resized meaning that it's size won't automatically change if more space is required. It must be explicitly resized.

|                  |           |
| --------         | --------  |
| Module id        | 0         | 
| Internal Repr    | "buffer"  |
| Info             | Byte Manipulation |
| Method Count     | 10        |
| Depends on       | _         |

## Methods


### `0` new
- Create a new buffer object with the given size.
- *Arguments* -> 1
	- `uint` -> Size of the buffer.
- *Returns*
	- `buffer-object` -> New buffer 


### `1` resize

- Resize a buffer object. Truncates if the new size is smaller.
- *Arguments* -> 2
	- `buffer object` -> Target buffer instance.
	- `uint` -> New size in bytes.
- *Returns*
	- `uint` -> New size


### `2` get
- Retrieve a byte at a specific index from the buffer.
- *Arguments* -> 2
	- `buffer object` -> Target buffer instance.
	- `uint` -> Zero-based byte index.
- *Returns*
	- `uint` -> Byte at specified index. 


### `3` set
- Set a byte at a specific index in the buffer.
- *Arguments* -> 3
	- `buffer object` -> Target buffer instance.
	- `uint` -> Zero-based byte index.
	- `uint` -> Byte value to write.
- *Returns*
	- `uint` -> The byte value 


### `4` setBuffer
- Copy another buffer's contents into the target buffer starting at an index.
- *Arguments* -> 3
	- `buffer object` -> Target destination buffer instance.
	- `uint` -> Starting destination index.
	- `buffer object` -> Source buffer instance.
- *Returns*
	- `uint` -> The index in the target buffer upto which the data was written.


### `5` fill
- Set every byte in the buffer to a specific value.
- *Arguments* -> 2
	- `buffer object` -> Target buffer instance.
	- `uint` -> Byte value to fill.
- *Returns*
	- `uint` -> Amount of bytes written (aka the size of the buffer.)


### `6` clear
- Clear the data in a buffer by setting all bytes to zero.
- *Arguments* -> 1
	- `buffer object` -> Target buffer instance.
- *Returns*
	- `uint` -> Amount of bytes cleared (aka the size of the buffer.) 


### `7` extend
- Append another buffer to the target buffer, automatically resizing it to fit.
- *Arguments* -> 2
	- `buffer object` -> Target destination buffer instance.
	- `buffer object` -> Source buffer instance to append.
- *Returns*
	- `uint` -> New size of the target buffer. 


### `8` slice
- Slice a sub-buffer from a buffer.
- *Arguments* -> 3
	- `buffer object` -> Target destination buffer instance.
	- `uint` -> Index to start the slice from.
	- `uint` -> Length of the slice.
- *Returns*
	- `buffer-object` -> Sliced buffer.

### `9` copy
- Create a copy of a buffer.
- *Arguments* -> 1
	- `buffer object` -> Source buffer to copy from.
- *Returns*
	- `buffer-object` -> Copied buffer.