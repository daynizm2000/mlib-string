#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include "../include/mlib/string.h"


#define MLIB_STR_DEFCAP 64


#define mlib_str_util_val_swap(a, b) do { \
                __typeof__(a) __tmp = (a); \
                (a) = (b); \
                (b) = __tmp; \
        } while (0)


#define mlib_str_util_min(a, b) (((a) < (b)) ? (a) : (b))


static char *mlib_str_util_strrstr(const char *haystack, const char *needle)
{
        size_t hlen;
        size_t nlen;


        if (!haystack || !needle)
                return NULL;


        if (*needle == '\0') {
                while (*haystack)
                        haystack++;
                return (char *)haystack;
        }


        hlen = strlen(haystack);
        nlen = strlen(needle);


        if (nlen > hlen)
                return NULL;


        for (const char *h = haystack + (hlen - nlen); h >= haystack; h--) {
                if (*h == *needle && memcmp(h, needle, nlen) == 0)
                        return (char *)h;
        }


        return NULL;
}



static inline void *mlib_str_default_alloc(size_t size, void *arg)
{
        return malloc(size);
}


static inline void *mlib_str_default_realloc(void *addr, size_t newsize, void *arg)
{
        return realloc(addr, newsize);
}


static inline void mlib_str_default_free(void *addr, void *arg)
{
        free(addr);
}


int mlib_str_init(mlib_str_t *obj, const char *data, const mlib_str_attr_t *attr)
{
        if (!obj)
                return -1;


        if (!data) {
                obj->data = "";
                obj->len = 0;
                obj->capacity = 0;


                return 0;
        }


        if (!attr) {
                obj->attr.mem_ops.alloc = mlib_str_default_alloc;
                obj->attr.mem_ops.realloc = mlib_str_default_realloc;
                obj->attr.mem_ops.free = mlib_str_default_free;
                obj->attr.private_data = NULL;
        }
        else {
                obj->attr = *attr;


                if (!attr->mem_ops.alloc)
                        obj->attr.mem_ops.alloc = mlib_str_default_alloc;
                
                if (!attr->mem_ops.realloc)
                        obj->attr.mem_ops.realloc = mlib_str_default_realloc;

                if (!attr->mem_ops.free)
                        obj->attr.mem_ops.free = mlib_str_default_free;
        }


        obj->capacity = strlen(data) + 1;
        obj->len = obj->capacity - 1;


        obj->data = obj->attr.mem_ops.alloc(obj->capacity, obj->attr.private_data);

        if (!obj->data)
                return -1;


        strcpy(obj->data, data);


        return 0;
}


int mlib_str_reserve(mlib_str_t *obj, size_t newcap)
{
        char *tmp;


        if (!obj || !newcap)
                return -1;


        tmp = obj->attr.mem_ops.realloc(obj->data, newcap, obj->attr.private_data);

        if (!tmp)
                return -1;


        obj->data = tmp;
        obj->capacity = newcap;


        return 0;
}


int mlib_str_resize(mlib_str_t *obj, size_t newsize, int fill_val)
{
        va_list vl;


        if (!obj)
                return -1;


        if (newsize == SIZE_MAX)
                return -1;


        if (!newsize) {
                mlib_str_clear(obj);

                return 0;
        }


        if (newsize < obj->len) {
                obj->len = newsize;


                if (obj->data)
                        obj->data[obj->len] = '\0';


                return 0;
        }


        if (newsize > obj->capacity)
                if (mlib_str_reserve(obj, newsize + 1))
                        return -1;


        memset(obj->data + obj->len, fill_val, newsize - obj->len);
        obj->len = newsize;
        obj->data[obj->len] = '\0';


        return 0;
}


int mlib_str_shrink_to_fit(mlib_str_t *obj)
{
        if (!obj)
                return -1;


        if (!obj->len) {
                mlib_str_destroy(obj);
                
                return 0;
        }


        return mlib_str_reserve(obj, obj->len + 1);
}


