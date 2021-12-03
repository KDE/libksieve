#! /bin/sh
# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
$EXTRACTRC `find . -name '*.kcfg'` >> rc.cpp || exit 11
$XGETTEXT `find . -name \*.cpp | grep -v '/tests/' | grep -v '/autotests/'` -o $podir/libksieve.pot
rm -f rc.cpp
