#define _GNU_SOURCE
#ifndef __LD_PRELOAD_AUTOCOMPILE_PMB__LIB__FIND_ORIG_FUNC_C

#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>


void find_orig_func(void* *ofptr, const char *ofname) {
  if (*ofptr) { return; }
  *ofptr = dlsym(RTLD_NEXT, ofname);
  if (*ofptr) { return; }
  fprintf(stderr, "Error: dlsym(RTLD_NEXT, %s): %s\n", ofname, dlerror());
  exit(60);
}


#define __LD_PRELOAD_AUTOCOMPILE_PMB__LIB__FIND_ORIG_FUNC_C 1
#endif
// -*- coding: utf-8, tab-width: 2 -*-
