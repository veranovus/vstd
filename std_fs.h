#pragma once

#include "std_common.h"
#include "std_string.h"
#include "std_vector.h"

#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

static const usize _STD_FS_INITIAL_BUFFER_CAPACITY = 128;

// STD_FS
// ======

// Read the whole file in path into a string, returns a null string if it fails
// to open the file.
STD_STATIC String std_fs_read_file(const char *path) {
  FILE *f = fopen(path, "r");

  if (!f) {
#ifdef DEBUG
    fprintf(stderr, "Failed to open file at `%s` to read.\n", path);
    perror("ERROR @std_fs_read_file");
#endif
    return _std_string_null();
  }

  String buff = std_string_with_capacity(_STD_FS_INITIAL_BUFFER_CAPACITY);

  char c;
  while ((c = fgetc(f)) != EOF) {
    std_string_push(&buff, c);
  }

  fclose(f);
  return buff;
}

// Reads the contents of the directory at the given path, returns an empty
// vector if fails.
STD_STATIC STD_Vector(String) std_fs_read_dir(const char *path) {
  DIR *d;
  struct dirent *dir;

  d = opendir(path);
  if (!d) {
#ifdef DEBUG
    fprintf(stderr, "ERROR: Failed to read directory at `%s`\n.", path);
    perror("ERROR @std_fs_read_dir");
#endif
    return (struct _STDVector){};
  }

  STD_Vector(String) vec = std_vector_new(String);
  while ((dir = readdir(d)) != NULL) {
    std_vector_push(String, vec, std_string_from(dir->d_name));
  }
  closedir(d);

  return vec;
}

// Writes the contents into a file, if file doesn't exist creates it.
STD_STATIC usize std_fs_write_file(const char *path, const char *content) {
  FILE *f = fopen(path, "w");

  if (!f) {
#ifdef DEBUG
    fprintf(stderr, "Failed to open file at `%s` to write.\n", path);
    perror("ERROR @std_fs_write_file");
#endif
    return errno;
  }

  i32 rc = fputs(content, f);
  if (rc == EOF) {
#ifdef DEBUG
    fprintf(stderr, "Failed to write to file at `%s`.\n", path);
    perror("ERROR @std_fs_write_file");
#endif
    return errno;
  }

  fclose(f);
  return 0;
}

STD_STATIC usize std_fs_create_dir(const char *path) {
  if (mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO) == -1) {
#ifdef DEBUG
    fprintf(stderr, "Failed to create directory at `%s`.\n", path);
    perror("ERROR @std_fs_create_dir");
#endif
    return errno;
  }

  return 0;
}
