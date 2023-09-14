/*****************************************************************************
 * Copyright (C) 2023 veranovus <veranovus1@gmail.com>
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the
 * use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 * */

#ifndef VSTD_H_
#define VSTD_H_

/*****************************************************************************
 * Commonly used C standard headers, and type abbreviations used by vstd.
 * */

#include <inttypes.h>
#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef size_t usize;
typedef intmax_t isize;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

#ifndef VSTD_STATIC
#define VSTD_STATIC __attribute__((unused)) static
#endif

#ifndef VSTD_INLINE
#define VSTD_INLINE __attribute__((unused, always_inline)) static inline
#endif

/*****************************************************************************
 *
 * @type:
 *   VSTD_String
 *
 * @description:
 *   String implementation, this struct never allocates memory for itself, but
 *   only for its underlying pointer. Whenever you're passing an instance of
 *   this type its safe to pass it as it is to a function, as long as that
 *   function doesn't modify the string or any of it's elements.
 *
 * */
struct _VSTD_String {
  char *ptr;
  usize len;
  usize cap;
};

#ifdef VSTD_STRIP_STRING_PREFIX
typedef struct _VSTD_String String;
#define _VSTD_String String
#else
typedef struct _VSTD_String VSTD_String;
#define _VSTD_String VSTD_String
#endif

/*****************************************************************************
 *
 * @function:
 *   vstd_string_new();
 *
 * @description:
 *   Creates a new string object, this function internally calls
 *   vstd_string_with_capacity with _VSTD_STRING_INITIAL_CAP as the argument.
 *
 * */
VSTD_STATIC _VSTD_String vstd_string_new();

/*****************************************************************************
 *
 * @function:
 *   vstd_string_with_capacity();
 *
 * @description:
 *   Creates a new string object with given capacity, memory is only allocated
 *   for the underlying pointer.
 *
 * */
VSTD_STATIC _VSTD_String vstd_string_with_capacity(usize cap);
VSTD_STATIC _VSTD_String vstd_string_from(const char *str);
VSTD_STATIC _VSTD_String vstd_string_clone(const _VSTD_String *string);
VSTD_STATIC _VSTD_String vstd_string_format(const char *fmt, ...);
VSTD_STATIC _VSTD_String *vstd_string_push(_VSTD_String *string, char c);
VSTD_STATIC _VSTD_String *vstd_string_push_str(_VSTD_String *string,
                                               const char *str);
VSTD_STATIC _VSTD_String *vstd_string_remove_at(_VSTD_String *string,
                                                usize index, usize len);
VSTD_STATIC _VSTD_String *vstd_string_remove(_VSTD_String *string,
                                             const char *sub);
VSTD_STATIC char *vstd_string_find_first(const _VSTD_String *string,
                                         const char *sub);
VSTD_STATIC char *vstd_string_find_last(const _VSTD_String *string,
                                        const char *sub);
VSTD_STATIC isize vstd_string_compare(const _VSTD_String *string,
                                      const char *str);
VSTD_STATIC void vstd_string_free(_VSTD_String *string);

#endif // VSTD_H_