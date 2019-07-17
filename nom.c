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

void nom_buffer_new(struct NomBuffer *out, long long initial_size) {
	out->off = 0x00;
	out->cap = initial_size;
	out->boff = 0x00;
	out->bcap = initial_size * 8;

	out->buf = malloc(initial_size * sizeof(unsigned char));
}

void nom_buffer_destroy(struct NomBuffer *b) {
	free(b->buf);
	free(b);
}

/* bitfield functions */

void nom_buffer_readbit(struct NomBuffer *b, unsigned char *out,
			long long off) {
	*out = (b->buf[off / 8] >> (7 - (off % 8))) & 1;
}

void nom_buffer_readbitnext(struct NomBuffer *b, unsigned char *out) {
	nom_buffer_readbit(b, out, b->boff);
	nom_buffer_seekbit(b, 1, 1);
}

void nom_buffer_readbits(struct NomBuffer *b, unsigned long long *out,
			 long long off, long long n) {
	unsigned char bout;
	for (int i = 0; i < n; i++) {
		nom_buffer_readbit(b, &bout, off + i);
		*out = (*out << 1) | bout;
	}
}

void nom_buffer_readbitsnext(struct NomBuffer *b, unsigned long long *out,
			     long long n) {
	nom_buffer_readbits(b, out, b->boff, n);
	nom_buffer_seekbit(b, n, 1);
}

void nom_buffer_setbit(struct NomBuffer *b, long long off) {
	b->buf[off / 8] |= (1 << (7 - (off % 8)));
}

void nom_buffer_setbitnext(struct NomBuffer *b) {
	nom_buffer_setbit(b, b->boff);
	nom_buffer_seekbyte(b, 1, 1);
}

void nom_buffer_clearbit(struct NomBuffer *b, long long off) {
	b->buf[off / 8] &= ~(1 << (7 - (off % 8)));
}

void nom_buffer_clearbitnext(struct NomBuffer *b) {
	nom_buffer_clearbit(b, b->boff);
	nom_buffer_seekbyte(b, 1, 1);
}

void nom_buffer_setbits(struct NomBuffer *b, long long off,
			unsigned long long data, long long n) {
	for (int i = 0; i < n; i++) {
		if (((data >> (n - i - 1)) & 1) == 0) {
			nom_buffer_clearbit(b, off + i);

		} else {
			nom_buffer_setbit(b, off + i);
		}
	}
}

void nom_buffer_setbitsnext(struct NomBuffer *b, unsigned long long data,
			    long long n) {
	nom_buffer_setbits(b, b->boff, data, n);
	nom_buffer_seekbyte(b, n, 1);
}

void nom_buffer_flipbit(struct NomBuffer *b, long long off) {
	b->buf[off / 8] ^= (1 << (7 - (off % 8)));
}

void nom_buffer_flipbitnext(struct NomBuffer *b) {
	nom_buffer_flipbit(b, b->boff);
	nom_buffer_seekbit(b, 1, 1);
}

void nom_buffer_clearallbits(struct NomBuffer *b) {
	for (int i = 0; i < b->cap; i++) {
		b->buf[i] = 0x00;
	}
}

void nom_buffer_setallbits(struct NomBuffer *b) {
	for (int i = 0; i < b->cap; i++) {
		b->buf[i] = 0xff;
	}
}

void nom_buffer_flipallbits(struct NomBuffer *b) {
	for (int i = 0; i < b->cap; i++) {
		b->buf[i] = ~b->buf[i];
	}
}

void nom_buffer_seekbit(struct NomBuffer *b, long long off,
			unsigned char relative) {
	if (relative < 0) {
		b->boff = off;

	} else if (relative >= 0) {
		b->boff += off;
	}
}

void nom_buffer_afterbit(struct NomBuffer *b, long long *out, long long off) {
	if (off < 0) {
		*out = b->bcap - b->boff - 1;

	} else if (off >= 0) {
		*out = b->bcap - off - 1;
	}
}

