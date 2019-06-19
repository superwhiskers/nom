/*

nom - crunch but the letter c
copyright (c) 2019 superwhiskers <whiskerdev@protonmail.com>

this program is free software: you can redistribute it and/or modify
it under the terms of the gnu lesser general public license as published by
the free software foundation, either version 3 of the license, or
(at your option) any later version.

this program is distributed in the hope that it will be useful,
but without any warranty; without even the implied warranty of
merchantability or fitness for a particular purpose.  see the
gnu lesser general public license for more details.

you should have received a copy of the gnu lesser general public license
along with this program.  if not, see <https://www.gnu.org/licenses/>.

*/

#include "minibuf.h"

/* creation and destruction functions */

void crunch_minibuffer_new(struct crunch_minibuffer *out, int initial_size) {

	out->off = 0x00;
	out->cap = initial_size;
	out->boff = 0x00;
	out->bcap = initial_size*8;

	out->buf = malloc(initial_size*sizeof(char));

}

void crunch_minibuffer_destroy(struct crunch_minibuffer *b) {

	free(b->buf);
	free(b);

}

/* bitfield functions */

void crunch_minibuffer_readbit(struct crunch_minibuffer *b, char *out, int off) {

	*out = (b->buf[off/8] & (1 << (7-(off%8)))) != 0;

}

void crunch_minibuffer_readbits(struct crunch_minibuffer *b, int *out, int off, int n) {

	char bout;
	for (int i = 0; i < n; i++) {

		crunch_minibuffer_readbit(b, &bout, off+i);
		*out = (*out << 1) | bout;

	}

}

void crunch_minibuffer_setbit(struct crunch_minibuffer *b, int off, char data) {

	switch (data) {

	case 0:
		b->buf[off] &= ~(1 << (7-(off%8)));

	case 1:
		b->buf[off] |= (1 << (7-(off%8)));

	}

}

void crunch_minibuffer_setbits(struct crunch_minibuffer *b, int off, int data, int n) {

	for (int i = 0; i < n; i++) {

		crunch_minibuffer_setbit(b, off+i, (data >> (n-i-1))&1);

	}

}

void crunch_minibuffer_flipbit(struct crunch_minibuffer *b, int off) {

	b->buf[off/8] ^= (1 << (7-(off%8)));

}

void crunch_minibuffer_clearallbits(struct crunch_minibuffer *b) {

	for (int i = 0; i < b->cap; i++) {

		b->buf[i] = 0x00;
		
	}

}

void crunch_minibuffer_setallbits(struct crunch_minibuffer *b) {

	for (int i = 0; i < b->cap; i++) {

		b->buf[i] = 0xff;

	}

}

void crunch_minibuffer_flipallbits(struct crunch_minibuffer *b) {

	for (int i = 0; i < b->cap; i++) {

		b->buf[i] = ~b->buf[i];

	}

}

/* if relative < 0, seek to offset off, if relative >= 0, seek foward off bits */
void crunch_minibuffer_seekbit(struct crunch_minibuffer *b, int off, int relative) {

	if (relative < 0) {

		b->boff = off;

	} else if (relative >= 0) {

		b->boff += off;

	}

}

/* if off < 0, provide the number of bits after the internal offset, if off >= 0, provide the number of bits after the provided offset */
void crunch_minibuffer_afterbit(struct crunch_minibuffer *b, int *out, int off) {

	if (off < 0) {

		*out = b->bcap - b->boff - 1;

	} else if (off >= 0) {

		*out = b->bcap - off - 1;

	}

}

/* bytebuffer functions */

void crunch_minibuffer_write(struct crunch_minibuffer *b, int off, int data_length, char *data) {

	memcpy(b->buf+off, data, data_length*sizeof(char)); 
	
}

void crunch_minibuffer_read(struct crunch_minibuffer *b, char *out, int off, int n) {

	memcpy(out, b->buf+off, n*sizeof(char));

}

/* if relative < 0, seek to offset off, if relative >= 0, seek foward off bytes */
void crunch_minibuffer_seek(struct crunch_minibuffer *b, int off, int relative) {

	if (relative < 0) {

		b->off = off;

	} else if (relative >= 0) {

		b->off += off;

	}

}

/* if off < 0, provide the number of bytes after the internal offset, if off >= 0, provide the number of bytes after the provided offset */
void crunch_minibuffer_after(struct crunch_minibuffer *b, int *out, int off) {

	if (off < 0) {

		*out = b->cap - b->off - 1;

	} else if (off >= 0) {

		*out = b->cap - off - 1;

	}

}

/* generic functions */

void crunch_minibuffer_grow(struct crunch_minibuffer *b, int n) {

	char *old = b->buf;

	b->buf = malloc(sizeof(*old)+n);
	memcpy(b->buf, old, sizeof(*old));

	b->cap = b->cap + n;
	b->bcap = b->cap*8;

	free(old);

}

void crunch_minibuffer_alignbit(struct crunch_minibuffer *b) {

	b->boff = b->off*8;

}

void crunch_minibuffer_alignbyte(struct crunch_minibuffer *b) {

	b->off = b->boff/8;

}

/* *next variants of some functions */

void crunch_minibuffer_flipbitnext(struct crunch_minibuffer *b) {

	crunch_minibuffer_flipbit(b, b->boff);
	crunch_minibuffer_seekbit(b, 1, 1);

}

void crunch_minibuffer_readbitnext(struct crunch_minibuffer *b, char *out) {

	crunch_minibuffer_readbit(b, out, b->boff);
	crunch_minibuffer_seekbit(b, 1, 1);

}

void crunch_minibuffer_readbitsnext(struct crunch_minibuffer *b, int *out, int n) {

	crunch_minibuffer_readbits(b, out, b->boff, n);
	crunch_minibuffer_seekbit(b, n, 1);
	
}

void crunch_minibuffer_readnext(struct crunch_minibuffer *b, char *out, int n) {

	crunch_minibuffer_read(b, out, b->off, n);
	crunch_minibuffer_seek(b, n, 1);

}

void crunch_minibuffer_setbitnext(struct crunch_minibuffer *b, char data) {

	crunch_minibuffer_setbit(b, b->boff, data);
	crunch_minibuffer_seek(b, 1, 1);

}

void crunch_minibuffer_setbitsnext(struct crunch_minibuffer *b, int data, int n) {

	crunch_minibuffer_setbits(b, b->boff, data, n);
	crunch_minibuffer_seek(b, n, 1);

}

void crunch_minibuffer_writenext(struct crunch_minibuffer *b, int n, char *data) {

	crunch_minibuffer_write(b, b->off, n, data);
	crunch_minibuffer_seek(b, n, 1);

}
