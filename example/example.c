// A simple example demonstrating the basic usage of MLib String


#include "../string/include/string.h"
#include <stdio.h>


int main(void)
{
        mlib_str_t strobj;


        mlib_str_init(&strobj, "test");


        mlib_str_cat(&strobj, " test");
        mlib_str_replace(&strobj, "test", "test...");
        mlib_str_replace(&strobj, " ", ",");


        printf("%s\n", mlib_str_data(&strobj));


        mlib_str_destroy(&strobj);
}
