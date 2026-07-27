#pragma once




/*
----FULL-API----

Data types:
- mlib_str_t          Main string type
- mlib_str_attr_t     String attributes (memory operations, private data)
- mlib_str_literal_t  Lightweight string literal type

Macros:
- mlib_str_for_each(obj, p)                    Forward iteration
- mlib_str_for_each_index(obj, idx)            Forward index iteration  
- mlib_str_for_each_reverse(obj, p)            Reverse iteration
- mlib_str_for_each_index_reverse(obj, idx)    Reverse index iteration
- mlib_str_prv_data(obj)                       Access private data

Literal operations:
- mlib_str_literal_set(obj, literal)     Set literal string
- mlib_str_literal_len(obj)              Get literal length
- mlib_str_literal_data(obj)             Get literal data pointer
- mlib_str_literal_clear(obj)            Clear literal string

Construction:
- mlib_str_init(obj, data, attr)         Initialize string
- mlib_str_destroy(obj)                  Destroy string

Capacity:
- mlib_str_reserve(obj, newcap)          Reserve capacity
- mlib_str_resize(obj, newsize, fill)    Resize string
- mlib_str_shrink_to_fit(obj)            Reduce capacity to fit

Element access:
- mlib_str_data(obj)                     Get data pointer
- mlib_str_len(obj)                      Get string length  
- mlib_str_at(obj, idx)                  Access character at index
- mlib_str_front(obj)                    Access first character
- mlib_str_back(obj)                     Access last character
- mlib_str_empty(obj)                    Check if empty

Modifiers:
- mlib_str_clear(obj)                    Clear string
- mlib_str_set(obj, data)                Set string content
- mlib_str_set_fmt(obj, fmt, ...)        Set with format
- mlib_str_cat(obj, data)                Concatenate string
- mlib_str_cat_fmt(obj, fmt, ...)        Concatenate with format
- mlib_str_push(obj, val)                Push character
- mlib_str_pop(obj)                      Pop character
- mlib_str_insert(obj, idx, data)        Insert string at position
- mlib_str_erase(obj, start, end)        Erase range
- mlib_str_replace(obj, old, new)        Replace substring
- mlib_str_swap(obj1, obj2)              Swap two strings
- mlib_str_copy(dst, src)                Copy string
- mlib_str_move(dst, src)                Move string

String operations:
- mlib_str_cmp(obj1, obj2)               Compare strings
- mlib_str_case_cmp(obj1, obj2)          Case-insensitive compare
- mlib_str_find(obj, data)               Find substring
- mlib_str_rfind(obj, data)              Find substring from end
- mlib_str_find_char(obj, data)          Find character
- mlib_str_rfind_char(obj, data)         Find character from end
- mlib_str_startswith(obj, prefix)       Check prefix
- mlib_str_endswith(obj, suffix)         Check suffix

Transformation:
- mlib_str_toupper(obj)                  Convert to uppercase
- mlib_str_tolower(obj)                  Convert to lowercase

----------------
*/






#include <stddef.h>
#include <sys/types.h>
#include <stdbool.h>
#include <ctype.h>
#include "literal.h"




#define mlib_str_for_each(obj, p) \
        for ((p) = (obj)->data; (p) < ((obj)->data + (obj)->len); (p)++)


#define mlib_str_for_each_index(obj, idx) \
        for ((idx) = 0; (idx) < (obj)->len; (idx)++)


#define mlib_str_for_each_reverse(obj, p) \
        for ((p) = (obj)->data + (obj)->len - 1; (p) >= (obj)->data; (p)--)


#define mlib_str_for_each_index_reverse(obj, idx)       \
        for ((idx) = (obj)->len; (idx) > 0; )           \
			if (--(idx), 1)



#define mlib_str_prv_data(obj) ((obj)->attr.private_data)