void mlib_str_swap(mlib_str_t *obj1, mlib_str_t *obj2)
{
        if (!obj1 || !obj2)
                return;


        mlib_str_util_val_swap(obj1->data, obj2->data);
        mlib_str_util_val_swap(obj1->len, obj2->len);
        mlib_str_util_val_swap(obj1->capacity, obj2->capacity);
        mlib_str_util_val_swap(obj1->attr, obj2->attr);
}


int mlib_str_copy(mlib_str_t *dst, const mlib_str_t *src)
{
        if (!dst || !src)
                return -1;

        if (dst == src)
                return 0;


        mlib_str_destroy(dst);


        return mlib_str_init(dst, src->data, &src->attr);
}


int mlib_str_move(mlib_str_t *dst, mlib_str_t *src)
{
        if (!dst || !src)
                return -1;


        mlib_str_destroy(dst);


        *dst = *src;
        memset(src, 0, sizeof(mlib_str_t));


        return 0;
}


int mlib_str_set(mlib_str_t *obj, const char *data)
{
        size_t len;


        if (!obj || !data)
                return -1;


        len = strlen(data);


        if (len == SIZE_MAX)
                return -1;


        if (!obj->data || !obj->capacity) {
                obj->capacity = len + 1;
                obj->len = len;


                obj->data = obj->attr.mem_ops.alloc(obj->capacity, obj->attr.private_data);

                if (!obj->data)
                        return -1;


                strcpy(obj->data, data);


                return 0;
        }


        if (len + 1 > obj->capacity)
                if (mlib_str_reserve(obj, len + 1))
                        return -1;


        strcpy(obj->data, data);
        obj->len = len;


        return 0;
}


int mlib_str_set_fmt(mlib_str_t *obj, const char *fmt, ...)
{
        va_list vl;
        size_t len;


        if (!obj || !fmt)
                return -1;


        va_start(vl, fmt);


        len = vsnprintf(NULL, 0, fmt, vl);


        va_end(vl);


        if (!obj->data || !obj->capacity) {
                obj->capacity = len + 1;


                obj->data = obj->attr.mem_ops.alloc(obj->capacity, obj->attr.private_data);
                
                if (!obj->data)
                        return -1;
        }

        
        if (len + 1 > obj->capacity)
                if (mlib_str_reserve(obj, len + 1))
                        return -1;


        va_start(vl, fmt);


        vsnprintf(obj->data, obj->capacity, fmt, vl);
        obj->len = len;


        va_end(vl);


        return 0;
}


int mlib_str_cat(mlib_str_t *obj, const char *data)
{
        size_t len;


        if (!obj || !data)
                return -1;


        len = strlen(data);


        if (len > SIZE_MAX - obj->len - 1)
                return -1;


        if (!obj->data || !obj->capacity) {
                obj->capacity = len + 1;
                obj->len = len;


                obj->data = obj->attr.mem_ops.alloc(obj->capacity, obj->attr.private_data);

                if (!obj->data)
                        return -1;


                strcpy(obj->data, data);


                return 0;
        }


        if (obj->len + len + 1 > obj->capacity)
                if (mlib_str_reserve(obj, obj->len + len + 1))
                        return -1;


        memcpy(obj->data + obj->len, data, len + 1);
        obj->len += len;


        return 0;
}


