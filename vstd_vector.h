#pragma once

#include "vstd_common.h"

#define _VSTD_VECTOR_INITIAL_CAPACITY 1

struct _VSTDVector {
  void *ptr;
  usize len;
  usize cap;
};

// VSTD_Vector
// ==========

#define VSTD_Vector(type) struct _VSTDVector

#define vstd_vector_new(type)                                                  \
  vstd_vector_with_capacity(type, _VSTD_VECTOR_INITIAL_CAPACITY)

#define vstd_vector_with_capacity(type, capacity)                              \
  (VSTD_Vector(type)) {                                                        \
    .ptr = malloc(sizeof(type) * capacity), .cap = capacity, .len = 0          \
  }

#define vstd_vector_from(type, var, ...)                                       \
  (VSTD_Vector(type)){};                                                       \
  {                                                                            \
    type temp[] = __VA_ARGS__;                                                 \
    var.ptr = malloc(sizeof(temp));                                            \
    memcpy(var.ptr, temp, sizeof(temp));                                       \
    var.cap = sizeof(temp) / sizeof(type);                                     \
    var.len = var.cap;                                                         \
  }                                                                            \
  NULL

#define vstd_vector_clone(type, var, other)                                    \
  (VSTD_Vector(type)){};                                                       \
  {                                                                            \
    var.ptr = malloc(sizeof(type) * other.cap);                                \
    memcpy(var.ptr, other.ptr, sizeof(type) * other.len);                      \
    var.cap = other.cap;                                                       \
    var.len = other.len;                                                       \
  }                                                                            \
  NULL

#define vstd_vector_get(type, vec, index) ((type *)vec.ptr)[index]

#define vstd_vector_set(type, vec, index, item) ((type *)vec.ptr)[index] = item

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

#define vstd_vector_remove(type, vec, index)                                   \
  {                                                                            \
    if (index != vec.len - 1) {                                                \
      type *ptr = vec.ptr + index;                                             \
      memmove(ptr, ptr + 1, (vec.len - (index - 1)) * sizeof(type));           \
    }                                                                          \
    vec.len--;                                                                 \
  }                                                                            \
  NULL

#define vstd_vector_iter(type, vec, ...)                                       \
  for (type *_$iter = vec.ptr; _$iter < ((type *)vec.ptr) + vec.len;           \
       ++_$iter) {                                                             \
    usize _$i = ((void *)_$iter - vec.ptr) / sizeof(type);                     \
    __VA_ARGS__;                                                               \
  }                                                                            \
  NULL

#define vstd_vector_clear(type, vec) vec.len = 0

#define vstd_vector_free(type, vec)                                            \
  {                                                                            \
    free(vec.ptr);                                                             \
    vec.ptr = NULL;                                                            \
    vec.cap = 0;                                                               \
    vec.len = 0;                                                               \
  }                                                                            \
  NULL
