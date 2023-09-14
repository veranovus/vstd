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

#include <dirent.h>
#include <errno.h>
#include <inttypes.h>
#include <memory.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

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
 *   _VSTD_String
 *
 * @description:
 *   String implementation, this struct never allocates memory for itself, but
 *   only for its underlying pointer. When passing an instance of this type, its
 *   safe to pass it as it is to a function, as long as that function doesn't
 *   modify the string's pointer or other properties. Otherwise you should pass
 *   it as a reference, or you may lose the underlying pointer.
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
 *   Creates a new empty _VSTD_String object, this function internally calls
 *   vstd_string_with_capacity with VSTD_STRING_INITIAL_CAP as the argument.
 *
 * @return
 *   New empty _VSTD_String with capacity of VSTD_STRING_INITIAL_CAP.
 *
 * */
VSTD_INLINE _VSTD_String vstd_string_new();

/*****************************************************************************
 *
 * @function
 *   vstd_string_with_capacity
 *
 * @description
 *   Creates a new empty _VSTD_String object with the given capacity.
 *
 * @param[in]
 *   cap : Capacity for the new _VSTD_String.
 *
 * @return
 *   New empty _VSTD_String with given capacity.
 *
 * */
VSTD_STATIC _VSTD_String vstd_string_with_capacity(usize cap);

/*****************************************************************************
 *
 * @function
 *   vstd_string_from
 *
 * @description
 *   Creates a new null terminated _VSTD_String from given C string.
 *
 * @param[in]
 *   str : Null terminated C string to copy from.
 *
 * @return
 *   New null terminated _VSTD_String with the same contents as the given C
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
 *   Clones the given _VSTD_String and returns the clone.
 *
 * @param[in]
 *   string : _VSTD_String to clone.
 *
 * @return
 *   New _VSTD_String identical to the given _VSTD_String.
 *
 * */
VSTD_STATIC _VSTD_String vstd_string_clone(const _VSTD_String *string);

/*****************************************************************************
 *
 * @function
 *   vstd_string_format
 *
 * @description
 *   Creates a new _VSTD_String using the formatted contents.
 *
 * @param[in]
 *   fmt : Null terminated C string specifying how to interpret the data.
 * @param[in]
 *   ... : Arguments specifying the data to format.
 *
 * @return
 *   New _VSTD_String with the formatted contents.
 *
 * */
VSTD_STATIC _VSTD_String vstd_string_format(const char *fmt, ...);

/*****************************************************************************
 *
 * @function
 *   vstd_string_push
 *
 * @description
 *   Adds the given character to the end of given _VSTD_String, this function
 *   may resize the given _VSTD_String.
 *
 * @param[in]
 *   string : _VSTD_String to modify.
 * @param[in]
 *   c : Character to push.
 *
 * @return
 *   Pointer to same modified _VSTD_String.
 *
 * */
VSTD_STATIC _VSTD_String *vstd_string_push(_VSTD_String *string, char c);

/*****************************************************************************
 *
 * @function
 *   vstd_string_push_str
 *
 * @description
 *   Adds the C string to the end of given _VSTD_String, this function
 *   may resize the given _VSTD_String.
 *
 * @param[in]
 *   string : _VSTD_String to modify.
 * @param[in]
 *   str : Null terminated C string to push.
 *
 * @return
 *   Pointer to same modified _VSTD_String.
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
 *   _VSTD_String. Substring is only removed once.
 *
 * @param[in]
 *   string : _VSTD_String to modify.
 * @param[in]
 *   sub : Null terminated C string to remove.
 *
 * @return
 *   Pointer to same _VSTD_String, it may or may not be modified depending on
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
 *   _VSTD_String.
 *
 * @param[in]
 *   string : _VSTD_String to modify.
 * @param[in]
 *   index : Starting index for removal.
 * @param[in]
 *   len : Number of characters to remove.
 *
 * @return
 *   Pointer to same modified _VSTD_String.
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
 *   _VSTD_String. Underlying implementation of this function just calls strstr
 *   from standard library. Returns a NULL pointer if it fails.
 *
 * @param[in]
 *   string : _VSTD_String to search.
 * @param[in]
 *   sub : Null terminated C string to search for.
 *
 * @return
 *   Pointer to the start of the C string's first occurrence inside the given
 *   _VSTD_String or a NULL pointer.
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
 *   _VSTD_String. Returns a NULL pointer if it fails.
 *
 * @param[in]
 *   string : _VSTD_String to search.
 * @param[in]
 *   sub : Null terminated C string to search for.
 *
 * @return
 *   Pointer to the start of the C string's last occurrence inside the given
 *   _VSTD_String or a NULL pointer.
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
 *   Compares the given C string with the given _VSTD_String. This function is
 *   only there for API consistency, underlying implementation of this function
 *   just calls strcmp thus, behaviour is identical to that function. For more
 *   information about strcmp you can check:
 *   https://pubs.opengroup.org/onlinepubs/9699919799.2016edition/functions/strcmp.html
 *
 * @param[in]
 *   string : _VSTD_String to compare.
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
 *   Frees the memory allocated for given _VSTD_String's underlying pointer.
 *
 * @param[in]
 *   string : _VSTD_String to free.
 *
 * */
