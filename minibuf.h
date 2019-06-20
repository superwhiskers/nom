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
struct nom_minibuffer {
	/* you shouldn't modify any of these directly, but you *can* */
	char  *buf;
	int    off;
	int    cap;
	int   boff;
	int   bcap;
};

/* nom_minibuffer_new creates a new minibuffer */
void nom_minibuffer_new(struct nom_minibuffer*, int);

/* nom_minibuffer_destroy destroys an existing minibuffer */
void nom_minibuffer_destroy(struct nom_minibuffer*);

/* nom_minibuffer_grow makes the buffer's capacity bigger by n bytes */
void nom_minibuffer_grow(struct nom_minibuffer*, int);

/* nom_minibuffer_seekbytes seeks to position off of the buffer relative to the current position or exact */
void nom_minibuffer_seekbyte(struct nom_minibuffer*, int, int);

/* nom_minibuffer_seekbit seeks to bit position off of buffer relative to the current position or exact */
void nom_minibuffer_seekbit(struct nom_minibuffer*, int, int);

/* nom_minibuffer_alignbit aligns the bit offset to the byte offset */
void nom_minibuffer_alignbit(struct nom_minibuffer*);

/* nom_minibuffer_alignbyte aligns the byte offset to the bit offset */
void nom_minibuffer_alignbyte(struct nom_minibuffer*);

/* nom_minibuffer_afterbyte stores the amount of bytes located after the current position or the specified one in out */
void nom_minibuffer_afterbyte(struct nom_minibuffer*, int*, int);

/* nom_minibuffer_afterbit stores the amount of bits located after the current position or the specified one in out */
void nom_minibuffer_afterbit(struct nom_minibuffer*, int*, int);

/* nom_minibuffer_readbytes reads n bytes from the buffer at the specified offset */
void nom_minibuffer_readbytes(struct nom_minibuffer*, char*, int, int);

/* nom_minibuffer_readbytesnext reads n bytes from the buffer at the current offset and moves the offset forward the amount of bytes read */
void nom_minibuffer_readbytesnext(struct nom_minibuffer*, char*, int);

/* nom_minibuffer_writebytes writes a byte array to the buffer at the specified offset */
void nom_minibuffer_writebytes(struct nom_minibuffer*, int, int, char*);

/* nom_minibuffer_writebytesnext writes a byte array to the buffer at the current offset and moves the offset forward the amount of bytes written */
void nom_minibuffer_writebytesnext(struct nom_minibuffer*, int, char*);

/* nom_minibuffer_readbit reads a bit from the buffer at the specified offset */
void nom_minibuffer_readbit(struct nom_minibuffer*, char*, int);

/* nom_minibuffer_readbits reads n bits from the buffer at the specified offset */
void nom_minibuffer_readbits(struct nom_minibuffer*, int*, int, int);

/* nom_minibuffer_readbitnext reads the next bit from the buffer at the current offset and moves the offset forward a bit */
void nom_minibuffer_readbitnext(struct nom_minibuffer*, char*);

/* nom_minibuffer_readbitsnext reads the next n bits from the buffer at the current offset and moves the offset forward the amount of bits read */
void nom_minibuffer_readbitsnext(struct nom_minibuffer*, int*, int);

/* nom_minibuffer_setbit sets the bit located at the specified offset */
void nom_minibuffer_setbit(struct nom_minibuffer*, int);

/* nom_minibuffer_clearbit clears the bit located at the specified offset */
void nom_minibuffer_clearbit(struct nom_minibuffer*, int);

/* nom_minibuffer_setbits sets the next n bits from the specified offset value */
void nom_minibuffer_setbits(struct nom_minibuffer*, int, int, int);

/* nom_minibuffer_setbitnext sets the next bit from the current offset and moves the offset forward a bit */
void nom_minibuffer_setbitnext(struct nom_minibuffer*);

/* nom_minibuffer_clearbitnext clears the next bit from the current offset and moves the offset forward a bit */
void nom_minibuffer_clearbitnext(struct nom_minibuffer*);

/* nom_minibuffer_setbitsnext sets the next n bits from the current offset and moves the offset forward the amount of bits set */
void nom_minibuffer_setbitsnext(struct nom_minibuffer*, int, int);

/* nom_minibuffer_flipbit flips the bit located at the specified offset */
void nom_minibuffer_flipbit(struct nom_minibuffer*, int);

/* nom_minibuffer_flipbitnext flips the bit located at the current offset and moves the offset forwards a bit */
void nom_minibuffer_flipbitnext(struct nom_minibuffer*);

/* nom_minibuffer_clearallbits clears all of the bits in the buffer */
void nom_minibuffer_clearallbits(struct nom_minibuffer*);

/* nom_minibuffer_setallbits sets all of the bits in the buffer */
void nom_minibuffer_setallbits(struct nom_minibuffer*);

/* nom_minibuffer_flipallbits flips all of the bits in the buffer */
void nom_minibuffer_flipallbits(struct nom_minibuffer*);
