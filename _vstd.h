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
 *
 * @section
 *   VSTD Common
 *
 * @description
 *   Commonly used C standard headers, and type abbreviations.
 *
 * */

#include <inttypes.h>
#include <memory.h>
#include <stdarg.h>
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

#define VSTD_STRINGIFY(name) #name

/*****************************************************************************
 *
 * @section
 *   VSTD String
 *
 */

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

#ifndef VSTD_STRING_INITIAL_CAP
#define VSTD_STRING_INITIAL_CAP 1
#endif

#define VSTD_STRING_EQUAL 0

/*****************************************************************************
 *
 * @section
 *   VSTD String function declarations
 *
 * */

/*****************************************************************************
 *
 * @function
 *   vstd_string_new
 *
 * @description
 *   Creates a new empty VSTD_String object, this function internally calls
 *   vstd_string_with_capacity with VSTD_STRING_INITIAL_CAP as the argument.
 *
 * @return
 *   New empty VSTD_String with capacity of VSTD_STRING_INITIAL_CAP.
 *
 * */
VSTD_INLINE _VSTD_String vstd_string_new();

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

/*****************************************************************************
 *
 * @function
 *   vstd_string_remove
 *
 * @description
 *   Tries to remove the given C string if it is present in the given
 *   VSTD_String. Substring is only removed once.
 *
 * @param[in]
 *   string : VSTD_String to modify.
 * @param[in]
 *   sub : Null terminated C string to remove.
 *
 * @return
 *   Pointer to same VSTD_String, it may or may not be modified depending on
 *   whether or not the given C string is present in it.
 *
 * */
VSTD_STATIC _VSTD_String *vstd_string_remove(_VSTD_String *string,
                                             const char *sub);

/*****************************************************************************
 *
 * @function
 *   vstd_string_remove_at
 *
 * @description
 *   Removes n number of characters starting from the given index for the given
 *   VSTD_String.
 *
 * @param[in]
 *   string : VSTD_String to modify.
 * @param[in]
 *   index : Starting index for removal.
 * @param[in]
 *   len : Number of characters to remove.
 *
 * @return
 *   Pointer to same modified VSTD_String.
 *
 * */
VSTD_STATIC _VSTD_String *vstd_string_remove_at(_VSTD_String *string,
                                                usize index, usize len);

/*****************************************************************************
 *
 * @function
 *   vstd_string_find_first
 *
 * @description
 *   Finds the first occurrence of the given C string inside of the given
 *   VSTD_String. Underlying implementation of this function just calls strstr
 *   from standard library. Returns a NULL pointer if it fails.
 *
 * @param[in]
 *   string : VSTD_String to search.
 * @param[in]
 *   sub : Null terminated C string to search for.
 *
 * @return
 *   Pointer to the start of the C string's first occurrence inside the given
 *   VSTD_String or a NULL pointer.
 *
 * */
VSTD_INLINE char *vstd_string_find_first(const _VSTD_String *string,
                                         const char *sub);

/*****************************************************************************
 *
 * @function
 *   vstd_string_find_first
 *
 * @description
 *   Finds the last occurrence of the given C string inside of the given
 *   VSTD_String. Returns a NULL pointer if it fails.
 *
 * @param[in]
 *   string : VSTD_String to search.
 * @param[in]
 *   sub : Null terminated C string to search for.
 *
 * @return
 *   Pointer to the start of the C string's last occurrence inside the given
 *   VSTD_String or a NULL pointer.
 *
 * */
VSTD_STATIC char *vstd_string_find_last(const _VSTD_String *string,
                                        const char *sub);

/*****************************************************************************
 *
 * @function
 *   vstd_string_compare
 *
 * @description
 *   Compares the given C string with the given VSTD_String. This function is
 *   only there for API consistency, underlying implementation of this function
 *   just calls strcmp thus behaviour is identical to that function. For more
 *   information about strcmp you can check:
 *   https://pubs.opengroup.org/onlinepubs/9699919799.2016edition/functions/strcmp.html
 *
 * @param[in]
 *   string : VSTD_String to compare.
 * @param[in]
 *   str : Null terminated C string to be compared with.
 *
 * @return
 *   Integer value indicating the relationship between the two strings.
 *
 * */
VSTD_INLINE isize vstd_string_compare(const _VSTD_String *string,
                                      const char *str);

/*****************************************************************************
 *
 * @function
 *   vstd_string_free
 *
 * @description
 *   Frees the memory allocated for given VSTD_String's underlying pointer.
 *
 * @param[in]
 *   string : VSTD_String to free.
 *
 * */
