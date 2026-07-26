#pragma once



#include <stddef.h>
#include <sys/types.h>
#include <stdbool.h>
#include <ctype.h>



#define mlib_str_for_each(obj, p) \
        for ((p) = (obj)->data; (obj)->data && (obj)->len && (p) < ((obj)->data + (obj)->len); (p)++)


#define mlib_str_for_each_index(obj, idx) \
        for ((idx) = 0; (obj)->data && (obj)->len && (idx) < (obj)->len; (idx)++)


#define mlib_str_for_each_reverse(obj, p) \
        for ((p) = (obj)->data + (obj)->len - 1; (obj)->len && (p) >= (obj)->data; (p)--)


#define mlib_str_for_each_index_reverse(obj, idx) \
        for ((idx) = (obj)->len; (obj)->data && (obj)->len && (idx) > 0; ) \
		if (--(idx), 1)



typedef struct {
        char *data;
        size_t len;
        size_t capacity;
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


        return (!obj->len) ? true : false;
}


static inline void mlib_str_toupper(const mlib_str_t *obj)
{
        if (!obj)
                return;


        for (char *p = obj->data; *p; p++)
                *p = toupper((unsigned char)*p);
}


static inline void mlib_str_tolower(mlib_str_t *obj)
{
        if (!obj)
                return;


        for (char *p = obj->data; *p; p++)
                *p = tolower((unsigned char)*p);
}




int mlib_str_init(mlib_str_t *obj, const char *data);


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
