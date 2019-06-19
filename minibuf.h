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

/* a higher-performance reimplementation of crunch's minibuffer in c */
struct crunch_minibuffer {
	/* you shouldn't modify any of these directly, but you *can* */
	char  *buf;
	int    off;
	int    cap;
	int   boff;
	int   bcap;
};

/* crunch_minibuffer_new creates a new minibuffer */
void crunch_minibuffer_new(struct crunch_minibuffer*, int);

/* crunch_minibuffer_destroy destroys an existing minibuffer */
void crunch_minibuffer_destroy(struct crunch_minibuffer*);

/* crunch_minibuffer_grow makes the buffer's capacity bigger by n bytes */
void crunch_minibuffer_grow(struct crunch_minibuffer*, int);

/* crunch_minibuffer_seek seeks to position off of the buffer relative to the current position or exact */
void crunch_minibuffer_seek(struct crunch_minibuffer*, int, int);

/* crunch_minibuffer_seekbit seeks to bit position off of buffer relative to the current position or exact */
void crunch_minibuffer_seekbit(struct crunch_minibuffer*, int, int);

/* crunch_minibuffer_alignbit aligns the bit offset to the byte offset */
void crunch_minibuffer_alignbit(struct crunch_minibuffer*);

/* crunch_minibuffer_alignbyte aligns the byte offset to the bit offset */
void crunch_minibuffer_alignbyte(struct crunch_minibuffer*);

/* crunch_minibuffer_after stores the amount of bytes located after the current position or the specified one in out */
void crunch_minibuffer_after(struct crunch_minibuffer*, int*, int);

/* crunch_minibuffer_afterbit stores the amount of bits located after the current position or the specified one in out */
void crunch_minibuffer_afterbit(struct crunch_minibuffer*, int*, int);

/* crunch_minibuffer_read reads n bytes from the buffer at the specified offset */
void crunch_minibuffer_read(struct crunch_minibuffer*, char*, int, int);

/* crunch_minibuffer_readnext reads n bytes from the buffer at the current offset and moves the offset forward the amount of bytes read */
void crunch_minibuffer_readnext(struct crunch_minibuffer*, char*, int);

/* crunch_minibuffer_write writes a byte array to the buffer at the specified offset */
void crunch_minibuffer_write(struct crunch_minibuffer*, int, int, char*);

/* crunch_minibuffer_writenext writes a byte array to the buffer at the current offset and moves the offset forward the amount of bytes written */
void crunch_minibuffer_writenext(struct crunch_minibuffer*, int, char*);

/* crunch_minibuffer_readbit reads a bit from the buffer at the specified offset */
void crunch_minibuffer_readbit(struct crunch_minibuffer*, char*, int);

/* crunch_minibuffer_readbits reads n bits from the buffer at the specified offset */
void crunch_minibuffer_readbits(struct crunch_minibuffer*, int*, int, int);

/* crunch_minibuffer_readbitnext reads the next bit from the buffer at the current offset and moves the offset forward a bit */
void crunch_minibuffer_readbitnext(struct crunch_minibuffer*, char*);

/* crunch_minibuffer_readbitsnext reads the next n bits from the buffer at the current offset and moves the offset forward the amount of bits read */
void crunch_minibuffer_readbitsnext(struct crunch_minibuffer*, int*, int);

/* crunch_minibuffer_setbit sets the bit located at the specified offset */
void crunch_minibuffer_setbit(struct crunch_minibuffer*, int, char);

/* crunch_minibuffer_setbits sets the next n bits from the specified offset value */
void crunch_minibuffer_setbits(struct crunch_minibuffer*, int, int, int);

/* crunch_minibuffer_setbitnext sets the next bit from the current offset and moves the offset forward a bit */
void crunch_minibuffer_setbitnext(struct crunch_minibuffer*, char);

/* crunch_minibuffer_setbitsnext sets the next n bits from the current offset and moves the offset forward the amount of bits set */
void crunch_minibuffer_setbitsnext(struct crunch_minibuffer*, int, int);

/* crunch_minibuffer_flipbit flips the bit located at the specified offset */
void crunch_minibuffer_flipbit(struct crunch_minibuffer*, int);

/* crunch_minibuffer_flipbitnext flips the bit located at the current offset and moves the offset forwards a bit */
void crunch_minibuffer_flipbitnext(struct crunch_minibuffer*);

/* crunch_minibuffer_clearallbits clears all of the bits in the buffer */
void crunch_minibuffer_clearallbits(struct crunch_minibuffer*);

/* crunch_minibuffer_setallbits sets all of the bits in the buffer */
void crunch_minibuffer_setallbits(struct crunch_minibuffer*);

/* crunch_minibuffer_flipallbits flips all of the bits in the buffer */
void crunch_minibuffer_flipallbits(struct crunch_minibuffer*);
