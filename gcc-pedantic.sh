#!/bin/bash
# -*- coding: utf-8, tab-width: 2 -*-


function gcc_pedantic () {
  local EARLY_OPT=(

    -fPIC

    -Wall             # Enable most warnings
    -Wextra           # Enable extra warnings
    -Werror           # Treat all warnings as errors.
    -pedantic         # Ensure strict adherence to C/C++ standards
    -pedantic-errors  # Treat warnings from -pedantic as errors.
    -Wunused          # Warn about unused code.
    -Wconversion      # Warn about implicit type conversions.
    -Wformat=2        # Validate printf/scanf format strings.
    -Wshadow          # Warn about shadowed variables.
    -Wfloat-equal     # Warns about ==/!= comparison on floats.

    -rdynamic

    )

  local LATE_OPT=(

    # Linker flags should appear _after_ all source files:
    -lbacktrace
    -ldl

    -g # include debug info (to enable stack traces)

    )

  local DBGLV="${DEBUGLEVEL:-0}"
  local ITEM='while read P; do [ -z "$P" ] || which "${P%=*}" 2>/dev/null \
    | grep -qPe "^/" || echo "${P%=*} (apt package: ${P#*=})"; done'
  ITEM="$(sh -c "$ITEM" <<<'
    gcc=build-essential
    ctags=exuberant-ctags
    ')"
  [ -z "$ITEM" ] || return 8$(echo E: $FUNCNAME: >&2 \
    "Cannot find some required tools: ${ITEM//$'\n'/, }.")

  local DEST=
  while [ "$#" -ge 1 ]; do
    case "$1" in
      -o ) shift; DEST="$1"; shift;;
      -f* | \
      -l* | \
      -r* | \
      -W* ) EARLY_OPT+=( "$1" ); shift;;
      * ) break;;
    esac
  done

  local FIRST_SRC=
  for ITEM in "$@"; do case "$ITEM" in
    -* ) continue;;
    * ) FIRST_SRC="$1"; break;;
  esac; done

  [ -n "$FIRST_SRC" ] || return 8$(
    echo E: $FUNCNAME: 'Failed to guess primary source file.' >&2)

  if [ -z "$DEST" ]; then
    DEST="$FIRST_SRC"
    DEST="${DEST%'.cpp'}"
    DEST="${DEST%'.c++'}"
    DEST="${DEST%'.c'}"
    DEST="${DEST%'.c'}"
    DEST+=.
  fi
  if [[ "$DEST" == *. ]]; then
    if ctags -x --c-kinds=fp -f - "$FIRST_SRC" | grep -qPe '^main '; then
      DEST+='elf'
    else
      EARLY_OPT+=( -shared )
      DEST+='so'
    fi
  fi

  set -- gcc -o "$DEST" "${EARLY_OPT[@]}" "$@" "${LATE_OPT[@]}"
  [ "$DBGLV" -lt 2 ] || echo D: $FUNCNAME: "$*" >&2
  "$@"; local GCC_RV=$?
  [[ "$DEST" != *.so ]] || chmod a-x -- "$DEST" || true
  return "$GCC_RV"
}


gcc_pedantic "$@"; exit $?
