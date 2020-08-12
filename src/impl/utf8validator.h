/*  -*- c++ -*-
    impl/utf8validator.h

    This file is part of KSieve,
    the KDE internet mail/usenet news message filtering library.
    SPDX-FileCopyrightText: 2003 Marc Mutz <mutz@kde.org>

    SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef KSIEVE_IMPL_UTF8VALIDATOR_H
#define KSIEVE_IMPL_UTF8VALIDATOR_H

namespace KSieve {
extern bool isValidUtf8(const char *s, unsigned int len);
} // namespace KSieve

#endif
