#define _GNU_SOURCE // Required for statx
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "lib/find_orig_func.c"
#include "lib/stacktrace.c"


size_t strftime(char *s, size_t max, const char *format, const struct tm *tm) {
  static size_t (*impl)(char *, size_t, const char *, const struct tm *) = NULL;
  find_orig_func((void**)&impl, "strftime");
  fprintf(stderr, "<< intercepted strftime(): format: '%s'>>\n", format);
  size_t len = impl(s, max, format, tm);
  fprintf(stderr, "<< intercepted strftime(): original result: '%s', ", s);
  if (strcmp(format, " %a") == 0) { strncpy(s, "ABCDE", strlen(s)); }
  if (strcmp(format, " %b") == 0) { strncpy(s, "FGHIJ", strlen(s)); }
  fprintf(stderr, "modified result: '%s' >>\n", s);
  if (strcmp(format, " %a") == 0) { stacktrace_print(16, 2); }
  return len;
}


// -*- coding: utf-8, tab-width: 2 -*-
