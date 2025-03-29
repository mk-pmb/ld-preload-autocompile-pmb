#define _GNU_SOURCE
#ifndef __LD_PRELOAD_AUTOCOMPILE_PMB__LIB__STRING_UTIL_C

#include <stdbool.h>
#include <strings.h>


// ==v== Utilities for writing to fixed-size char arrays ==v==
#define STR_ENSURE_NULL_TERMINATED(str) *((char*)str + sizeof(str) - 1) = 0;
#define STRCPY_SAFE(dest, src) \
  strncpy(dest, src, sizeof(dest) - 1); \
  STR_ENSURE_NULL_TERMINATED(dest);
#define STRCAT_SAFE(dest, src) \
  strncat(dest, src, size_t_minus(sizeof(dest) - 1, strlen(dest))); \
  STR_ENSURE_NULL_TERMINATED(dest);
// ==^== Utilities for writing to fixed-size char arrays ==^==


const char always_empty[1] = "";
inline const char* empty_str() { return always_empty; }


size_t size_t_minus(const size_t minuend, const size_t subtrahend) {
  return (minuend > subtrahend) ? (minuend - subtrahend) : 0;
}


bool str_chop_chr(char* str, const char expected_last_character) {
  if (str == NULL) { return false; }
  size_t len = strlen(str);
  if (len < 1) { return false; }
  char* lastchar = (char*)str + len - 1;
  bool same = (*lastchar == expected_last_character);
  if (same) { *lastchar = '\0'; }
  return same;
}


char* str_ifnull(const char* x, const char* d) {
  return (x == NULL ? (char*)d : (char*)x);
}


char* str_ifnullorempty(const char* x, const char* d) {
  if (x == NULL) { return (char*)d; }
  if (x[0] == '\0') { return (char*)d; }
  return (char*)x;
}


// char* pluspart = strrchr(input, '+');
char* str_split_inplace_at_last(char* start, const char* sep) {
  if (start == NULL) { return NULL; }
  const size_t seplen = strlen(sep);
  if (seplen < 1) { return NULL; }
  char* last_sep = NULL;
  char* remainder = NULL;
  char* cur = start;
  while ((cur = strstr(cur, sep)) != NULL) {
    last_sep = cur;
    cur += seplen;
    remainder = cur;
  }
  if (last_sep == NULL) { return NULL; }
  last_sep[0] = '\0';
  return remainder;
}


size_t str_append_at(char** dest_pos, const char* dest_last, char* src) { /*
  dest_pos:   Position in the destination buffer where we shall continue
              appending string parts.
  dest_last:  The address of the last char available for the string.
              This should be (dest_buf + dest_size - 1).
  src:        The string part to be appended.
  */
  if (!dest_pos) { return 0; }
  if (!(*dest_pos)) { return 0; }
  if (!dest_last) { return 0; }
  if (!src) { return 0; }
  const size_t src_len = strlen(src);
  if (!src_len) { return 0; }
  size_t copied = 0;
  while (((*dest_pos) < dest_last) && (copied < src_len)) {
    (**dest_pos) = src[copied];
    (*dest_pos)++;
    copied++;
  }
  (**dest_pos) = '\0'; /* This will hit at most dest_last because we only
    increase if it was lower, and dest_last is still within the string. */
  return copied;
}


size_t str_append_autoseek(char* dest_buf, size_t dest_size, char* src) {
  if (!dest_buf) { return 0; }
  if (!dest_size) { return 0; }
  if (!src) { return 0; }
  const size_t dest_orig_len = strnlen(dest_buf, dest_size);
  const size_t dest_max_len = size_t_minus(dest_size, 1);
  char* dest_last = dest_buf + dest_max_len;
  char* dest_end = dest_buf + dest_orig_len;
  return str_append_at(&dest_end, dest_last, src);
}







#define __LD_PRELOAD_AUTOCOMPILE_PMB__LIB__STRING_UTIL_C 1
#endif
// -*- coding: utf-8, tab-width: 2 -*-
