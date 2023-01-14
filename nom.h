/*

nom - crunch but the letter c
Copyright (c) 2019 superwhiskers <whiskerdev@protonmail.com>

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at https://mozilla.org/MPL/2.0/.

*/

//TODO(superwhiskers): consider making the iterator `i` variables use a int64_t
//TODO(superwhiskers): add in support for signed integers and floating point numbers

#ifndef NOM_NOM_H
#define NOM_NOM_H

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* a high-performance buffer type */
typedef struct NomBuffer {
	/* you shouldn't modify any of these directly, but you *can* */
	uint8_t *buf;
	int64_t off;
	int64_t cap;
	int64_t boff;
	int64_t bcap;
} NomBuffer;

/* nom_buffer_new creates a new buffer */
void nom_buffer_new(struct NomBuffer *out, int64_t initial_size) {
	out->off = 0x00;
	out->cap = initial_size;
	out->boff = 0x00;
	out->bcap = initial_size * 8;

	out->buf = (uint8_t *)(malloc(initial_size * sizeof(uint8_t)));
}

/* nom_buffer_destroy destroys an existing buffer */
void nom_buffer_destroy(struct NomBuffer *b) {
	free(b->buf);
	free(b);
}

/* nom_buffer_seekbit seeks to bit position off of buffer relative to the current position or exact */
void nom_buffer_seekbit(struct NomBuffer *b, int64_t off, uint8_t relative) {
	if (relative < 0) {
		b->boff = off;

	} else if (relative >= 0) {
		b->boff += off;
	}
}

/* nom_buffer_readbit reads a bit from the buffer at the specified offset */
void nom_buffer_readbit(struct NomBuffer *b, uint8_t *out, int64_t off) {
	*out = (b->buf[off / 8] >> (7 - (off % 8))) & 1;
}

/* nom_buffer_readbitnext reads the next bit from the buffer at the current offset and moves the offset forward a bit */
void nom_buffer_readbitnext(struct NomBuffer *b, uint8_t *out) {
	nom_buffer_readbit(b, out, b->boff);
	nom_buffer_seekbit(b, 1, 1);
}

/* nom_buffer_readbits reads n bits from the buffer at the specified offset */
void nom_buffer_readbits(struct NomBuffer *b, uint64_t *out, int64_t off,
			 int64_t n) {
	uint8_t bout;
	for (int i = 0; i < n; i++) {
		nom_buffer_readbit(b, &bout, off + i);
		*out = (*out << 1) | bout;
	}
}

/* nom_buffer_readbitsnext reads the next n bits from the buffer at the current offset and moves the offset forward the amount of bits read */
void nom_buffer_readbitsnext(struct NomBuffer *b, uint64_t *out, int64_t n) {
	nom_buffer_readbits(b, out, b->boff, n);
	nom_buffer_seekbit(b, n, 1);
}

/* nom_buffer_setbit sets the bit located at the specified offset */
void nom_buffer_setbit(struct NomBuffer *b, int64_t off) {
	b->buf[off / 8] |= (1 << (7 - (off % 8)));
}

/* nom_buffer_setbitnext sets the next bit from the current offset and moves the offset forward a bit */
void nom_buffer_setbitnext(struct NomBuffer *b) {
	nom_buffer_setbit(b, b->boff);
	nom_buffer_seekbit(b, 1, 1);
}

/* nom_buffer_clearbit clears the bit located at the specified offset */
void nom_buffer_clearbit(struct NomBuffer *b, int64_t off) {
	b->buf[off / 8] &= ~(1 << (7 - (off % 8)));
}

/* nom_buffer_clearbitnext clears the next bit from the current offset and moves the offset forward a bit */
void nom_buffer_clearbitnext(struct NomBuffer *b) {
	nom_buffer_clearbit(b, b->boff);
	nom_buffer_seekbit(b, 1, 1);
}

/* nom_buffer_setbits sets the next n bits from the specified offset value */
void nom_buffer_setbits(struct NomBuffer *b, int64_t off, uint64_t data,
			int64_t n) {
	for (int i = 0; i < n; i++) {
		if (((data >> (n - i - 1)) & 1) == 0) {
			nom_buffer_clearbit(b, off + i);

		} else {
			nom_buffer_setbit(b, off + i);
		}
	}
}