void nom_buffer_alignbit(struct NomBuffer *b) {
	b->boff = b->off * 8;
}

/* bytebuffer functions */

void nom_buffer_writebytes(struct NomBuffer *b, long long off,
			   long long data_length, unsigned char *data) {
	memcpy(b->buf + off, data, data_length * sizeof(unsigned char));
}

void nom_buffer_writebytesnext(struct NomBuffer *b, long long data_length,
			       unsigned char *data) {
	nom_buffer_writebytes(b, b->off, data_length, data);
	nom_buffer_seekbyte(b, data_length, 1);
}

void nom_buffer_writeu16le(struct NomBuffer *b, long long off,
			   long long data_length, unsigned *data) {
	for (int i = 0; i < data_length; i++) {
		b->buf[off + (i * 2)] = (unsigned char)(data[i]);
		b->buf[off + (1 + (i * 2))] = (unsigned char)(data[i] >> 8);
	}
}

void nom_buffer_writeu16lenext(struct NomBuffer *b, long long data_length,
			       unsigned *data) {
	nom_buffer_writeu16le(b, b->off, data_length, data);
	nom_buffer_seekbyte(b, data_length * 2, 1);
}

void nom_buffer_writeu16be(struct NomBuffer *b, long long off,
			   long long data_length, unsigned *data) {
	for (int i = 0; i < data_length; i++) {
		b->buf[off + (i * 2)] = (unsigned char)(data[i] >> 8);
		b->buf[off + (1 + (i * 2))] = (unsigned char)(data[i]);
	}
}

void nom_buffer_writeu16benext(struct NomBuffer *b, long long data_length,
			       unsigned *data) {
	nom_buffer_writeu16be(b, b->off, data_length, data);
	nom_buffer_seekbyte(b, data_length * 2, 1);
}

void nom_buffer_writeu32le(struct NomBuffer *b, long long off,
			   long long data_length, unsigned long *data) {
	for (int i = 0; i < data_length; i++) {
		b->buf[off + (i * 4)] = (unsigned char)(data[i]);
		b->buf[off + (1 + (i * 4))] = (unsigned char)(data[i] >> 8);
		b->buf[off + (2 + (i * 4))] = (unsigned char)(data[i] >> 16);
		b->buf[off + (3 + (i * 4))] = (unsigned char)(data[i] >> 24);
	}
}

void nom_bufferwriteu32lenext(struct NomBuffer *b, long long data_length,
			      unsigned long *data) {
	nom_buffer_writeu32le(b, b->off, data_length, data);
	nom_buffer_seekbyte(b, data_length * 4, 1);
}

void nom_buffer_writeu32be(struct NomBuffer *b, long long off,
			   long long data_length, unsigned long *data) {
	for (int i = 0; i < data_length; i++) {
		b->buf[off + (i * 4)] = (unsigned char)(data[i] >> 24);
		b->buf[off + (1 + (i * 4))] = (unsigned char)(data[i] >> 16);
		b->buf[off + (2 + (i * 4))] = (unsigned char)(data[i] >> 8);
		b->buf[off + (3 + (i * 4))] = (unsigned char)(data[i]);
	}
}

void nom_buffer_writeu32benext(struct NomBuffer *b, long long data_length,
			       unsigned long *data) {
	nom_buffer_writeu32be(b, b->off, data_length, data);
	nom_buffer_seekbyte(b, data_length * 4, 1);
}

void nom_buffer_writeu64le(struct NomBuffer *b, long long off,
			   long long data_length, unsigned long long *data) {
	for (int i = 0; i < data_length; i++) {
		b->buf[off + (i * 8)] = (unsigned char)(data[i]);
		b->buf[off + (1 + (i * 8))] = (unsigned char)(data[i] >> 8);
		b->buf[off + (2 + (i * 8))] = (unsigned char)(data[i] >> 16);
		b->buf[off + (3 + (i * 8))] = (unsigned char)(data[i] >> 24);
		b->buf[off + (4 + (i * 8))] = (unsigned char)(data[i] >> 32);
		b->buf[off + (5 + (i * 8))] = (unsigned char)(data[i] >> 40);
		b->buf[off + (6 + (i * 8))] = (unsigned char)(data[i] >> 48);
		b->buf[off + (7 + (i * 8))] = (unsigned char)(data[i] >> 56);
	}
}

