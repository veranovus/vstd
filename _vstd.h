/*****************************************************************************
 * Copyright (C) 2023 veranovus
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

#ifdef VSTD_STRING_STRIP_PREFIX
typedef struct _VSTD_String String;
#define _VSTD_String String
#else
typedef struct _VSTD_String VSTD_String;
#define _VSTD_String VSTD_String
#endif

#define _VSTD_STRING_INITIAL_CAP 1

/*****************************************************************************
 *
 * @function
 *   vstd_string_new
 *
 * @description
 *   Creates a new empty VSTD_String object, this function internally calls
 *   vstd_string_with_capacity with _VSTD_STRING_INITIAL_CAP as the argument.
 *
 * @return
 *   New empty VSTD_String with capacity of _VSTD_STRING_INITIAL_CAP.
 *
 * */
VSTD_STATIC _VSTD_String vstd_string_new();

/*****************************************************************************
 *
 * @function
 *   vstd_string_with_capacity
 *
 * @description
 *   Creates a new empty VSTD_String object with given capacity, memory is only
 *   allocated for the underlying pointer.
 *
 * @param[in]
 *   cap : Capacity for the new VSTD_String.
 *
 * @return
 *   New empty VSTD_String with the given capacity.
 *
 * */
VSTD_STATIC _VSTD_String vstd_string_with_capacity(usize cap);

/*****************************************************************************
 *
 * @function
 *   vstd_string_from
 *
 * @description
 *   Creates a new null terminated VSTD_String from given C string.
 *
 * @param[in]
 *   str : Null terminated C string to copy from.
 *
 * @return
 *   New null terminated VSTD_String with the same contents as the given C
 *   string.
 *
 * */
VSTD_STATIC _VSTD_String vstd_string_from(const char *str);

/*****************************************************************************
 *
 * @function
 *   vstd_string_clone
 *
 * @description
 *   Clones the given VSTD_String and returns the clone.
 *
 * @param[in]
 *   string : VSTD_String to clone.
 *
 * @return
 *   New VSTD_String identical to the given VSTD_String.
 *
 * */
VSTD_STATIC _VSTD_String vstd_string_clone(const _VSTD_String *string);

/*****************************************************************************
 *
 * @function
 *   vstd_string_format
 *
 * @description
 *   Creates a new VSTD_String using the formatted contents.
 *
 * @param[in]
 *   fmt : Null terminated C string specifying how to interpret the data.
 * @param[in]
 *   ... : Arguments specifying the data to format.
 *
 * @return
 *   New VSTD_String with the formatted contents.
 *
 * */
VSTD_STATIC _VSTD_String vstd_string_format(const char *fmt, ...);

/*****************************************************************************
 *
 * @function
 *   vstd_string_push
 *
 * @description
 *   Adds the given character to the end of given VSTD_String, this function
 *   may resize the given VSTD_String.
 *
 * @param[in]
 *   string : VSTD_String to modify.
 * @param[in]
 *   c : Character to push.
 *
 * @return
 *   Pointer to same modified VSTD_String.
 *
 * */
VSTD_STATIC _VSTD_String *vstd_string_push(_VSTD_String *string, char c);

/*****************************************************************************
 *
 * @function
 *   vstd_string_push_str
 *
 * @description
 *   Adds the C string to the end of given VSTD_String, this function
 *   may resize the given VSTD_String.
 *
 * @param[in]
 *   string : VSTD_String to modify.
 * @param[in]
 *   str : Null terminated C string to push.
 *
 * @return
 *   Pointer to same modified VSTD_String.
 *
 * */
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