/* nom_buffer_setbitsnext sets the next n bits from the current offset and moves the offset forward the amount of bits set */
void nom_buffer_setbitsnext(struct NomBuffer *b, uint64_t data, int64_t n) {
	nom_buffer_setbits(b, b->boff, data, n);
	nom_buffer_seekbit(b, n, 1);
}

/* nom_buffer_flipbit flips the bit located at the specified offset */
void nom_buffer_flipbit(struct NomBuffer *b, int64_t off) {
	b->buf[off / 8] ^= (1 << (7 - (off % 8)));
}

/* nom_buffer_flipbitnext flips the bit located at the current offset and moves the offset forwards a bit */
void nom_buffer_flipbitnext(struct NomBuffer *b) {
	nom_buffer_flipbit(b, b->boff);
	nom_buffer_seekbit(b, 1, 1);
}

/* nom_buffer_clearallbits clears all of the bits in the buffer */
void nom_buffer_clearallbits(struct NomBuffer *b) {
	for (int i = 0; i < b->cap; i++) {
		b->buf[i] = 0x00;
	}
}

/* nom_buffer_setallbits sets all of the bits in the buffer */
void nom_buffer_setallbits(struct NomBuffer *b) {
	for (int i = 0; i < b->cap; i++) {
		b->buf[i] = 0xff;
	}
}

/* nom_buffer_flipallbits flips all of the bits in the buffer */
void nom_buffer_flipallbits(struct NomBuffer *b) {
	for (int i = 0; i < b->cap; i++) {
		b->buf[i] = ~b->buf[i];
	}
}

/* nom_buffer_afterbit stores the amount of bits located after the current position or the specified one in out */
void nom_buffer_afterbit(struct NomBuffer *b, int64_t *out, int64_t off) {
	if (off < 0) {
		*out = b->bcap - b->boff - 1;

	} else if (off >= 0) {
		*out = b->bcap - off - 1;
	}
}

/* nom_buffer_alignbit aligns the bit offset to the byte offset */
void nom_buffer_alignbit(struct NomBuffer *b) {
	b->boff = b->off * 8;
}

/* nom_buffer_seekbytes seeks to position off of the buffer relative to the current position or exact */
void nom_buffer_seekbyte(struct NomBuffer *b, int64_t off, uint8_t relative) {
	if (relative < 0) {
		b->off = off;

	} else if (relative >= 0) {
		b->off += off;
	}
}

/* nom_buffer_writebytes writes a byte array to the buffer at the specified offset */
void nom_buffer_writebytes(struct NomBuffer *b, int64_t off,
			   int64_t data_length, uint8_t *data) {
	memcpy(b->buf + off, data, data_length * sizeof(uint8_t));
}

/* nom_buffer_writebytesnext writes a byte array to the buffer at the current offset and moves the offset forward the amount of bytes written */
void nom_buffer_writebytesnext(struct NomBuffer *b, int64_t data_length,
			       uint8_t *data) {
	nom_buffer_writebytes(b, b->off, data_length, data);
	nom_buffer_seekbyte(b, data_length, 1);
}

/* nom_buffer_writeu16le writes an array of u16s to the buffer in little endian at the specified offset */
void nom_buffer_writeu16le(struct NomBuffer *b, int64_t off,
			   int64_t data_length, uint16_t *data) {
	for (int i = 0; i < data_length; i++) {
		b->buf[off + (i * 2)] = (uint8_t)(data[i]);
		b->buf[off + (1 + (i * 2))] = (uint8_t)(data[i] >> 8);
	}
}

/* nom_buffer_writeu16lenext writes an array of u16s to the buffer in little endian at the current offset and moves the offset forward the amount of bytes written */
void nom_buffer_writeu16lenext(struct NomBuffer *b, int64_t data_length,
			       uint16_t *data) {
	nom_buffer_writeu16le(b, b->off, data_length, data);
	nom_buffer_seekbyte(b, data_length * 2, 1);
}

/* nom_buffer_writeu16be writes an array of u16s to the buffer in big endian at the specified offset */
void nom_buffer_writeu16be(struct NomBuffer *b, int64_t off,
			   int64_t data_length, uint16_t *data) {
	for (int i = 0; i < data_length; i++) {
		b->buf[off + (i * 2)] = (uint8_t)(data[i] >> 8);
		b->buf[off + (1 + (i * 2))] = (uint8_t)(data[i]);
	}
}

