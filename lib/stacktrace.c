#define _GNU_SOURCE
#ifndef __LD_PRELOAD_AUTOCOMPILE_PMB__LIB__PRINT_STACKTRACE_C

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>

#include "string_util.c"


void stacktrace_core(const u_char max_depth, const u_char skip,
  char* names_chain_buf, size_t names_chain_sz,
  FILE* print_to)
{
  if (names_chain_buf == NULL) { names_chain_sz = 0; }
  void* stack[max_depth];
  int depth = backtrace(stack, max_depth);
  char** symbols_in_trace = backtrace_symbols(stack, depth);
  char symb[256] = "";
  char* basename;
  char* funcname;
  char* addr;
  char* offset;
  char* funcname_or_qm;

  char* names_chain_pos = names_chain_buf;
  const char* names_chain_last = names_chain_buf + size_t_minus(names_chain_sz, 1);
  if (names_chain_sz > 0) { names_chain_buf[0] = '\0'; }

  int step;
  for (step = skip; step < depth; step++) {
    STRCPY_SAFE(symb, symbols_in_trace[step]);
    addr = str_split_inplace_at_last(symb, " [0x");
    str_chop_chr(addr, ']');
    offset = str_split_inplace_at_last(symb, "+0x");
    str_chop_chr(offset, ')');
    funcname = str_split_inplace_at_last(symb, "(");
    basename = str_split_inplace_at_last(symb, "/");
    if (basename == NULL) { basename = symb; }
    funcname_or_qm = str_ifnullorempty(funcname, "?");
    if (print_to != NULL) {
      fprintf(print_to, "\t^%d\t%s\t%s\t+0x%s\t@0x%s\n", step - skip,
        basename,
        funcname_or_qm,
        str_ifnull(offset, "?"),
        str_ifnull(addr, "?"));
    }
    if (names_chain_sz >= 1) {
      // :TODO: We cannot use STRCAT_SAFE on char pointers as dest!
      if (names_chain_buf[0] != '\0') {
        str_append_at(&names_chain_pos, names_chain_last, " < ");
      }
      // fprintf(print_to, "\tNCB: '%s' + '%s'\n", names_chain_buf, funcname_or_qm);
      str_append_at(&names_chain_pos, names_chain_last, funcname_or_qm);
      // fprintf(print_to, "\tNCB: '%s'\n", names_chain_buf);
    }
  }
  free(symbols_in_trace);
  if (print_to != NULL) {
    if (step == skip) {
      fprintf(print_to, "\t^#\ttoo few stack frames to report (%u <= %u)\n",
        depth, skip);
    } else {
      if (names_chain_sz > 0) {
        fprintf(print_to, "\t^=\t%s\n", names_chain_buf);
      }
    }
  }
}


void stacktrace_print(const u_char max_depth, const u_char skip) {
  char names_chain[512];
  stacktrace_core(max_depth, skip, names_chain, sizeof(names_chain), stderr);
}


void stacktrace_capture_names_chain(const u_char max_depth, const u_char skip,
  char* names_chain_buf, size_t names_chain_sz)
{
  stacktrace_core(max_depth, skip, names_chain_buf, names_chain_sz, NULL);
}


#define __LD_PRELOAD_AUTOCOMPILE_PMB__LIB__PRINT_STACKTRACE_C 1
#endif
// -*- coding: utf-8, tab-width: 2 -*-
