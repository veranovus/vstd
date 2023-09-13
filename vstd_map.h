#pragma once

#include "vstd_common.h"
#include "vstd_string.h"
#include "vstd_vector.h"

struct _VSTDMap {
  struct _VSTDVector keys;
  struct _VSTDVector vals;
  void *func_ptr;
  isize cache;
};

// VSTD_Map
// ========

#define VSTD_Map(k_t, v_t) struct _VSTDMap

#define vstd_map_new(k_t, v_t, condition)                                      \
  (VSTD_Map(k_t, v_t)) {                                                       \
    .keys = vstd_vector_new(k_t), .vals = vstd_vector_new(v_t),                \
    .func_ptr = condition, .cache = -1,                                        \
  }

#define vstd_map_contains(k_t, v_t, map, k, out)                               \
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

#define vstd_map_set(k_t, v_t, map, k, v)                                      \
  {                                                                            \
    bool exists;                                                               \
    vstd_map_contains(k_t, v_t, map, &k, &exists);                             \
    if (!exists) {                                                             \
      vstd_vector_push(k_t, map.keys, k);                                      \
      vstd_vector_push(v_t, map.vals, v);                                      \
    } else {                                                                   \
      vstd_vector_set(v_t, map.vals, map.cache, v);                            \
    }                                                                          \
  }                                                                            \
  NULL

#define vstd_map_get(k_t, v_t, map, k, out)                                    \
  {                                                                            \
    bool exists;                                                               \
    vstd_map_contains(k_t, v_t, map, &k, &exists);                             \
    if (!exists) {                                                             \
      out = NULL;                                                              \
    } else {                                                                   \
      out = &(vstd_vector_get(v_t, map.vals, map.cache));                      \
    }                                                                          \
  }                                                                            \
  NULL

#define vstd_map_remove(k_t, v_t, map, k)                                      \
  {                                                                            \
    bool exists;                                                               \
    vstd_map_contains(k_t, v_t, map, &k, &exists);                             \
    if (exists) {                                                              \
      vstd_vector_remove(k_t, map.keys, map.cache);                            \
      vstd_vector_remove(v_t, map.vals, map.cache);                            \
    }                                                                          \
  }                                                                            \
  NULL

#define vstd_map_iter(k_t, v_t, map, ...)                                      \
  {                                                                            \
    struct _VSTDMapPair {                                                      \
      k_t *key;                                                                \
      v_t *val;                                                                \
    };                                                                         \
    for (usize _$i = 0; _$i < map.keys.len; ++_$i) {                           \
      struct _VSTDMapPair _$iter = (struct _VSTDMapPair){                      \
          .key = &(vstd_vector_get(k_t, map.keys, _$i)),                       \
          .val = &(vstd_vector_get(v_t, map.vals, _$i)),                       \
      };                                                                       \
      __VA_ARGS__;                                                             \
    }                                                                          \
  }                                                                            \
  NULL

#define vstd_map_clear(k_t, v_t, map)                                          \
  {                                                                            \
    vstd_vector_clear(k_t, map.keys);                                          \
    vstd_vector_clear(v_t, map.vals);                                          \
    map.cache = -1;                                                            \
  }

#define vstd_map_free(k_t, v_t, map)                                           \
  {                                                                            \
    vstd_vector_free(k_t, map.keys);                                           \
    vstd_vector_free(v_t, map.vals);                                           \
    map.cache = -1;                                                            \
  }                                                                            \
  NULL

// Predefined condition functions
// ------------------------------

#ifdef _VSTD_MAP_PREDEFINED_CONDITIONS
VSTD_STATIC bool vstd_map_condition_isize(const isize *a, const isize *b) {
  return *(a) == *(b);
}

VSTD_STATIC bool vstd_map_condition_usize(const usize *a, const usize *b) {
  return *(a) == *(b);
}

VSTD_STATIC bool vstd_map_condition_string(const String *a, const String *b) {
  return vstd_string_compare(a, b->ptr) == VSTD_STRING_EQUAL;
}

VSTD_STATIC bool vstd_map_condition_void(const void **a, const void **b) {
  return *a == *b;
}
#endif