/* nom_buffer_writeu16benext writes an array of u16s to the buffer in big endian at the current offset and moves the offset forward the amount of bytes written */
void nom_buffer_writeu16benext(struct NomBuffer *b, int64_t data_length,
			       uint16_t *data) {
	nom_buffer_writeu16be(b, b->off, data_length, data);
	nom_buffer_seekbyte(b, data_length * 2, 1);
}

/* nom_buffer_writeu32le writes an array of u32s to the buffer in little endian at the specified offset */
void nom_buffer_writeu32le(struct NomBuffer *b, int64_t off,
			   int64_t data_length, uint32_t *data) {
	for (int i = 0; i < data_length; i++) {
		b->buf[off + (i * 4)] = (uint8_t)(data[i]);
		b->buf[off + (1 + (i * 4))] = (uint8_t)(data[i] >> 8);
		b->buf[off + (2 + (i * 4))] = (uint8_t)(data[i] >> 16);
		b->buf[off + (3 + (i * 4))] = (uint8_t)(data[i] >> 24);
	}
}

/* nom_buffer_writeu32lenext writes an array of u32s to the buffer in little endian at the current offset and moves the offset forward the amount of bytes written */
void nom_bufferwriteu32lenext(struct NomBuffer *b, int64_t data_length,
			      uint32_t *data) {
	nom_buffer_writeu32le(b, b->off, data_length, data);
	nom_buffer_seekbyte(b, data_length * 4, 1);
}

/* nom_buffer_writeu32be writes an array of u32s to the buffer in big endian at the specified offset */
void nom_buffer_writeu32be(struct NomBuffer *b, int64_t off,
			   int64_t data_length, uint32_t *data) {
	for (int i = 0; i < data_length; i++) {
		b->buf[off + (i * 4)] = (uint8_t)(data[i] >> 24);
		b->buf[off + (1 + (i * 4))] = (uint8_t)(data[i] >> 16);
		b->buf[off + (2 + (i * 4))] = (uint8_t)(data[i] >> 8);
		b->buf[off + (3 + (i * 4))] = (uint8_t)(data[i]);
	}
}

/* nom_buffer_writeu32benext writes an array of u32s to the buffer in big endian at the current offset and moves the offset forward the amount of bytes written */
void nom_buffer_writeu32benext(struct NomBuffer *b, int64_t data_length,
			       uint32_t *data) {
	nom_buffer_writeu32be(b, b->off, data_length, data);
	nom_buffer_seekbyte(b, data_length * 4, 1);
}

/* nom_buffer_writeu64le writes an array of u64s to the buffer in little endian at the specified offset */
void nom_buffer_writeu64le(struct NomBuffer *b, int64_t off,
			   int64_t data_length, uint64_t *data) {
	for (int i = 0; i < data_length; i++) {
		b->buf[off + (i * 8)] = (uint8_t)(data[i]);
		b->buf[off + (1 + (i * 8))] = (uint8_t)(data[i] >> 8);
		b->buf[off + (2 + (i * 8))] = (uint8_t)(data[i] >> 16);
		b->buf[off + (3 + (i * 8))] = (uint8_t)(data[i] >> 24);
		b->buf[off + (4 + (i * 8))] = (uint8_t)(data[i] >> 32);
		b->buf[off + (5 + (i * 8))] = (uint8_t)(data[i] >> 40);
		b->buf[off + (6 + (i * 8))] = (uint8_t)(data[i] >> 48);
		b->buf[off + (7 + (i * 8))] = (uint8_t)(data[i] >> 56);
	}
}

/* nom_buffer_writeu64lenext writes an array of u64s to the buffer in little endian at the current offset and moves the offset forward the amount of bytes written */
void nom_buffer_writeu64lenext(struct NomBuffer *b, int64_t data_length,
			       uint64_t *data) {
	nom_buffer_writeu64le(b, b->off, data_length, data);
	nom_buffer_seekbyte(b, data_length * 8, 1);
}

