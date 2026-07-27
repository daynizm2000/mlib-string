#pragma once


#include <stddef.h>



#define mlib_str_literal_set(obj, literal) do {                 \
                mlib_str_literal_t *__mlibstr_obj = (obj);      \
                                                                \
                __mlibstr_obj->data = literal "";               \
                __mlibstr_obj->len  = sizeof(literal "") - 1;   \
        } while (0)


#define mlib_str_literal_len(obj) ((obj)->len)
#define mlib_str_literal_data(obj) ((obj)->data)


#define mlib_str_literal_clear(obj) do {   \
                mlib_str_literal_t *__mlibstr_obj = (obj);      \
                                                                \
                __mlibstr_obj->data = NULL;                     \
                __mlibstr_obj->len  = 0;                        \
        } while (0)



typedef struct {
        const char *data;
        size_t len;
} mlib_str_literal_t;