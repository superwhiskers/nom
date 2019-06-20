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

#pragma once

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* a high-performance buffer type */
struct nom_buffer {
	/* you shouldn't modify any of these directly, but you *can* */
	unsigned char *buf;
	long long      off;
	long long      cap;
	long long     boff;
	long long     bcap;
};

/* nom_buffer_new creates a new buffer */
void nom_buffer_new(struct nom_buffer*, long long);

/* nom_buffer_destroy destroys an existing buffer */
void nom_buffer_destroy(struct nom_buffer*);

/* nom_buffer_readbit reads a bit from the buffer at the specified offset */
void nom_buffer_readbit(struct nom_buffer*, unsigned char*, long long);

/* nom_buffer_readbitnext reads the next bit from the buffer at the current offset and moves the offset forward a bit */
void nom_buffer_readbitnext(struct nom_buffer*, unsigned char*);

/* nom_buffer_readbits reads n bits from the buffer at the specified offset */
void nom_buffer_readbits(struct nom_buffer*, unsigned long long*, long long, long long);

/* nom_buffer_readbitsnext reads the next n bits from the buffer at the current offset and moves the offset forward the amount of bits read */
void nom_buffer_readbitsnext(struct nom_buffer*, unsigned long long*, long long);

/* nom_buffer_setbit sets the bit located at the specified offset */
void nom_buffer_setbit(struct nom_buffer*, long long);

/* nom_buffer_setbitnext sets the next bit from the current offset and moves the offset forward a bit */
void nom_buffer_setbitnext(struct nom_buffer*);

/* nom_buffer_clearbit clears the bit located at the specified offset */
void nom_buffer_clearbit(struct nom_buffer*, long long);

/* nom_buffer_clearbitnext clears the next bit from the current offset and moves the offset forward a bit */
void nom_buffer_clearbitnext(struct nom_buffer*);

/* nom_buffer_setbits sets the next n bits from the specified offset value */
void nom_buffer_setbits(struct nom_buffer*, long long, unsigned long long, long long);

/* nom_buffer_setbitsnext sets the next n bits from the current offset and moves the offset forward the amount of bits set */
void nom_buffer_setbitsnext(struct nom_buffer*, unsigned long long, long long);

/* nom_buffer_flipbit flips the bit located at the specified offset */
void nom_buffer_flipbit(struct nom_buffer*, long long);

/* nom_buffer_flipbitnext flips the bit located at the current offset and moves the offset forwards a bit */
void nom_buffer_flipbitnext(struct nom_buffer*);

/* nom_buffer_clearallbits clears all of the bits in the buffer */
void nom_buffer_clearallbits(struct nom_buffer*);

/* nom_buffer_setallbits sets all of the bits in the buffer */
void nom_buffer_setallbits(struct nom_buffer*);

/* nom_buffer_flipallbits flips all of the bits in the buffer */
void nom_buffer_flipallbits(struct nom_buffer*);

/* nom_buffer_seekbit seeks to bit position off of buffer relative to the current position or exact */
void nom_buffer_seekbit(struct nom_buffer*, long long, unsigned char);

/* nom_buffer_afterbit stores the amount of bits located after the current position or the specified one in out */
void nom_buffer_afterbit(struct nom_buffer*, long long*, long long);

/* nom_buffer_alignbit aligns the bit offset to the byte offset */
void nom_buffer_alignbit(struct nom_buffer*);

/* nom_buffer_writebytes writes a byte array to the buffer at the specified offset */
void nom_buffer_writebytes(struct nom_buffer*, long long, long long, unsigned char*);

/* nom_buffer_writebytesnext writes a byte array to the buffer at the current offset and moves the offset forward the amount of bytes written */
void nom_buffer_writebytesnext(struct nom_buffer*, long long, unsigned char*);

/* nom_buffer_readbytes reads n bytes from the buffer at the specified offset */
void nom_buffer_readbytes(struct nom_buffer*, unsigned char*, long long, long long);

/* nom_buffer_readbytesnext reads n bytes from the buffer at the current offset and moves the offset forward the amount of bytes read */
void nom_buffer_readbytesnext(struct nom_buffer*, unsigned char*, long long);

/* nom_buffer_seekbytes seeks to position off of the buffer relative to the current position or exact */
void nom_buffer_seekbyte(struct nom_buffer*, long long, unsigned char);

/* nom_buffer_afterbyte stores the amount of bytes located after the current position or the specified one in out */
void nom_buffer_afterbyte(struct nom_buffer*, long long*, long long);

/* nom_buffer_alignbyte aligns the byte offset to the bit offset */
void nom_buffer_alignbyte(struct nom_buffer*);

/* nom_buffer_grow makes the buffer's capacity bigger by n bytes */
void nom_buffer_grow(struct nom_buffer*, long long);
