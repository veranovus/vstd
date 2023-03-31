#pragma once

#include "std_common.h"

#define _STD_VECTOR_INITIAL_CAPACITY 1

struct _STDVector {
    void *ptr;
    usize len;
    usize cap;
};

#define STDVector(type) struct _STDVector

#define std_vector_new(type)                                                   \
  std_vector_with_capacity(type, _STD_VECTOR_INITIAL_CAPACITY)

#define std_vector_with_capacity(type, capacity)                               \
  (STDVector(type)) {                                                          \
    .ptr = malloc(sizeof(type) * capacity), .cap = capacity, .len = 0          \
  }

#define std_vector_from(type, var, ...)                                        \
  (STDVector(type)){};                                                         \
  {                                                                            \
    type temp[] = __VA_ARGS__;                                                 \
    var.ptr = malloc(sizeof(temp));                                            \
    memcpy(var.ptr, temp, sizeof(temp));                                       \
    var.cap = sizeof(temp) / sizeof(type);                                     \
    var.len = var.cap;                                                         \
  }                                                                            \
  NULL

#define std_vector_clone(type, var, other)                                     \
  (STDVector(type)){};                                                         \
  {                                                                            \
    var.ptr = malloc(sizeof(type) * other.cap);                                \
    memcpy(var.ptr, other.ptr, sizeof(type) * other.len);                      \
    var.cap = other.cap;                                                       \
    var.len = other.len;                                                       \
  }                                                                            \
  NULL

#define std_vector_get(type, vec, index) (type)((type *)vec.ptr)[index]

#define std_vector_set(type, vec, index, item) ((type *)vec.ptr)[index] = item

#define std_vector_push(type, vec, item)                                       \
  {                                                                            \
    if (vec.len + 1 >= vec.cap) {                                              \
      vec.cap *= 2;                                                            \
      vec.ptr = realloc(vec.ptr, vec.cap * sizeof(type));                      \
    }                                                                          \
    std_vector_set(type, vec, vec.len, item);                                  \
    vec.len++;                                                                 \
  }                                                                            \
  NULL

#define std_vector_remove(type, vec, index)                                    \
  {                                                                            \
    if (index != vec.len - 1) {                                                \
      type *ptr = vec.ptr + index;                                             \
      memmove(ptr, ptr + 1, (vec.len - (index - 1)) * sizeof(type));           \
    }                                                                          \
    vec.len--;                                                                 \
  }                                                                            \
  NULL

#define std_vector_clear(type, vec) vec.len = 0

#define std_vector_free(type, vec)                                             \
  {                                                                            \
    free(vec.ptr);                                                             \
    vec.ptr = NULL;                                                            \
    vec.cap = 0;                                                               \
    vec.len = 0;                                                               \
  }                                                                            \
  NULL
