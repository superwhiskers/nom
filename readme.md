# nom

crunch but the letter c

## example

```c
#include <stdio.h>
#include "nom.h"

void print_char_array(int size, char *arr) {

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
	struct nom_minibuffer *buf = malloc(sizeof(struct nom_minibuffer));

	// creates a new buffer with four zeroes
	nom_minibuffer_new(buf, 4);

	// write the byte `0x01` to the first offset, and move the offset forward one
	nom_minibuffer_writebytesnext(buf, 1, (char[]){0x01});

	// write the byte `0x01` to the second offset, and move the offset forward one
	nom_minibuffer_writebytesnext(buf, 1, (char[]){0x01});

	// seek the offset back one
	nom_minibuffer_seekbyte(buf, -1, 1);
	
	// write the bytes `0x02` and `0x03` to the second and third offsets, respectively
	nom_minibuffer_writebytesnext(buf, 2, (char[]){0x02, 0x03});

	// write the byte `0x04` to offset `0x03`
	nom_minibuffer_writebytes(buf, 0x03, 1, (char[]){0x04});

	// output the buffer's contents to the console
	print_char_array(buf->cap, buf->buf);

	// free the used memory
	nom_minibuffer_destroy(buf);

  return 0;

}
```
