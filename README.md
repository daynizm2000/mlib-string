# MLib String

`mlib_string` is a lightweight dynamic string library written in C.

The library provides a simple string object with automatic memory management and a convenient API for common string operations such as concatenation, formatting, searching, replacing, insertion, and deletion.  
Additionally, it offers a **string literal** type – a non‑owning, zero‑allocation view of a compile‑time or constant string.

The project is written in ISO C and depends only on the standard C library.

---

## Features

- Dynamic string type with custom memory allocator support
- Lightweight string literal type for zero‑allocation string views
- Pluggable memory management (malloc/free or custom allocators)
- Copy and move operations
- String formatting (`printf`-style)
- Concatenation
- Insert and erase operations
- Replace substrings
- Search and reverse search
- Case‑sensitive and case‑insensitive comparison
- Character push/pop
- Prefix and suffix checks
- Uppercase and lowercase conversion
- Capacity management
- Iterator macros

---

## Requirements

- GCC
- GNU Make
- Standard C library

No external dependencies are required.

---

## Building

Build the static library:

```sh
make
```

This produces:

```
libmlib_string.a
```

---

## Using the Library

Include the header:

```c
#include "string.h"
```

Compile your program and link the library:

```sh
gcc main.c -L. -lmlib_string
```

or compile directly with the implementation source:

```sh
gcc main.c string/src/string.c
```

---

## Basic Example

```c
#include "../string/include/string.h"
#include <stdio.h>

int main(void)
{
    mlib_str_t strobj;
    mlib_str_literal_t litobj;

    /* Pass NULL as the third argument to use default malloc/free */
    mlib_str_init(&strobj, "test", NULL);
    mlib_str_literal_set(&litobj, "literal data");

    mlib_str_cat(&strobj, " test");
    mlib_str_replace(&strobj, "test", "test...");
    mlib_str_replace(&strobj, " ", ",");

    printf("%s\n", mlib_str_data(&strobj));
    printf("%s, len: %zu\n", mlib_str_literal_data(&litobj),
           mlib_str_literal_len(&litobj));

    mlib_str_destroy(&strobj);

    return 0;
}
```

Output:

```
test...,test...
literal data, len: 12
```

---

## Custom Memory Allocators

The library allows you to supply your own memory management functions through the `mlib_str_attr_t` structure.  
This is useful for working with custom heaps, memory pools, or tracking allocations.

### mlib_str_attr_t

```c
typedef struct {
    struct {
        void *(*alloc)(size_t size, void *arg);
        void *(*realloc)(void *addr, size_t newsize, void *arg);
        void (*free)(void *addr, void *arg);
    } mem_ops;

    void *private_data;
} mlib_str_attr_t;
```

- `mem_ops.alloc`   – allocation function (signature like `malloc`, but receives `private_data`)
- `mem_ops.realloc` – reallocation function (signature like `realloc`, receives `private_data`)
- `mem_ops.free`    – deallocation function (signature like `free`, receives `private_data`)
- `private_data`    – user‑defined pointer passed to all memory operations

If any function pointer is `NULL`, the library falls back to the standard `malloc`/`realloc`/`free`.

### Passing attributes to `mlib_str_init`

```c
mlib_str_t s;
mlib_str_attr_t attr;

attr.mem_ops.alloc   = my_alloc;
attr.mem_ops.realloc = my_realloc;
attr.mem_ops.free    = my_free;
attr.private_data    = my_context;

mlib_str_init(&s, "data", &attr);
```

Pass `NULL` as the third argument to `mlib_str_init` to use default allocators (and `private_data = NULL`).

The attribute is stored inside the string object and used for all subsequent memory operations. `mlib_str_copy` preserves the source’s attributes, `mlib_str_swap` exchanges them, and `mlib_str_destroy` resets the attribute structure.

Use the macro `mlib_str_prv_data(obj)` to retrieve the `private_data` pointer for external use.

---

## API Overview

### Initialization

```c
mlib_str_init(mlib_str_t *obj, const char *data, const mlib_str_attr_t *attr)
mlib_str_destroy()
mlib_str_clear()
```

Creates, destroys, and clears dynamic string objects.  
`mlib_str_init` takes an optional `attr` pointer; if `NULL`, standard library allocators are used.

---

### String Literal

`mlib_str_literal_t` represents a read‑only view of a constant string. It does **not** own the underlying memory and requires no allocation or destruction.

**Macros**

- `mlib_str_literal_set(obj, literal)` – assigns a string literal to a literal object.
- `mlib_str_literal_data(obj)` – returns the `const char*` data pointer.
- `mlib_str_literal_len(obj)` – returns the length of the literal string.
- `mlib_str_literal_clear(obj)` – resets the literal object to a NULL/empty state.

Example:

```c
mlib_str_literal_t lit;
mlib_str_literal_set(&lit, "hello");
printf("%s, %zu\n", mlib_str_literal_data(&lit), mlib_str_literal_len(&lit));
mlib_str_literal_clear(&lit);
```

---

### Capacity Management

```c
mlib_str_reserve()
mlib_str_resize()
mlib_str_shrink_to_fit()
```

Controls memory allocation and string capacity.

---

### Assignment

```c
mlib_str_set()
mlib_str_set_fmt()
mlib_str_copy()
mlib_str_move()
mlib_str_swap()
```

Assigns, copies, formats, or exchanges string contents.  
`mlib_str_copy` also copies the source object’s memory attributes.

---

### Concatenation

```c
mlib_str_cat()
mlib_str_cat_fmt()
```

Appends strings or formatted text.

---

### Element Access

```c
mlib_str_data()
mlib_str_front()
mlib_str_back()
mlib_str_at()
mlib_str_len()
mlib_str_empty()
```

Accesses string data and properties.

---

### Modification

```c
mlib_str_push()
mlib_str_pop()
mlib_str_insert()
mlib_str_erase()
mlib_str_replace()
```

Modifies string contents.

---

### Searching

```c
mlib_str_find()
mlib_str_rfind()

mlib_str_find_char()
mlib_str_rfind_char()
```

Searches for substrings or characters.

---

### Comparison

```c
mlib_str_cmp()
mlib_str_case_cmp()
```

Compares two strings.

---

### Prefix/Suffix

```c
mlib_str_startswith()
mlib_str_endswith()
```

Checks string prefixes and suffixes.

---

### Character Conversion

```c
mlib_str_toupper()
mlib_str_tolower()
```

Converts string characters to upper or lower case.

---

### Iterator Macros

The library provides convenient iteration macros.

Forward iteration:

```c
mlib_str_for_each()
```

Forward iteration by index:

```c
mlib_str_for_each_index()
```

Reverse iteration:

```c
mlib_str_for_each_reverse()
```

Reverse iteration by index:

```c
mlib_str_for_each_index_reverse()
```

---

## Notes

- All dynamic strings are null‑terminated.
- The library manages memory internally – call `mlib_str_destroy()` when a dynamic string is no longer needed.
- String literals (`mlib_str_literal_t`) are non‑owning views; they do not need to be destroyed and do not allocate memory.
- Include `"string.h"` to get both dynamic strings and the literal type.
- By default, the library uses the standard `malloc`, `realloc`, and `free`.  
  Use `mlib_str_attr_t` to provide custom allocators for specialised environments.
```