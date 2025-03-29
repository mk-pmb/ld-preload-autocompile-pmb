#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include "../../lib/string_util.c"

int main() {
    u_char safety1 = 0;
    char befor[5] = "b3f0r";
    char hello[5] = "hello";
    char after[5] = "AFt3R";
    u_char safety2 = 0;

    printf("befor = '%s' (%lu)\n", befor, strlen(befor));
    printf("hello = '%s' (%lu)\n", hello, strlen(hello));
    printf("after = '%s' (%lu)\n", after, strlen(after));

    printf("ensure \\0:\n");
    STR_ENSURE_NULL_TERMINATED(hello);

    printf("befor = '%s' (%lu)\n", befor, strlen(befor));
    printf("hello = '%s' (%lu)\n", hello, strlen(hello));
    printf("after = '%s' (%lu)\n", after, strlen(after));

    return (safety1 && safety2);
}


// -*- coding: utf-8, tab-width: 2 -*-