void nom_buffer_writeu64lenext(struct NomBuffer *b, long long data_length,
			       unsigned long long *data) {
	nom_buffer_writeu64le(b, b->off, data_length, data);
	nom_buffer_seekbyte(b, data_length * 8, 1);
}

void nom_buffer_writeu64be(struct NomBuffer *b, long long off,
			   long long data_length, unsigned long long *data) {
	for (int i = 0; i < data_length; i++) {
		b->buf[off + (i * 8)] = (unsigned char)(data[i] >> 56);
		b->buf[off + (1 + (i * 8))] = (unsigned char)(data[i] >> 48);
		b->buf[off + (2 + (i * 8))] = (unsigned char)(data[i] >> 40);
		b->buf[off + (3 + (i * 8))] = (unsigned char)(data[i] >> 32);
		b->buf[off + (4 + (i * 8))] = (unsigned char)(data[i] >> 24);
		b->buf[off + (5 + (i * 8))] = (unsigned char)(data[i] >> 16);
		b->buf[off + (6 + (i * 8))] = (unsigned char)(data[i] >> 8);
		b->buf[off + (7 + (i * 8))] = (unsigned char)(data[i]);
	}
}

void nom_buffer_writeu64benext(struct NomBuffer *b, long long data_length,
			       unsigned long long *data) {
	nom_buffer_writeu64be(b, b->off, data_length, data);
	nom_buffer_seekbyte(b, data_length * 8, 1);
}

void nom_buffer_readbytes(struct NomBuffer *b, unsigned char *out,
			  long long off, long long n) {
	memcpy(out, b->buf + off, n * sizeof(unsigned char));
}

void nom_buffer_readbytesnext(struct NomBuffer *b, unsigned char *out,
			      long long n) {
	nom_buffer_readbytes(b, out, b->off, n);
	nom_buffer_seekbyte(b, n, 1);
}

void nom_buffer_readu16le(struct NomBuffer *b, unsigned *out, long long off,
			  long long n) {
	for (int i = 0; i < n; i++) {
		out[i] = (unsigned)(b->buf[off + (i * 2)]) |
			 (unsigned)(b->buf[off + (1 + (i * 2))]) << 8;
	}
}

void nom_buffer_readu16lenext(struct NomBuffer *b, unsigned *out, long long n) {
	nom_buffer_readu16le(b, out, b->off, n);
	nom_buffer_seekbyte(b, n * 2, 1);
}

void nom_buffer_readu16be(struct NomBuffer *b, unsigned *out, long long off,
			  long long n) {
	for (int i = 0; i < n; i++) {
		out[i] = (unsigned)(b->buf[off + (1 + (i * 2))]) |
			 (unsigned)(b->buf[off + (i * 2)]) << 8;
	}
}

void nom_buffer_readu16benext(struct NomBuffer *b, unsigned *out, long long n) {
	nom_buffer_readu16be(b, out, b->off, n);
	nom_buffer_seekbyte(b, n * 2, 1);
}

void nom_buffer_readu32le(struct NomBuffer *b, unsigned long *out,
			  long long off, long long n) {
	for (int i = 0; i < n; i++) {
		out[i] = (unsigned long)(b->buf[off + (i * 4)]) |
			 (unsigned long)(b->buf[off + (1 + (i * 4))]) << 8 |
			 (unsigned long)(b->buf[off + (2 + (i * 4))]) << 16 |
			 (unsigned long)(b->buf[off + (3 + (i * 4))]) << 24;
	}
}

void nom_buffer_readu32lenext(struct NomBuffer *b, unsigned long *out,
			      long long n) {
	nom_buffer_readu32le(b, out, b->off, n);
	nom_buffer_seekbyte(b, n * 4, 1);
}