/* nom_buffer_writeu64be writes an array of u64s to the buffer in big endian at the specified offset */
void nom_buffer_writeu64be(struct NomBuffer *b, int64_t off,
			   int64_t data_length, uint64_t *data) {
	for (int i = 0; i < data_length; i++) {
		b->buf[off + (i * 8)] = (uint8_t)(data[i] >> 56);
		b->buf[off + (1 + (i * 8))] = (uint8_t)(data[i] >> 48);
		b->buf[off + (2 + (i * 8))] = (uint8_t)(data[i] >> 40);
		b->buf[off + (3 + (i * 8))] = (uint8_t)(data[i] >> 32);
		b->buf[off + (4 + (i * 8))] = (uint8_t)(data[i] >> 24);
		b->buf[off + (5 + (i * 8))] = (uint8_t)(data[i] >> 16);
		b->buf[off + (6 + (i * 8))] = (uint8_t)(data[i] >> 8);
		b->buf[off + (7 + (i * 8))] = (uint8_t)(data[i]);
	}
}

/* nom_buffer_writeu64benext writes an array of u64s to the buffer in big endian at the current offset and moves the offset forward the amount of bytes written */
void nom_buffer_writeu64benext(struct NomBuffer *b, int64_t data_length,
			       uint64_t *data) {
	nom_buffer_writeu64be(b, b->off, data_length, data);
	nom_buffer_seekbyte(b, data_length * 8, 1);
}

/* nom_buffer_readbytes reads n bytes from the buffer at the specified offset */
void nom_buffer_readbytes(struct NomBuffer *b, uint8_t *out, int64_t off,
			  int64_t n) {
	memcpy(out, b->buf + off, n * sizeof(uint8_t));
}

/* nom_buffer_readbytesnext reads n bytes from the buffer at the current offset and moves the offset forward the amount of bytes read */
void nom_buffer_readbytesnext(struct NomBuffer *b, uint8_t *out, int64_t n) {
	nom_buffer_readbytes(b, out, b->off, n);
	nom_buffer_seekbyte(b, n, 1);
}

/* nom_buffer_readu16le reads n u16s from the buffer in little endian at the specified offset */
void nom_buffer_readu16le(struct NomBuffer *b, uint16_t *out, int64_t off,
			  int64_t n) {
	for (int i = 0; i < n; i++) {
		out[i] = (uint16_t)(b->buf[off + (i * 2)]) |
			 (uint16_t)(b->buf[off + (1 + (i * 2))]) << 8;
	}
}

/* nom_buffer_readu16lenext reads n u16s from the buffer in little endian at the current offset and moves the offset forward the amount of bytes read */
void nom_buffer_readu16lenext(struct NomBuffer *b, uint16_t *out, int64_t n) {
	nom_buffer_readu16le(b, out, b->off, n);
	nom_buffer_seekbyte(b, n * 2, 1);
}

/* nom_buffer_readu16be reads n u16s from the buffer in big endian at the specified offset */
void nom_buffer_readu16be(struct NomBuffer *b, uint16_t *out, int64_t off,
			  int64_t n) {
	for (int i = 0; i < n; i++) {
		out[i] = (uint16_t)(b->buf[off + (1 + (i * 2))]) |
			 (uint16_t)(b->buf[off + (i * 2)]) << 8;
	}
}

/* nom_buffer_readu16benext reads n u16s from the buffer in big endian at the current offset and moves the offset forward the amount of bytes read */
void nom_buffer_readu16benext(struct NomBuffer *b, uint16_t *out, int64_t n) {
	nom_buffer_readu16be(b, out, b->off, n);
	nom_buffer_seekbyte(b, n * 2, 1);
}

/* nom_buffer_readu32le reads n u32s from the buffer in little endian at the specified offset */
void nom_buffer_readu32le(struct NomBuffer *b, uint32_t *out, int64_t off,
			  int64_t n) {
	for (int i = 0; i < n; i++) {
		out[i] = (uint32_t)(b->buf[off + (i * 4)]) |
			 (uint32_t)(b->buf[off + (1 + (i * 4))]) << 8 |
			 (uint32_t)(b->buf[off + (2 + (i * 4))]) << 16 |
			 (uint32_t)(b->buf[off + (3 + (i * 4))]) << 24;
	}
}

/* nom_buffer_readu32lenext reads n u32s from the buffer in little endian at the current offset and moves the offset forward the amount of bytes read */
void nom_buffer_readu32lenext(struct NomBuffer *b, uint32_t *out, int64_t n) {
	nom_buffer_readu32le(b, out, b->off, n);
	nom_buffer_seekbyte(b, n * 4, 1);
}

