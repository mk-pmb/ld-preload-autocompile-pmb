#!/bin/bash
# -*- coding: utf-8, tab-width: 2 -*-


function ld_preload_autocompile () {
  local SELFPATH="$(readlink -m -- "$BASH_SOURCE"/..)"
  local PROG="$FUNCNAME"
  local SRC= SO= LDP=
  case "$1" in
    --resolve-strip-suffix )
      shift
      SRC="$(readlink -m -- "$1")"
      [ -f "$SRC" ] || return 4$(
        echo E: $PROG: "Failed to detect absolute path of '$1'." >&2)
      shift
      [ -z "$1" ] || SRC="${SRC%$1}"
      shift
      SRC="${SRC%.c}.c"
      set -- "$SRC" "$@"
      ;;
  esac
  while [ "$#" -ge 1 ]; do
    if [[ "$1" == *.c ]]; then
      SRC="$1"; shift
      SO="${SRC%.c}.so"
      maybe_recompile || return $?
      set -- "$SO" "$@"
    fi
    [[ "$1" == *.so ]] || break
    [ "${1:0:1}" == / ] || set -- "$PWD/$@"
    LDP+="$1:"
    shift
  done
  [ "$1" != -- ] || shift
  [ -n "$SRC" ] || return 4$(echo E: $PROG: "No source files given!" >&2)
  # echo D: $PROG: "exec: '$*'" >&2
  export LD_PRELOAD="$LDP$LD_PRELOAD"
  exec "$@"; return $?
}


function maybe_recompile () {
  if [ "$SO" -nt "$SRC" ]; then
    # The `-nt` criterion implies existence check.
    # Skipping the (re-)compile is only safe if the previous compile
    # (1) was done at all and (2) was done strictly after $SRC's last save.
    # (Save at the seemingly-same time cannot be relied on due to race
    # condition.)
    return 0
  fi

  local DIR="$(dirname -- "$SRC")"
  local BFN="$(basename -- "$SRC")"
  local TMP_SO="${BFN%.c}.tmp-$$.so"

  "$SELFPATH"/gcc-pedantic.sh --chdir "$DIR" -o "$TMP_SO" -shared "$BFN"
  local RV=$?

  TMP_SO="$DIR/$TMP_SO"
  if [ "$RV" == 0 ]; then
    mv --no-target-directory -- "$TMP_SO" "$SO" || return $?$(
      echo E: $PROG: "Failed to rename '$TMP_SO'" >&2)
    return 0
  fi

  echo E: $PROG: "Failed to compile $SRC" >&2
  rm -- "$TMP_SO" || true
  return 8
}


ld_preload_autocompile "$@"; exit $?