VSTD_STATIC void vstd_string_free(_VSTD_String *string);

/*****************************************************************************
 *
 * @function
 *   _vstd_string_realloc
 *
 * @description
 *   Doubles the given _VSTD_String's capacity and reallocates the underlying
 *   pointer. This is a helper function and it should only be used by the vstd
 *   library functions.
 *
 * @param[in]
 *   string : _VSTD_String to reallocate.
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

/*****************************************************************************
 *
 * @section
 *   VSTD Vector
 *
 * */

/*****************************************************************************
 *
 * @type:
 *   _VSTD_Vector
 *
 * @description:
 *   Vector implementation, this struct never allocates memory for itself, but
 *   only for its underlying pointer. When passing an instance of this type its
 *   safe to pass it as it is to a function, as long as that function doesn't
 *   modify the vector's pointer or other properties. Otherwise you should pass
 *   it as a reference, or you may lose the underlying pointer. Even though
 *   functions that are defined for _VSTD_Vector in this library modifies those
 *   values, they are an exception to this rule since they are not ordinary
 *   functions but macros instead, and passing by reference to those macros will
 *   break them.
 *
 * */
struct _VSTD_Vector {
  void *ptr;
  usize len;
  usize cap;
};

#ifdef VSTD_VECTOR_STRIP_PREFIX
#define Vector(type) struct _VSTD_Vector
#else
#define VSTD_Vector(type) struct _VSTD_Vector
#endif

#ifndef VSTD_VECTOR_INITIAL_CAP
#define VSTD_VECTOR_INITIAL_CAP 1
#endif

/*****************************************************************************
 *
 * @macro
 *   vstd_vector_new
 *
 * @description
 *   Creates a new empty _VSTD_Vector object, this macro internally just calls
 *   vstd_vector_with_capacity with VSTD_VECTOR_INITIAL_CAP as the argument.
 *
 * @param[in]
 *   type : Type of the _VSTD_Vector's data.
 *
 * @return
 *   New empty _VSTD_Vector with capacity of VSTD_VECTOR_INITIAL_CAP.
 *
 * */
#define vstd_vector_new(type)                                                  \
  vstd_vector_with_capacity(type, VSTD_VECTOR_INITIAL_CAP)

/*****************************************************************************
 *
 * @macro
 *   vstd_vector_with_capacity
 *
 * @description
 *   Creates a new empty _VSTD_Vector object with the given capacity.
 *
 * @param[in]
 *   type : Type of the _VSTD_Vector's data.
 * @param[in]
 *   capacity : Capacity of the new _VSTD_Vector.
 *
 * @return
 *   New empty _VSTD_Vector with given capacity.
 *
 * */
#define vstd_vector_with_capacity(type, capacity)                              \
  (struct _VSTD_Vector) {                                                      \
    .ptr = malloc(sizeof(type) * capacity), .cap = capacity, .len = 0          \
  }

/*****************************************************************************
 *
 * @macro
 *   vstd_vector_from
 *
 * @description
 *   Creates a new _VSTD_Vector containing the data in the initializer list.
 *   Data is first assigned to a temporary variable then copied to the var.
 *   Returns a non-allocated _VSTD_Vector to allow single line initializations.
 *
 * @param[in]
 *   type : Type of the _VSTD_Vector's data.
 * @param[out]
 *   var : Variable to assign the vector.
 * @param[in]
 *   ... : Initializer list
 *
 * @return
 *   New non-allocated _VSTD_Vector.
 *
 * */