int mlib_str_cat_fmt(mlib_str_t *obj, const char *fmt, ...)
{
        size_t len;
        va_list vl;


        if (!obj || !fmt)
                return -1;


        va_start(vl, fmt);


        len = vsnprintf(NULL, 0, fmt, vl);


        va_end(vl);


        if (len > SIZE_MAX - obj->len - 1)
                return -1;


        if (!obj->data || !obj->capacity) {
                obj->capacity = len + 1;
                obj->len = len;


                obj->data = obj->attr.mem_ops.alloc(obj->capacity, obj->attr.private_data);
                
                if (!obj->data)
                        return -1;

        
                va_start(vl, fmt);
                

                vsnprintf(obj->data, obj->capacity, fmt, vl);


                va_end(vl);


                return 0;
        }


        if (obj->len + len + 1 > obj->capacity)
                if (mlib_str_reserve(obj, obj->len + len + 1))
                        return -1;


        va_start(vl, fmt);


        vsnprintf(obj->data + obj->len, obj->capacity - obj->len, fmt, vl);
        obj->len += len;


        va_end(vl);


        return  0;
}


int mlib_str_cmp(const mlib_str_t *obj1, const mlib_str_t *obj2)
{
        size_t len;
        int ret;


        if (!obj1 || !obj2)
                return 0;


        if ((!obj1->data || !obj1->capacity) && (!obj2->data || !obj2->capacity))
                return 0;

        if ((!obj1->data || !obj1->capacity) && (obj2->data || obj2->capacity))
                return -1;

        if ((obj1->data || obj1->capacity) && (!obj2->data || !obj2->capacity))
                return 1;


        len = mlib_str_util_min(obj1->len, obj2->len);
        ret = strncmp(obj1->data, obj2->data, len);


        if (ret == 0) {
                if (obj1->len == obj2->len)
                        return 0;

                
                return (obj1->len > obj2->len) ? 1 : -1;
        }


        return ret;
}


int mlib_str_case_cmp(const mlib_str_t *obj1, const mlib_str_t *obj2)
{
        size_t len;
        int ret;


        if (!obj1 || !obj2)
                return 0;


        if ((!obj1->data || !obj1->capacity) && (!obj2->data || !obj2->capacity))
                return 0;

        if ((!obj1->data || !obj1->capacity) && (obj2->data || obj2->capacity))
                return -1;

        if ((obj1->data || obj1->capacity) && (!obj2->data || !obj2->capacity))
                return 1;


        len = mlib_str_util_min(obj1->len, obj2->len);
        ret = strncasecmp(obj1->data, obj2->data, len);


        if (ret == 0) {
                if (obj1->len == obj2->len)
                        return 0;

                
                return (obj1->len > obj2->len) ? 1 : -1;
        }


        return ret;
}


int mlib_str_push(mlib_str_t *obj, char val)
{
        if (!obj)
                return -1;


        if (!obj->data || !obj->capacity) {
                obj->data = obj->attr.mem_ops.alloc(MLIB_STR_DEFCAP, obj->attr.private_data);

                if (!obj->data)
                        return -1;


                obj->capacity = MLIB_STR_DEFCAP;
        }


        if (obj->len >= obj->capacity - 2)
                if (mlib_str_reserve(obj, obj->len + MLIB_STR_DEFCAP + 1))
                        return -1;


        obj->data[obj->len++] = val;
        obj->data[obj->len] = '\0';


        return 0;
}


char mlib_str_pop(mlib_str_t *obj)
{
        char ret;


        if (!obj)
                return 0;


        if (!obj->data || !obj->capacity || !obj->len)
                return 0;


        ret = obj->data[obj->len - 1];


        obj->data[obj->len - 1] = '\0';
        obj->len--;


        return ret;
}


int mlib_str_insert(mlib_str_t *obj, size_t idx, const char *data)
{
        size_t len;


        if (!obj || !data || idx > obj->len)
                return -1;


        len = strlen(data);


        if (!len)
                return 0;


        if (len > SIZE_MAX - obj->len - 1)
                return -1;


        if (obj->len + len + 1 > obj->capacity)
                if (mlib_str_reserve(obj, obj->len + len + 1))
                        return -1;
        

        memmove(&obj->data[idx + len],
                &obj->data[idx],
                obj->len - idx + 1);


        memmove(&obj->data[idx], data, len);
        obj->len += len;


        return 0;
}


