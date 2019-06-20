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

#include "nom.h"

/* creation and destruction functions */

void nom_buffer_new(struct nom_buffer *out, long long int initial_size) {

	out->off = 0x00;
	out->cap = initial_size;
	out->boff = 0x00;
	out->bcap = initial_size*8;

	out->buf = malloc(initial_size*sizeof(char));

}

void nom_buffer_destroy(struct nom_buffer *b) {

	free(b->buf);
	free(b);

}

/* bitfield functions */

void nom_buffer_readbit(struct nom_buffer *b, char *out, long long int off) {

	*out = (b->buf[off/8] >> (7 - (off%8))) & 1;

}

void nom_buffer_readbitnext(struct nom_buffer *b, char *out) {

	nom_buffer_readbit(b, out, b->boff);
	nom_buffer_seekbit(b, 1, 1);

}

void nom_buffer_readbits(struct nom_buffer *b, unsigned long long int *out, long long int off, long long int n) {

	char bout;
	for (int i = 0; i < n; i++) {

		nom_buffer_readbit(b, &bout, off+i);
		*out = (*out << 1) | bout;

	}

}

void nom_buffer_readbitsnext(struct nom_buffer *b, unsigned long long int *out, long long int n) {

	nom_buffer_readbits(b, out, b->boff, n);
	nom_buffer_seekbit(b, n, 1);
	
}

void nom_buffer_setbit(struct nom_buffer *b, long long int off) {

	b->buf[off/8] |= (1 << (7-(off%8)));

}

void nom_buffer_setbitnext(struct nom_buffer *b) {

	nom_buffer_setbit(b, b->boff);
	nom_buffer_seekbyte(b, 1, 1);

}

void nom_buffer_clearbit(struct nom_buffer *b, long long int off) {

	b->buf[off/8] &= ~(1 << (7-(off%8)));

}

void nom_buffer_clearbitnext(struct nom_buffer *b) {

	nom_buffer_clearbit(b, b->boff);
	nom_buffer_seekbyte(b, 1, 1);

}

void nom_buffer_setbits(struct nom_buffer *b, long long int off, unsigned long long int data, long long int n) {

	for (int i = 0; i < n; i++) {

		if (((data>>(n-i-1))&1) == 0) {

			nom_buffer_clearbit(b, off + i);

		} else {

			nom_buffer_setbit(b, off + i);

		}

	}

}

void nom_buffer_setbitsnext(struct nom_buffer *b, unsigned long long int data, long long int n) {

	nom_buffer_setbits(b, b->boff, data, n);
	nom_buffer_seekbyte(b, n, 1);

}

void nom_buffer_flipbit(struct nom_buffer *b, long long int off) {

	b->buf[off/8] ^= (1 << (7-(off%8)));

}

void nom_buffer_flipbitnext(struct nom_buffer *b) {

	nom_buffer_flipbit(b, b->boff);
	nom_buffer_seekbit(b, 1, 1);

}

void nom_buffer_clearallbits(struct nom_buffer *b) {

	for (int i = 0; i < b->cap; i++) {

		b->buf[i] = 0x00;
		
	}

}

void nom_buffer_setallbits(struct nom_buffer *b) {

	for (int i = 0; i < b->cap; i++) {

		b->buf[i] = 0xff;

	}

}

void nom_buffer_flipallbits(struct nom_buffer *b) {

	for (int i = 0; i < b->cap; i++) {

		b->buf[i] = ~b->buf[i];

	}

}

void nom_buffer_seekbit(struct nom_buffer *b, long long int off, char relative) {

	if (relative < 0) {

		b->boff = off;

	} else if (relative >= 0) {

		b->boff += off;

	}

}

void nom_buffer_afterbit(struct nom_buffer *b, long long int *out, long long int off) {

	if (off < 0) {

		*out = b->bcap - b->boff - 1;

	} else if (off >= 0) {

		*out = b->bcap - off - 1;

	}

}

void nom_buffer_alignbit(struct nom_buffer *b) {

	b->boff = b->off*8;

}

/* bytebuffer functions */

void nom_buffer_writebytes(struct nom_buffer *b, long long int off, long long int data_length, char *data) {

	memcpy(b->buf+off, data, data_length*sizeof(char)); 
	
}

void nom_buffer_writebytesnext(struct nom_buffer *b, long long int data_length, char *data) {

	nom_buffer_writebytes(b, b->off, data_length, data);
	nom_buffer_seekbyte(b, data_length, 1);

}

void nom_buffer_readbytes(struct nom_buffer *b, char *out, long long int off, long long int n) {

	memcpy(out, b->buf+off, n*sizeof(char));

}

void nom_buffer_readbytesnext(struct nom_buffer *b, char *out, long long int n) {

	nom_buffer_readbytes(b, out, b->off, n);
	nom_buffer_seekbyte(b, n, 1);

}

void nom_buffer_seekbyte(struct nom_buffer *b, long long int off, char relative) {

	if (relative < 0) {

		b->off = off;

	} else if (relative >= 0) {

		b->off += off;

	}

}

void nom_buffer_afterbyte(struct nom_buffer *b, long long int *out, long long int off) {

	if (off < 0) {

		*out = b->cap - b->off - 1;

	} else if (off >= 0) {

		*out = b->cap - off - 1;

	}

}

void nom_buffer_alignbyte(struct nom_buffer *b) {

	b->off = b->boff/8;

}

/* generic functions */

void nom_buffer_grow(struct nom_buffer *b, long long int n) {

	char *old = b->buf;

	b->buf = malloc(sizeof(*old)+n);
	memcpy(b->buf, old, sizeof(*old));

	b->cap = b->cap + n;
	b->bcap = b->cap*8;

	free(old);

}