VSTD_STATIC void vstd_string_free(_VSTD_String *string);

/*****************************************************************************
 *
 * @function
 *   _vstd_string_realloc
 *
 * @description
 *   Doubles the given VSTD_String's capacity and reallocates the underlying
 *   pointer. This is a helper function and it should only be used by the vstd
 *   library functions.
 *
 * @param[in]
 *   string : VSTD_String to reallocate.
 *
 * */
VSTD_INLINE void _vstd_string_realloc(_VSTD_String *string);

/*****************************************************************************
 *
 * @section
 *   VSTD String function implementations
 *
 * */

VSTD_INLINE _VSTD_String vstd_string_new() {
  return vstd_string_with_capacity(VSTD_STRING_INITIAL_CAP);
}

VSTD_STATIC _VSTD_String vstd_string_with_capacity(usize cap) {
  return (_VSTD_String){
      .ptr = (char *)calloc(cap, sizeof(char)),
      .cap = cap,
      .len = 0,
  };
}

VSTD_STATIC _VSTD_String vstd_string_from(const char *str) {
  usize len = strlen(str);
  _VSTD_String s = vstd_string_with_capacity(len + 1);

  strcpy(s.ptr, str);
  s.len = len;

  return s;
}

VSTD_STATIC _VSTD_String vstd_string_clone(const _VSTD_String *string) {
  _VSTD_String clone = vstd_string_with_capacity(string->cap);

  memcpy(clone.ptr, string->ptr, sizeof(char) * string->len);
  clone.len = string->len;

  return clone;
}

VSTD_STATIC _VSTD_String vstd_string_format(const char *fmt, ...) {
  va_list arg_ptr;
  va_start(arg_ptr, fmt);

  usize len = vsnprintf(NULL, 0, fmt, arg_ptr);
  _VSTD_String s = vstd_string_with_capacity(len + 1);

  vsnprintf(s.ptr, s.cap, fmt, arg_ptr);
  s.len = len;

  va_end(arg_ptr);
  return s;
}

VSTD_STATIC _VSTD_String *vstd_string_push(_VSTD_String *string, char c) {
  if (string->len + 1 >= string->cap) {
    _vstd_string_realloc(string);
  }

  string->ptr[string->len] = c;
  string->len++;

  string->ptr[string->len] = '\0';
  return string;
}

VSTD_STATIC _VSTD_String *vstd_string_push_str(_VSTD_String *string,
                                               const char *str) {
  usize len = strlen(str);

  while (string->len + len >= string->cap) {
    _vstd_string_realloc(string);
  }

  strcpy(string->ptr + string->len, str);
  string->len += len;

  string->ptr[string->len] = '\0';
  return string;
}

VSTD_STATIC _VSTD_String *vstd_string_remove(_VSTD_String *string,
                                             const char *sub) {
  if (!sub[0]) {
    return string;
  }

  usize len = strlen(sub);
  char *ptr = strstr(string->ptr, sub);
  if (!ptr) {
    return string;
  }

  return vstd_string_remove_at(string, (usize)(ptr - string->ptr), len);
}

VSTD_STATIC _VSTD_String *vstd_string_remove_at(_VSTD_String *string,
                                                usize index, usize len) {
  usize move_size = string->len - (index + len);

  if (move_size > 0) {
    memmove(string->ptr + index, string->ptr + index + len, move_size);
  }
  string->len -= len;

  string->ptr[string->len] = '\0';
  return string;
}

VSTD_INLINE char *vstd_string_find_first(const _VSTD_String *string,
                                         const char *sub) {
  return strstr(string->ptr, sub);
}

VSTD_STATIC char *vstd_string_find_last(const _VSTD_String *string,
                                        const char *sub) {
  if (!sub[0]) {
    return NULL;
  }

  char *pos = NULL;
  for (char *ptr = string->ptr; (ptr = strstr(ptr, sub)) != NULL; ptr += 1) {
    pos = ptr;
  }

  return pos;
}

VSTD_INLINE isize vstd_string_compare(const _VSTD_String *string,
                                      const char *str) {
  return strcmp(string->ptr, str);
}

VSTD_STATIC void vstd_string_free(_VSTD_String *string) {
  free(string->ptr);
  string->len = 0;
  string->cap = 0;
}

VSTD_INLINE void _vstd_string_realloc(_VSTD_String *string) {
  string->cap *= 2;
  string->ptr = (char *)realloc(string->ptr, sizeof(char) * string->cap);
}

#endif // VSTD_H_