void nom_buffer_readu32be(struct NomBuffer *b, unsigned long *out,
			  long long off, long long n) {
	for (int i = 0; i < n; i++) {
		out[i] = (unsigned long)(b->buf[off + (3 + (i * 4))]) |
			 (unsigned long)(b->buf[off + (2 + (i * 4))]) << 8 |
			 (unsigned long)(b->buf[off + (1 + (i * 4))]) << 16 |
			 (unsigned long)(b->buf[off + (i * 4)]) << 24;
	}
}

void nom_buffer_readu32benext(struct NomBuffer *b, unsigned long *out,
			      long long n) {
	nom_buffer_readu32le(b, out, b->off, n);
	nom_buffer_seekbyte(b, n * 4, 1);
}

void nom_buffer_readu64le(struct NomBuffer *b, unsigned long long *out,
			  long long off, long long n) {
	for (int i = 0; i < n; i++) {
		out[i] =
			(unsigned long long)(b->buf[off + (i * 8)]) |
			(unsigned long long)(b->buf[off + (1 + (i * 8))]) << 8 |
			(unsigned long long)(b->buf[off + (2 + (i * 8))])
				<< 16 |
			(unsigned long long)(b->buf[off + (3 + (i * 8))])
				<< 24 |
			(unsigned long long)(b->buf[off + (4 + (i * 8))])
				<< 32 |
			(unsigned long long)(b->buf[off + (5 + (i * 8))])
				<< 40 |
			(unsigned long long)(b->buf[off + (6 + (i * 8))])
				<< 48 |
			(unsigned long long)(b->buf[off + (7 + (i * 8))]) << 56;
	}
}

void nom_buffer_readu64lenext(struct NomBuffer *b, unsigned long long *out,
			      long long n) {
	nom_buffer_readu64le(b, out, b->off, n);
	nom_buffer_seekbyte(b, n * 8, 1);
}

void nom_buffer_readu64be(struct NomBuffer *b, unsigned long long *out,
			  long long off, long long n) {
	for (int i = 0; i < n; i++) {
		out[i] = (unsigned long long)(b->buf[off + (7 + (i * 8))]) |
			 (unsigned long long)(b->buf[off + (6 + (i * 8))])
				 << 8 |
			 (unsigned long long)(b->buf[off + (5 + (i * 8))])
				 << 16 |
			 (unsigned long long)(b->buf[off + (4 + (i * 8))])
				 << 24 |
			 (unsigned long long)(b->buf[off + (3 + (i * 8))])
				 << 32 |
			 (unsigned long long)(b->buf[off + (2 + (i * 8))])
				 << 40 |
			 (unsigned long long)(b->buf[off + (1 + (i * 8))])
				 << 48 |
			 (unsigned long long)(b->buf[off + (i * 8)]) << 54;
	}
}

void nom_buffer_readu64benext(struct NomBuffer *b, unsigned long long *out,
			      long long n) {
	nom_buffer_readu64be(b, out, b->off, n);
	nom_buffer_seekbyte(b, n * 8, 1);
}

void nom_buffer_seekbyte(struct NomBuffer *b, long long off,
			 unsigned char relative) {
	if (relative < 0) {
		b->off = off;

	} else if (relative >= 0) {
		b->off += off;
	}
}

void nom_buffer_afterbyte(struct NomBuffer *b, long long *out, long long off) {
	if (off < 0) {
		*out = b->cap - b->off - 1;

	} else if (off >= 0) {
		*out = b->cap - off - 1;
	}
}

void nom_buffer_alignbyte(struct NomBuffer *b) {
	b->off = b->boff / 8;
}

/* generic functions */

void nom_buffer_grow(struct NomBuffer *b, long long n) {
	unsigned char *old = b->buf;

	b->buf = malloc(sizeof(*old) + n);
	memcpy(b->buf, old, sizeof(*old));

	b->cap = b->cap + n;
	b->bcap = b->cap * 8;

	free(old);
}

