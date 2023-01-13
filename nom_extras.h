/*

nom - crunch but the letter c
copyright (c) 2019 superwhiskers <whiskerdev@protonmail.com>

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at https://mozilla.org/MPL/2.0/.

*/

#ifndef NOM_NOM_EXTRAS_H
#define NOM_NOM_EXTRAS_H

#include <stdint.h>

#include "nom.h"

/* nom_buffer_writecomplexle writes an array of integers to the buffer in little endian at the specified offset */
#define nom_buffer_writecomplexle(b, o, n, d)                                  \
	_Generic((d), uint8_t *                                                \
		 : nom_buffer_writebytes, uint16_t *                           \
		 : nom_buffer_writeu16le, uint32_t *                           \
		 : nom_buffer_writeu32le, uint64_t *                           \
		 : nom_buffer_writeu64le)((b), (o), (n), (d))

/* nom_buffer_writecomplexlenext writes an array of integers to the buffer in little endian at the current offset and moves the offset forward the amount of bytes written */
#define nom_buffer_writecomplexlenext(b, n, d)                                 \
	_Generic((d), uint8_t *                                                \
		 : nom_buffer_writebytesnext, uint16_t *                       \
		 : nom_buffer_writeu16lenext, uint32_t *                       \
		 : nom_buffer_writeu32lenext, uint64_t *                       \
		 : nom_buffer_writeu64lenext)((b), (n), (d))

/* nom_buffer_writecomplexbe writes an array of integers to the buffer in big endian at the specified offset */
#define nom_buffer_writecomplexbe(b, o, n, d)                                  \
	_Generic((d), uint8_t *                                                \
		 : nom_buffer_writebytes, uint16_t *                           \
		 : nom_buffer_writeu16be, uint32_t *                           \
		 : nom_buffer_writeu32be, uint64_t *                           \
		 : nom_buffer_writeu64be)((b), (o), (n), (d))

/* nom_buffer_writecomplexbenext writes an array of integers to the buffer in big endian at the current offset and moves the offset forward the amount of bytes written */
#define nom_buffer_writecomplexbenext(b, n, d)                                 \
	_Generic((d), uint8_t *                                                \
		 : nom_buffer_writebytesnext, uint16_t *                       \
		 : nom_buffer_writeu16benext, uint32_t *                       \
		 : nom_buffer_writeu32benext, uint64_t *                       \
		 : nom_buffer_writeu64benext)((b), (n), (d))

/* nom_buffer_readcomplexle reads n integers from the buffer in little endian at the specified offset */
#define nom_buffer_readcomplexle(b, d, o, n)                                   \
	_Generic((d), uint8_t *                                                \
		 : nom_buffer_readbytes, uint16_t *                            \
		 : nom_buffer_readu16le, uint32_t *                            \
		 : nom_buffer_readu32le, uint64_t *                            \
		 : nom_buffer_readu64le)((b), (d), (o), (n))

/* nom_buffer_readcomplexlenext reads n integers from the buffer in little endian at the current offset and moves the offset forward the amount of bytes read */
#define nom_buffer_readcomplexlenext(b, d, n)                                  \
	_Generic((d), uint8_t *                                                \
		 : nom_buffer_readbytesnext, uint16_t *                        \
		 : nom_buffer_readu16lenext, uint32_t *                        \
		 : nom_buffer_readu32lenext, uint64_t *                        \
		 : nom_buffer_readu64lenext)((b), (d), (n))

/* nom_buffer_readcomplexbe reads n integers from the buffer in big endian at the specified offset */
#define nom_buffer_readcomplexbe(b, d, o, n)                                   \
	_Generic((d), uint8_t *                                                \
		 : nom_buffer_readbytes, uint16_t *                            \
		 : nom_buffer_readu16be, uint32_t *                            \
		 : nom_buffer_readu32be, uint64_t *                            \
		 : nom_buffer_readu64be)((b), (d), (o), (n))

/* nom_buffer_readcomplexbenext reads n integers from the buffer in big endian at the current offset and moves the offset forward the amount of bytes read */
#define nom_buffer_readcomplexbenext(b, d, n)                                  \
	_Generic((d), uint8_t *                                                \
		 : nom_buffer_readbytesnext, uint16_t *                        \
		 : nom_buffer_readu16benext, uint32_t *                        \
		 : nom_buffer_readu32benext, uint64_t *                        \
		 : nom_buffer_readu64benext)((b), (d), (n))

#endif