#define vstd_vector_from(type, var, ...)                                       \
  (struct _VSTD_Vector){};                                                     \
  {                                                                            \
    type temp[] = __VA_ARGS__;                                                 \
    var.ptr = malloc(sizeof(temp));                                            \
    memcpy(var.ptr, temp, sizeof(temp));                                       \
    var.cap = sizeof(temp) / sizeof(type);                                     \
    var.len = var.cap;                                                         \
  }                                                                            \
  NULL

/*****************************************************************************
 *
 * @macro
 *   vstd_vector_clone
 *
 * @description
 *   Clones the given _VSTD_Vector and assigns the clone to var. Returns a
 *   non-allocated _VSTD_Vector to allow single line initializations.
 *
 * @param[in]
 *   type : Type of the _VSTD_Vector's data.
 * @param[out]
 *   var : Variable to assign the clone.
 * @param[in]
 *   other : _VSTD_Vector to clone.
 *
 * @return
 *   New non-allocated _VSTD_Vector.
 *
 * */
#define vstd_vector_clone(type, var, other)                                    \
  (struct _VSTD_Vector){};                                                     \
  {                                                                            \
    var.ptr = malloc(sizeof(type) * other.cap);                                \
    memcpy(var.ptr, other.ptr, sizeof(type) * other.len);                      \
    var.cap = other.cap;                                                       \
    var.len = other.len;                                                       \
  }                                                                            \
  NULL

/*****************************************************************************
 *
 * @macro
 *   vstd_vector_get
 *
 * @description
 *   Returns the item at the given index. This is the intended way to access an
 *   element from _VSTD_Vector since _VSTD_Vector needs to be casted to desired
 *   type first.
 *
 * @param[in]
 *   type : Type of the _VSTD_Vector's data.
 * @param[in]
 *   vec : _VSTD_Vector to retrieve the item from.
 * @param[in]
 *   index : Index of the item.
 *
 * @return
 *   Item at given index.
 *
 * */
#define vstd_vector_get(type, vec, index) ((type *)vec.ptr)[index]

/*****************************************************************************
 *
 * @macro
 *   vstd_vector_set
 *
 * @description
 *   Replaces the item at the given index with the given item.
 *
 * @param[in]
 *   type : Type of the _VSTD_Vector's data.
 * @param[in]
 *   vec : _VSTD_Vector to modify.
 * @param[in]
 *   index : Target index to replace.
 * @param[in]
 *   item : Item to replace with.
 *
 * */
#define vstd_vector_set(type, vec, index, item) ((type *)vec.ptr)[index] = item

/*****************************************************************************
 *
 * @macro
 *   vstd_vector_push
 *
 * @description
 *   Pushes a new item to the end of the given _VSTD_Vector.
 *
 * @param[in]
 *   type : Type of the _VSTD_Vector's data.
 * @param[in]
 *   vec : _VSTD_Vector to modify.
 * @param[in]
 *   item : Item to add.
 *
 * */
#define vstd_vector_push(type, vec, item)                                      \
  {                                                                            \
    if (vec.len + 1 >= vec.cap) {                                              \
      vec.cap *= 2;                                                            \
      vec.ptr = realloc(vec.ptr, vec.cap * sizeof(type));                      \
    }                                                                          \
    vstd_vector_set(type, vec, vec.len, item);                                 \
    vec.len++;                                                                 \
  }                                                                            \
  NULL

/*****************************************************************************
 *
 * @macro
 *   vstd_vector_remove
 *
 * @description
 *   Removes the item at the given index from _VSTD_Vector. If item is not at
 *   the end of the _VSTD_Vector remaining items are shifted to left.
 *
 * @param[in]
 *   type : Type of the _VSTD_Vector's data.
 * @param[in]
 *   vec : _VSTD_Vector to modify.
 * @param[in]
 *   index : Index to remove.
 *
 * */
#define vstd_vector_remove(type, vec, index)                                   \
  {                                                                            \
    if (index != vec.len - 1) {                                                \
      type *ptr = ((type *)vec.ptr) + index;                                   \
      memmove(ptr, ptr + 1, (vec.len - (index + 1)) * sizeof(type));           \
    }                                                                          \
    vec.len--;                                                                 \
  }                                                                            \
  NULL

/*****************************************************************************
 *
 * @macro
 *   vstd_vector_iter
 *
 * @description
 *   Helper function to easily iterate trough the length of the _VSTD_Vector.
 *   In every iteration you can access the current item from _$iter and current
 *   index from _$i.
 *
 * @param[in]
 *   type : Type of the _VSTD_Vector's data.
 * @param[in]
 *   vec : _VSTD_Vector to iterate.
 * @param[in]
 *   ... : A single function or a block of code to execute every iteration.
 *
 * */