/* nom_buffer_readu32be reads n u32s from the buffer in big endian at the specified offset */
void nom_buffer_readu32be(struct NomBuffer *b, uint32_t *out, int64_t off,
			  int64_t n) {
	for (int i = 0; i < n; i++) {
		out[i] = (uint32_t)(b->buf[off + (3 + (i * 4))]) |
			 (uint32_t)(b->buf[off + (2 + (i * 4))]) << 8 |
			 (uint32_t)(b->buf[off + (1 + (i * 4))]) << 16 |
			 (uint32_t)(b->buf[off + (i * 4)]) << 24;
	}
}

/* nom_buffer_readu32benext reads n u32s from the buffer in big endian at the current offset and moves the offset forward the amount of bytes read */
void nom_buffer_readu32benext(struct NomBuffer *b, uint32_t *out, int64_t n) {
	nom_buffer_readu32le(b, out, b->off, n);
	nom_buffer_seekbyte(b, n * 4, 1);
}

/* nom_buffer_readu64le reads n u64s from the buffer in little endian at the specified offset */
void nom_buffer_readu64le(struct NomBuffer *b, uint64_t *out, int64_t off,
			  int64_t n) {
	for (int i = 0; i < n; i++) {
		out[i] = (uint64_t)(b->buf[off + (i * 8)]) |
			 (uint64_t)(b->buf[off + (1 + (i * 8))]) << 8 |
			 (uint64_t)(b->buf[off + (2 + (i * 8))]) << 16 |
			 (uint64_t)(b->buf[off + (3 + (i * 8))]) << 24 |
			 (uint64_t)(b->buf[off + (4 + (i * 8))]) << 32 |
			 (uint64_t)(b->buf[off + (5 + (i * 8))]) << 40 |
			 (uint64_t)(b->buf[off + (6 + (i * 8))]) << 48 |
			 (uint64_t)(b->buf[off + (7 + (i * 8))]) << 56;
	}
}

/* nom_buffer_readu64lenext reads n u64s from the buffer in little endian at the current offset and moves the offset forward the amount of bytes read */
void nom_buffer_readu64lenext(struct NomBuffer *b, uint64_t *out, int64_t n) {
	nom_buffer_readu64le(b, out, b->off, n);
	nom_buffer_seekbyte(b, n * 8, 1);
}

/* nom_buffer_readu64be reads n u64s from the buffer in big endian at the specified offset */
void nom_buffer_readu64be(struct NomBuffer *b, uint64_t *out, int64_t off,
			  int64_t n) {
	for (int i = 0; i < n; i++) {
		out[i] = (uint64_t)(b->buf[off + (7 + (i * 8))]) |
			 (uint64_t)(b->buf[off + (6 + (i * 8))]) << 8 |
			 (uint64_t)(b->buf[off + (5 + (i * 8))]) << 16 |
			 (uint64_t)(b->buf[off + (4 + (i * 8))]) << 24 |
			 (uint64_t)(b->buf[off + (3 + (i * 8))]) << 32 |
			 (uint64_t)(b->buf[off + (2 + (i * 8))]) << 40 |
			 (uint64_t)(b->buf[off + (1 + (i * 8))]) << 48 |
			 (uint64_t)(b->buf[off + (i * 8)]) << 54;
	}
}

/* nom_buffer_readu64benext reads n u64s from the buffer in big endian at the current offset and moves the offset forward the amount of bytes read */
void nom_buffer_readu64benext(struct NomBuffer *b, uint64_t *out, int64_t n) {
	nom_buffer_readu64be(b, out, b->off, n);
	nom_buffer_seekbyte(b, n * 8, 1);
}

/* nom_buffer_afterbyte stores the amount of bytes located after the current position or the specified one in out */
void nom_buffer_afterbyte(struct NomBuffer *b, int64_t *out, int64_t off) {
	if (off < 0) {
		*out = b->cap - b->off - 1;

	} else if (off >= 0) {
		*out = b->cap - off - 1;
	}
}

/* nom_buffer_alignbyte aligns the byte offset to the bit offset */
void nom_buffer_alignbyte(struct NomBuffer *b) {
	b->off = b->boff / 8;
}

/* nom_buffer_grow makes the buffer's capacity bigger by n bytes */
void nom_buffer_grow(struct NomBuffer *b, int64_t n) {
	uint8_t *old = b->buf;

	b->buf = (uint8_t *)(malloc(sizeof(*old) + n));
	memcpy(b->buf, old, sizeof(*old));

	b->cap += n;
	b->bcap = b->cap * 8;

	free(old);
}

#endif
