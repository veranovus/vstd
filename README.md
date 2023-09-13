# VSTD

Simple utilities to make using C a bit more comfortable.

> Version 0.2.9

## Modules

- [vstd.h](https://github.com/veranovus/vstd/blob/main/vstd.h): Sum of every module below.
- [vstd_common.h](https://github.com/veranovus/vstd/blob/main/vstd_common.h): Common typedefs used by `vstd`.
- [vstd_string.h](https://github.com/veranovus/vstd/blob/main/vstd_string.h): String implementation.
- [vstd_vector.h](https://github.com/veranovus/vstd/blob/main/vstd_vector.h): Vector implementation.
- [vstd_map.h](https://github.com/veranovus/vstd/blob/main/vstd_map.h): Map implementation.
- [vstd_fs.h](https://github.com/veranovus/vstd/blob/main/vstd_fs.h): Everything related to files.
- [vstd_io.h](https://github.com/veranovus/vstd/blob/main/vstd_io.h): Input & Output related utility functions.

## vstd_common.h

Some typedefs for default C types to make them a bit more clear, also include a lot of commonly used C standard library
headers.

```c
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
```

## vstd_string.h

String type to make working with string a bit easier in C. It comes with a lot of predefined helper functions, and more
will be added in the future. This is the only non-native type without a prefix, and it was a deliberate choice since I
planned to make it usable without the rest of the library.

```c
#include <vstd/vstd.h>

int main(i32 argc, char **argv) {
  
  String str = vstd_string_from("`%d` number of arguments supplied.", argc);
  
  printf("%s\n", str.ptr);
  
  vstd_string_free(&str);
  
  return 0;
}
```

## vstd_vector.h

Vector type for dynamic arrays, it stores everything as a void pointer and casts to desired type when an element is
accessed. Here is a simple example for how you can use it.

```c
#include <vstd/vstd.h>

int main(i32 argc, char **argv) {
  
  // Creates a vector from given initializer list
  // You can also use vstd_vector_new or vstd_vector_with_capacity to 
  // create vectors
  VSTD_Vector(i32) vec = vstd_vector_from(i32, vec, {1, 2, 3, 4, 5});
  
  vstd_vector_push(i32, vec, 6);
  
  // Iterate through the vector
  for (i32 i = 0; i < vec.len; ++i) {
    printf("%d - %d\n", i, vstd_vector_get(i32, vec, i));
  }
  
  // You can also iterate using the vstd_vector_iter, 
  // `{}` are not required if you're just going to call one 
  // function inside vstd_vector_iter
  vstd_vector_iter(i32, vec, 
    printf("%d - %d\n", _$i, *_$iter);
  })
  
  vstd_vector_free(i32, vec);
  
  return 0;
}
```

## vstd_map.h

Map type for storing keys and values, it requires you to provide a condition function to check equalities between keys.
There are some predefined conditions for String and native C types.

```c
#include <vstd/vstd.h>

int main(i32 argc, char **argv) {
  
  VSTD_Map(String, i32) map = vstd_map_new(String, i32, vstd_map_condition_string);

  String a = vstd_string_from("Vanessa");
  String b = vstd_string_from("Jane");
  String c = vstd_string_from("Carl");

  vstd_map_set(String, i32, map, a, (i32)a.len);
  vstd_map_set(String, i32, map, b, (i32)b.len);
  vstd_map_set(String, i32, map, c, (i32)c.len);

  // Iterate through the map
  // `{}` are not required if you're just going to call one 
  // function inside vstd_map_iter
  vstd_map_iter(String, i32, map, {
    printf("(Name, Length) : (%s, %d)\n", _$iter.key->ptr, *_$iter.val);

    // Freeing the map doesn't free the values stored in map and since program
    // is almost finished we should free the keys.
    vstd_string_free(_$iter.key);
  });

  vstd_map_free(String, i32, map);
  return 0;
}
```
