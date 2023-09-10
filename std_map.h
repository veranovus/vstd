#pragma once

#include "std_common.h"
#include "std_string.h"
#include "std_vector.h"

struct _STDMap {
  struct _STDVector keys;
  struct _STDVector vals;
  void *func_ptr;
  isize cache;
};

// STD_Map
// =======

#define STD_Map(k_t, v_t) struct _STDMap

#define std_map_new(k_t, v_t, condition)                                       \
  (STD_Map(k_t, v_t)) {                                                        \
    .keys = std_vector_new(k_t), .vals = std_vector_new(v_t),                  \
    .func_ptr = condition, .cache = -1,                                        \
  }

#define std_map_contains(k_t, v_t, map, k, out)                                \
  {                                                                            \
    *(out) = false;                                                            \
    map.cache = -1;                                                            \
    for (k_t *_ptr = map.keys.ptr;                                             \
         _ptr < ((k_t *)map.keys.ptr) + map.keys.len; ++_ptr) {                \
      if (((bool (*)(k_t *, k_t *))map.func_ptr)(_ptr, k)) {                   \
        *(out) = true;                                                         \
        map.cache = _ptr - ((k_t *)map.keys.ptr);                              \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
  }                                                                            \
  NULL

#define std_map_set(k_t, v_t, map, k, v)                                       \
  {                                                                            \
    bool exists;                                                               \
    std_map_contains(k_t, v_t, map, &k, &exists);                              \
    if (!exists) {                                                             \
      std_vector_push(k_t, map.keys, k);                                       \
      std_vector_push(v_t, map.vals, v);                                       \
    } else {                                                                   \
      std_vector_set(v_t, map.vals, map.cache, v);                             \
    }                                                                          \
  }                                                                            \
  NULL

#define std_map_get(k_t, v_t, map, k, out)                                     \
  {                                                                            \
    bool exists;                                                               \
    std_map_contains(k_t, v_t, map, &k, &exists);                              \
    if (!exists) {                                                             \
      out = NULL;                                                              \
    } else {                                                                   \
      out = &(std_vector_get(v_t, map.vals, map.cache));                       \
    }                                                                          \
  }                                                                            \
  NULL

#define std_map_remove(k_t, v_t, map, k)                                       \
  {                                                                            \
    bool exists;                                                               \
    std_map_contains(k_t, v_t, map, &k, &exists);                              \
    if (exists) {                                                              \
      std_vector_remove(k_t, map.keys, map.cache);                             \
      std_vector_remove(v_t, map.vals, map.cache);                             \
    }                                                                          \
  }                                                                            \
  NULL

#define std_map_iter(k_t, v_t, map, ...)                                       \
  {                                                                            \
    struct _STDMapPair {                                                       \
      k_t *key;                                                                \
      v_t *val;                                                                \
    };                                                                         \
    for (usize _$i = 0; _$i < map.keys.len; ++_$i) {                           \
      struct _STDMapPair _iter = (struct _STDMapPair){                         \
          .key = &(std_vector_get(k_t, map.keys, _$i)),                        \
          .val = &(std_vector_get(v_t, map.vals, _$i)),                        \
      };                                                                       \
      __VA_ARGS__;                                                             \
    }                                                                          \
  }                                                                            \
  NULL

#define std_map_clear(k_t, v_t, map)                                           \
  {                                                                            \
    std_vector_clear(k_t, map.keys);                                           \
    std_vector_clear(v_t, map.vals);                                           \
    map.cache = -1;                                                            \
  }

#define std_map_free(k_t, v_t, map)                                            \
  {                                                                            \
    std_vector_free(k_t, map.keys);                                            \
    std_vector_free(v_t, map.vals);                                            \
    map.cache = -1;                                                            \
  }                                                                            \
  NULL

#ifdef _STD_MAP_PREDEFINED_CONDITIONS
STD_STATIC bool std_map_condition_isize(const isize *a, const isize *b) {
  return *(a) == *(b);
}

STD_STATIC bool std_map_condition_usize(const usize *a, const usize *b) {
  return *(a) == *(b);
}

STD_STATIC bool std_map_condition_string(const String *a, const String *b) {
  return std_string_compare(a, b->ptr) == STD_STRING_EQUAL;
}

STD_STATIC bool std_map_condition_void(const void **a, const void **b) {
  return *a == *b;
}
#endif