#define vstd_vector_iter(type, vec, ...)                                       \
  for (type *_$iter = vec.ptr; _$iter < ((type *)vec.ptr) + vec.len;           \
       ++_$iter) {                                                             \
    usize _$i = ((void *)_$iter - vec.ptr) / sizeof(type);                     \
    __VA_ARGS__;                                                               \
  }                                                                            \
  NULL

/*****************************************************************************
 *
 * @macro
 *   vstd_vector_clear

 * @description
 *   Set's given _VSTD_Vector's length to 0, basically soft resetting it. This
 *   doesn't actually modify or free the underlying pointer.
 *
 * @param[in]
 *   type : Type of the _VSTD_Vector's data.
 * @param[in]
 *   vec : _VSTD_Vector to clear.
 *
 * */
#define vstd_vector_clear(type, vec) vec.len = 0

/*****************************************************************************
 *
 * @macro
 *   vstd_vector_free
 *
 * @description
 *   Frees the underlying pointer of the given _VSTD_Vector.
 *
 * @param[in]
 *   type : Type of the _VSTD_Vector's data.
 * @param[in]
 *   vec : _VSTD_Vector to free.
 *
 * */
#define vstd_vector_free(type, vec)                                            \
  {                                                                            \
    free(vec.ptr);                                                             \
    vec.ptr = NULL;                                                            \
    vec.cap = 0;                                                               \
    vec.len = 0;                                                               \
  }                                                                            \
  NULL

/*****************************************************************************
 *
 * @section
 *   VSTD Map
 *
 * */

/*****************************************************************************
 *
 * @type
 *   _VSTD_Map
 *
 * @description
 *   Map implementation, this type doesn't allocate any memory for itself, but
 *   its underlying _VSTD_Vector's allocate memory for their pointers. Thus it's
 *   safe to pass this type to functions as it is, as long as said function
 *   doesn't alter any of the properties of _VSTD_Map or its _VSTD_Vector's.
 *   Otherwise you should pass it as reference or you may lose the underlying
 *   pointers of the _VSTD_Vector's.
 *
 * */
struct _VSTD_Map {
  struct _VSTD_Vector keys;
  struct _VSTD_Vector vals;
  void *func_ptr;
  isize cache;
};

#ifdef VSTD_MAP_STRIP_PREFIX
#define Map(k, v) struct _VSTD_Map
#else
#define VSTD_Map(k, v) struct _VSTD_Map
#endif

/*****************************************************************************
 *
 * @macro
 *   vstd_map_new
 *
 * @description
 *   Creates a new empty _VSTD_Map.
 *
 * @param[in]
 *   k : Type of the keys stored in _VSTD_Map.
 * @param[in]
 *   v : Type of the values stored in _VSTD_Map.
 * @param[in]
 *   condition : Condition function that will be used to compare keys.
 *
 * @return
 *   New empty _VSTD_Map.
 *
 * */
#define vstd_map_new(k, v, condition)                                          \
  (struct _VSTD_Map) {                                                         \
    .keys = vstd_vector_new(k), .vals = vstd_vector_new(v),                    \
    .func_ptr = condition, .cache = -1,                                        \
  }

/*****************************************************************************
 *
 * @macro
 *   vstd_map_contains
 *
 * @description
 *   Searches the given _VSTD_Map for given key and stores the result in the
 *   given variable.
 *
 * @param[in]
 *   k : Type of the keys stored in _VSTD_Map.
 * @param[in]
 *   v : Type of the values stored in _VSTD_Map.
 * @param[in]
 *   map : Map to search for the key.
 * @param[in]
 *   key : Key to search for.
 * @param[out]
 *   result : Reference to the variable that the result will be stored in.
 *
 * */
#define vstd_map_contains(k, v, map, key, result)                              \
  {                                                                            \
    *(result) = false;                                                         \
    map.cache = -1;                                                            \
    for (k *_ptr = map.keys.ptr; _ptr < ((k *)map.keys.ptr) + map.keys.len;    \
         ++_ptr) {                                                             \
      if (((bool (*)(k *, k *))map.func_ptr)(_ptr, key)) {                     \
        *(result) = true;                                                      \
        map.cache = _ptr - ((k *)map.keys.ptr);                                \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
  }                                                                            \
  NULL

#endif // VSTD_H_