int mlib_str_erase(mlib_str_t *obj, size_t start, size_t end)
{
        if (!obj || !obj->data || !obj->capacity || !obj->len)
                return -1;


        if (start > obj->len || end > obj->len || start > end)
                return -1;


        if (start == end)
                return 0;


        memmove(&obj->data[start], &obj->data[end], obj->len - end + 1);
        obj->len -= end - start;


        return 0;
}


ssize_t mlib_str_find(const mlib_str_t *obj, const char *data)
{
        const char *res;


        if (!obj || !obj->data || !data)
                return -1;


        res = strstr(obj->data, data);

        if (!res)
                return -1;


        return res - obj->data;
}


ssize_t mlib_str_rfind(const mlib_str_t *obj, const char *data)
{
        const char *res;


        if (!obj || !obj->data || !data)
                return -1;


        res = mlib_str_util_strrstr(obj->data, data);

        if (!res)
                return -1;


        return res - obj->data;
}


ssize_t mlib_str_find_char(const mlib_str_t *obj, char data)
{
        const char *res;


        if (!obj || !obj->data || !data)
                return -1;


        res = strchr(obj->data, data);

        if (!res)
                return -1;


        return res - obj->data;
}


ssize_t mlib_str_rfind_char(const mlib_str_t *obj, char data)
{
        const char *res;


        if (!obj || !obj->data || !data)
                return -1;


        res = strrchr(obj->data, data);

        if (!res)
                return -1;


        return res - obj->data;
}


int mlib_str_replace(mlib_str_t *obj, const char *old, const char *new)
{
        size_t olen;
        size_t nlen;
        char *data;


        if (!obj || !old || !new || !obj->data || !obj->capacity)
                return -1;


        if (!obj->len)
                return 0;


        data = obj->data;

        olen = strlen(old);
        nlen = strlen(new);


        if (!olen)
                return 0;


        if (nlen > olen) {
                int count = 0;
                char *d = obj->data;

                while (*d) {
                        char *start = strstr(d, old);

                        if (!start)
                                break;


                        d = start + olen;
                        count++;
                }


                if (!count)
                        return 0;


                if (obj->len + ((nlen - olen) * count) + 1 > obj->capacity)
                        if (mlib_str_reserve(obj, obj->capacity + ((nlen - olen) * count)))
                                return -1;
        }


        while (*data) {
                char *start = strstr(data, old);

                if (!start)
                        break;


                if (!nlen) {
                        memmove(start,
                                &start[olen],
                                obj->len - (&start[olen] - obj->data) + 1);


                        obj->len -= olen;
                }
                else if (nlen != olen) {
                        memmove(&start[nlen],
                                &start[olen],
                                obj->len - (&start[olen] - obj->data) + 1);


                        if (nlen > olen)
                                obj->len += nlen - olen;
                        else
                                obj->len -= olen - nlen;
                }


                memcpy(start, new, nlen);
                data = start + nlen;
        }


        return 0;
}


bool mlib_str_startswith(const mlib_str_t *obj, const char *prefix)
{
        size_t len;


        if (!obj || !obj->data || !prefix)
                return false;


        len = strlen(prefix);


        if (len > obj->len)
                return false;


        return memcmp(obj->data, prefix, len) == 0;
}


bool mlib_str_endswith(const mlib_str_t *obj, const char *suffix)
{
        size_t len;


        if (!obj || !obj->data || !suffix)
                return false;


        len = strlen(suffix);


        if (len > obj->len)
                return false;


        return memcmp((obj->data + obj->len) - len, suffix, len) == 0;
}


void mlib_str_destroy(mlib_str_t *obj)
{
        if (!obj)
                return;


        if (obj->data && obj->capacity) {
                obj->attr.mem_ops.free(obj->data, obj->attr.private_data);
                obj->data = NULL;
        }


        obj->len = 0;
        obj->capacity = 0;
        memset(&obj->attr, 0, sizeof(mlib_str_attr_t));
}