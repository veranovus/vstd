#pragma once

#include "vstd_common.h"
#include "vstd_string.h"
#include "vstd_vector.h"

#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

// VSTD_FS
// =======

// Read the whole file in path into a string, returns a null string if it fails
// to open the file.
VSTD_STATIC String vstd_fs_read_file(const char *path) {
  /* TODO: Potential Improvement
   * Maybe change "r" to "rb" to make this function more general purpose.
   */
  FILE *f = fopen(path, "r");

  if (!f) {
#ifdef DEBUG
    fprintf(stderr, "Failed to open file at `%s` to read.\n", path);
    perror("ERROR @vstd_fs_read_file");
#endif
    return _vstd_string_null();
  }

  fseek(f, 0, SEEK_END);
  usize size = ftell(f);
  fseek(f, 0, SEEK_SET);

  String buff = vstd_string_with_capacity(size + 1);

  usize read = fread(buff.ptr, size, 1, f);
  if (read != 1) {
    fprintf(stderr, "Failed to read file at `%s`.\n", path);
    perror("ERROR @vstd_fs_read_file");
  }
  fclose(f);

  buff.len = size;
  buff.ptr[buff.len] = '\0';

  return buff;
}

// Reads the contents of the directory at the given path, returns an empty
// vector if fails.
VSTD_STATIC VSTD_Vector(String) vstd_fs_read_dir(const char *path) {
  DIR *d;
  struct dirent *dir;

  d = opendir(path);
  if (!d) {
#ifdef DEBUG
    fprintf(stderr, "ERROR: Failed to read directory at `%s`\n.", path);
    perror("ERROR @vstd_fs_read_dir");
#endif
    return (struct _VSTDVector){};
  }

  VSTD_Vector(String) vec = vstd_vector_new(String);
  while ((dir = readdir(d)) != NULL) {
    vstd_vector_push(String, vec, vstd_string_from(dir->d_name));
  }
  closedir(d);

  return vec;
}

// Writes the contents into a file, if file doesn't exist creates it.
VSTD_STATIC usize vstd_fs_write_file(const char *path, const char *content) {
  FILE *f = fopen(path, "w");

  if (!f) {
#ifdef DEBUG
    fprintf(stderr, "Failed to open file at `%s` to write.\n", path);
    perror("ERROR @vstd_fs_write_file");
#endif
    return errno;
  }

  i32 rc = fputs(content, f);
  if (rc == EOF) {
#ifdef DEBUG
    fprintf(stderr, "Failed to write to file at `%s`.\n", path);
    perror("ERROR @vstd_fs_write_file");
#endif
    return errno;
  }

  fclose(f);
  return 0;
}

VSTD_STATIC usize vstd_fs_create_dir(const char *path) {
  if (mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO) == -1) {
#ifdef DEBUG
    fprintf(stderr, "Failed to create directory at `%s`.\n", path);
    perror("ERROR @vstd_fs_create_dir");
#endif
    return errno;
  }

  return 0;
}
