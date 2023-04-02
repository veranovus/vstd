#pragma once

#include "std_common.h"
#include <stdarg.h>

#define STD_STRING_EQUAL 0

static const usize _STD_STRING_INITIAL_CAPACITY = 1;

typedef struct {
  char *ptr;
  usize len;
  usize cap;
} String;

static inline String *_std_string_realloc(String *s) {
  s->cap *= 2;
  s->ptr = (char *)realloc(s->ptr, sizeof(char) * s->cap);
  return s;
}

static inline String _std_string_null() {
  return (String){
      .ptr = NULL,
      .cap = 0,
      .len = 0,
  };
}

static String std_string_with_capacity(usize cap) {
  return (String){
      .ptr = (char *)calloc(cap, sizeof(char)), .cap = cap, .len = 0};
}

static String std_string_new() {
  return std_string_with_capacity(_STD_STRING_INITIAL_CAPACITY);
}

static String std_string_from(const char *str) {
  usize len = strlen(str);
  String s = std_string_with_capacity(len + 1);

  strcpy(s.ptr, str);
  s.len = len;

  return s;
}

static String std_string_clone(const String *s) {
  String clone = std_string_with_capacity(s->cap);

  memcpy(clone.ptr, s->ptr, sizeof(char) * s->len);
  clone.len = s->len;

  clone.ptr[clone.len] = '\0';
  return clone;
}

static String std_string_format(const char *fmt, ...) {
  va_list argptr;
  va_start(argptr, fmt);

  usize len = vsnprintf(NULL, 0, fmt, argptr);
  String s = std_string_with_capacity(len + 1);

  vsnprintf(s.ptr, s.cap, fmt, argptr);
  s.len = len;

  va_end(argptr);
  return s;
}

static String std_string_push(String *s, char c) {
  if (s->len + 1 >= s->cap) {
    s = _std_string_realloc(s);
  }

  s->ptr[s->len] = c;
  s->len++;

  s->ptr[s->len] = '\0';
  return *s;
}

static String std_string_push_str(String *s, const char *str) {
  usize len = strlen(str);

  while (s->len + len >= s->cap) {
    s = _std_string_realloc(s);
  }

  strcpy(s->ptr + s->len, str);
  s->len += len;

  s->ptr[s->len] = '\0';
  return *s;
}

static String std_string_remove_at(String *s, usize index, usize len) {
  usize move_size = s->len - ((s->ptr + index + len) - s->ptr);

  if (move_size > 0) {
    memmove(s->ptr + index, s->ptr + index + len, move_size);
  }
  s->len -= len;

  s->ptr[s->len] = '\0';
  return *s;
}

static String std_string_remove(String *s, const char *sub) {
  if (!sub[0]) {
    return *s;
  }

  usize len = strlen(sub);
  char *ptr = strstr(s->ptr, sub);
  if (!ptr) {
    return *s;
  }

  return std_string_remove_at(s, (usize)(ptr - s->ptr), len);
}

static inline char *std_string_find_first(const String *s, const char *sub) {
  return strstr(s->ptr, sub);
}

static char *std_string_find_last(const String *s, const char *sub) {
  if (!sub[0]) {
    return NULL;
  }

  char *pos = NULL;
  for (char *ptr = s->ptr; (ptr = strstr(ptr, sub)) != NULL; ptr += 1) {
    pos = ptr;
  }
  return pos;
}

static inline isize std_string_compare(const String *s, const char *str) {
  return strcmp(s->ptr, str);
}

static void std_string_free(String s) { free(s.ptr); }
