/*  -*- c++ -*-
    impl/utf8validator.h

    This file is part of KSieve,
    the KDE internet mail/usenet news message filtering library.
    SPDX-FileCopyrightText: 2003 Marc Mutz <mutz@kde.org>

    SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

namespace KSieve
{
extern bool isValidUtf8(const char *s, unsigned int len);
} // namespace KSieve
