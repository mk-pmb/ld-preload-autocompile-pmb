#!/bin/sh
# -*- coding: utf-8, tab-width: 2 -*-
LC_TIME=C ./autoldpre.sh --resolve-strip-suffix "$0" .sh date "$@"; exit $?
