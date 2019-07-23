# nom

crunch but the letter c

## using pure stack allocation

to put nom in no-heap mode, simply place this directly above the include
directive

```c
#define NOM_NOHEAP
```

when in no-heap mode, you initialize the buffer a little differently. here's
how you do it

```c
NomBuffer *buf = &(NomBuffer){};
```

aside from those differences, it's pretty much the exact same except with
a finite buffer size. however, you can increase this size by changing the
`NOM_MAX_BUFFER_SIZE` macro to a larger number (starts at 2000) like this

```c
#define NOM_MAX_BUFFER_SIZE 5000
```

## example

```c
#include <stdio.h>
#include "nom.h"

void print_char_array(int size, unsigned char *arr) {

	printf("[");
	for (int i = 0; i < size; i++) {

		printf("%d", arr[i]);
		if (i != size-1) {

			printf(" ");

		}

	}
	printf("]");

}

int main() {

	// create a container for the buffer
	NomBuffer *buf = malloc(sizeof(NomBuffer));

	// creates a new buffer with four zeroes
	nom_buffer_new(buf, 4);

	// write the byte `0x01` to the first offset, and move the offset forward one
	nom_buffer_writebytesnext(buf, 1, (unsigned char[]){0x01});

	// write the byte `0x01` to the second offset, and move the offset forward one
	nom_buffer_writebytesnext(buf, 1, (unsigned char[]){0x01});

	// seek the offset back one
	nom_buffer_seekbyte(buf, -1, 1);
	
	// write the bytes `0x02` and `0x03` to the second and third offsets, respectively
	nom_buffer_writebytesnext(buf, 2, (unsigned char[]){0x02, 0x03});

	// write the byte `0x04` to offset `0x03`
	nom_buffer_writebytes(buf, 0x03, 1, (unsigned char[]){0x04});

	// output the buffer's contents to the console
	print_char_array(buf->cap, buf->buf);

	// free the used memory
	nom_buffer_destroy(buf);

 	return 0;

}
```
