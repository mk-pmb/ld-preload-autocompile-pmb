#define _GNU_SOURCE // Required for statx
#include <dlfcn.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void find_orig_func(void* *ofptr, const char *ofname) {
  if (*ofptr) { return; }
  *ofptr = dlsym(RTLD_NEXT, ofname);
  if (*ofptr) { return; }
  fprintf(stderr, "Error: dlsym(RTLD_NEXT, %s): %s\n", ofname, dlerror());
  exit(60);
}


size_t strftime(char *s, size_t max, const char *format, const struct tm *tm) {
  static size_t (*impl)(char *, size_t, const char *, const struct tm *) = NULL;
  find_orig_func((void**)&impl, "strftime");
  printf("<< intercepted strftime(): format: '%s'>>\n", format);
  size_t len = impl(s, max, format, tm);
  printf("<< intercepted strftime(): result: '%s'>>\n", s);
  return len;
}


// -*- coding: utf-8, tab-width: 2 -*-
