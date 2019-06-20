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

void nom_minibuffer_new(struct nom_minibuffer *out, int initial_size) {

	out->off = 0x00;
	out->cap = initial_size;
	out->boff = 0x00;
	out->bcap = initial_size*8;

	out->buf = malloc(initial_size*sizeof(char));

}

void nom_minibuffer_destroy(struct nom_minibuffer *b) {

	free(b->buf);
	free(b);

}

/* bitfield functions */

void nom_minibuffer_readbit(struct nom_minibuffer *b, char *out, int off) {

	*out = (b->buf[off/8] >> (7 - (off%8))) & 1;

}

void nom_minibuffer_readbitnext(struct nom_minibuffer *b, char *out) {

	nom_minibuffer_readbit(b, out, b->boff);
	nom_minibuffer_seekbit(b, 1, 1);

}

void nom_minibuffer_readbits(struct nom_minibuffer *b, int *out, int off, int n) {

	char bout;
	for (int i = 0; i < n; i++) {

		nom_minibuffer_readbit(b, &bout, off+i);
		*out = (*out << 1) | bout;

	}

}

void nom_minibuffer_readbitsnext(struct nom_minibuffer *b, int *out, int n) {

	nom_minibuffer_readbits(b, out, b->boff, n);
	nom_minibuffer_seekbit(b, n, 1);
	
}

void nom_minibuffer_setbit(struct nom_minibuffer *b, int off) {

	b->buf[off/8] |= (1 << (7-(off%8)));

}

void nom_minibuffer_setbitnext(struct nom_minibuffer *b) {

	nom_minibuffer_setbit(b, b->boff);
	nom_minibuffer_seekbyte(b, 1, 1);

}

void nom_minibuffer_clearbit(struct nom_minibuffer *b, int off) {

	b->buf[off/8] &= ~(1 << (7-(off%8)));

}

void nom_minibuffer_clearbitnext(struct nom_minibuffer *b) {

	nom_minibuffer_clearbit(b, b->boff);
	nom_minibuffer_seekbyte(b, 1, 1);

}

void nom_minibuffer_setbits(struct nom_minibuffer *b, int off, int data, int n) {

	for (int i = 0; i < n; i++) {

		if (((data>>(n-i-1))&1) == 0) {

			nom_minibuffer_clearbit(b, off + i);

		} else {

			nom_minibuffer_setbit(b, off + i);

		}

	}

}

void nom_minibuffer_setbitsnext(struct nom_minibuffer *b, int data, int n) {

	nom_minibuffer_setbits(b, b->boff, data, n);
	nom_minibuffer_seekbyte(b, n, 1);

}

void nom_minibuffer_flipbit(struct nom_minibuffer *b, int off) {

	b->buf[off/8] ^= (1 << (7-(off%8)));

}

void nom_minibuffer_flipbitnext(struct nom_minibuffer *b) {

	nom_minibuffer_flipbit(b, b->boff);
	nom_minibuffer_seekbit(b, 1, 1);

}

void nom_minibuffer_clearallbits(struct nom_minibuffer *b) {

	for (int i = 0; i < b->cap; i++) {

		b->buf[i] = 0x00;
		
	}

}

void nom_minibuffer_setallbits(struct nom_minibuffer *b) {

	for (int i = 0; i < b->cap; i++) {

		b->buf[i] = 0xff;

	}

}

void nom_minibuffer_flipallbits(struct nom_minibuffer *b) {

	for (int i = 0; i < b->cap; i++) {

		b->buf[i] = ~b->buf[i];

	}

}

void nom_minibuffer_seekbit(struct nom_minibuffer *b, int off, int relative) {

	if (relative < 0) {

		b->boff = off;

	} else if (relative >= 0) {

		b->boff += off;

	}

}

void nom_minibuffer_afterbit(struct nom_minibuffer *b, int *out, int off) {

	if (off < 0) {

		*out = b->bcap - b->boff - 1;

	} else if (off >= 0) {

		*out = b->bcap - off - 1;

	}

}

void nom_minibuffer_alignbit(struct nom_minibuffer *b) {

	b->boff = b->off*8;

}

/* bytebuffer functions */

void nom_minibuffer_writebytes(struct nom_minibuffer *b, int off, int data_length, char *data) {

	memcpy(b->buf+off, data, data_length*sizeof(char)); 
	
}

void nom_minibuffer_writebytesnext(struct nom_minibuffer *b, int n, char *data) {

	nom_minibuffer_writebytes(b, b->off, n, data);
	nom_minibuffer_seekbyte(b, n, 1);

}

void nom_minibuffer_readbytes(struct nom_minibuffer *b, char *out, int off, int n) {

	memcpy(out, b->buf+off, n*sizeof(char));

}

void nom_minibuffer_readbytesnext(struct nom_minibuffer *b, char *out, int n) {

	nom_minibuffer_readbytes(b, out, b->off, n);
	nom_minibuffer_seekbyte(b, n, 1);

}

void nom_minibuffer_seekbyte(struct nom_minibuffer *b, int off, int relative) {

	if (relative < 0) {

		b->off = off;

	} else if (relative >= 0) {

		b->off += off;

	}

}

void nom_minibuffer_afterbyte(struct nom_minibuffer *b, int *out, int off) {

	if (off < 0) {

		*out = b->cap - b->off - 1;

	} else if (off >= 0) {

		*out = b->cap - off - 1;

	}

}

void nom_minibuffer_alignbyte(struct nom_minibuffer *b) {

	b->off = b->boff/8;

}

/* generic functions */

void nom_minibuffer_grow(struct nom_minibuffer *b, int n) {

	char *old = b->buf;

	b->buf = malloc(sizeof(*old)+n);
	memcpy(b->buf, old, sizeof(*old));

	b->cap = b->cap + n;
	b->bcap = b->cap*8;

	free(old);

}