typedef struct {
        struct {
                void *(*alloc)(size_t size, void *arg);
                void *(*realloc)(void *addr, size_t newsize, void *arg);
                void (*free)(void *addr, void *arg);
        } mem_ops;

        void *private_data;
} mlib_str_attr_t;


typedef struct {
        char *data;
        size_t len;
        size_t capacity;

        mlib_str_attr_t attr;
} mlib_str_t;



static inline void mlib_str_clear(mlib_str_t *obj)
{
        if (!obj)
                return;


        obj->len = 0;


        if (obj->data)
                obj->data[obj->len] = '\0';
}


static inline char *mlib_str_data(mlib_str_t *obj)
{
        if (!obj)
                return NULL;


        return obj->data;
}


static inline size_t mlib_str_len(const mlib_str_t *obj)
{
        if (!obj)
                return 0;


        return obj->len;
}


static inline char *mlib_str_back(mlib_str_t *obj)
{
        if (!obj || !obj->data || !obj->len)
                return NULL;


        return &obj->data[obj->len - 1];
}


static inline char *mlib_str_front(mlib_str_t *obj)
{
        if (!obj || !obj->data || !obj->len)
                return NULL;


        return &obj->data[0];
}


static inline char *mlib_str_at(mlib_str_t *obj, size_t idx)
{
        if (!obj || !obj->data || !obj->len || idx >= obj->len)
                return NULL;


        return &obj->data[idx];        
}


static inline bool mlib_str_empty(mlib_str_t *obj)
{
        if (!obj || !obj->data)
                return true;


        return obj->len == 0;
}


static inline void mlib_str_toupper(mlib_str_t *obj)
{
        char *p;


        if (!obj)
                return;


        mlib_str_for_each(obj, p)
                *p = toupper((unsigned char)*p);
}


static inline void mlib_str_tolower(mlib_str_t *obj)
{
        char *p;


        if (!obj)
                return;


        mlib_str_for_each(obj, p)
                *p = tolower((unsigned char)*p);
}




int mlib_str_init(mlib_str_t *obj, const char *data, const mlib_str_attr_t *attr);


int mlib_str_reserve(mlib_str_t *obj, size_t newcap);
int mlib_str_resize(mlib_str_t *obj, size_t newsize, int fill_val);
int mlib_str_shrink_to_fit(mlib_str_t *obj);


void mlib_str_swap(mlib_str_t *obj1, mlib_str_t *obj2);


int mlib_str_copy(mlib_str_t *dst, const mlib_str_t *src);
int mlib_str_move(mlib_str_t *dst, mlib_str_t *src);


int mlib_str_set(mlib_str_t *obj, const char *data);
int mlib_str_set_fmt(mlib_str_t *obj, const char *fmt, ...);


int mlib_str_cat(mlib_str_t *obj, const char *data);
int mlib_str_cat_fmt(mlib_str_t *obj, const char *fmt, ...);


int mlib_str_cmp(const mlib_str_t *obj1, const mlib_str_t *obj2);
int mlib_str_case_cmp(const mlib_str_t *obj1, const mlib_str_t *obj2);


int mlib_str_push(mlib_str_t *obj, char val);
char mlib_str_pop(mlib_str_t *obj);


ssize_t mlib_str_find(const mlib_str_t *obj, const char *data);
ssize_t mlib_str_rfind(const mlib_str_t *obj, const char *data);
ssize_t mlib_str_find_char(const mlib_str_t *obj, char data);
ssize_t mlib_str_rfind_char(const mlib_str_t *obj, char data);


int mlib_str_insert(mlib_str_t *obj, size_t idx, const char *data);
int mlib_str_erase(mlib_str_t *obj, size_t start, size_t end);
int mlib_str_replace(mlib_str_t *obj, const char *old, const char *);


bool mlib_str_startswith(const mlib_str_t *obj, const char *prefix);
bool mlib_str_endswith(const mlib_str_t *obj, const char *suffix);


void mlib_str_destroy(mlib_str_t *obj);