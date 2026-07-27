# MLib String

`mlib_string` is a lightweight dynamic string library written in C.

The library provides a simple string object with automatic memory management and a convenient API for common string operations such as concatenation, formatting, searching, replacing, insertion, and deletion.  
Additionally, it offers a **string literal** type – a non‑owning, zero‑allocation view of a compile‑time or constant string.

The project is written in ISO C and depends only on the standard C library.

---

## Features

- Dynamic string type
- Lightweight string literal type for zero‑allocation string views
- Automatic memory management
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

    mlib_str_init(&strobj, "test");
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

## API Overview

### Initialization

```c
mlib_str_init()
mlib_str_destroy()
mlib_str_clear()
```

Creates, destroys, and clears dynamic string objects.

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