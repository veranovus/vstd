#pragma once

#include "std_common.h"
#include "std_string.h"

String std_io_read_line(usize max_char) {
  String line = std_string_with_capacity(max_char);

  fgets(line.ptr, (i32)max_char, stdin);

  usize len = 0;
  for (char *ptr = line.ptr; ptr < line.ptr + line.cap; ++ptr) {
    if (*ptr == '\n' || *ptr == '\0') {
      len = ptr - line.ptr;
      break;
    }
  }

  line.len = len;
  line.ptr[line.len] = '\0';

  return line;
}
