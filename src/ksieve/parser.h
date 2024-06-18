/*  -*- c++ -*-
    ksieve/parser.h

    This file is part of KSieve,
    the KDE internet mail/usenet news message filtering library.
    SPDX-FileCopyrightText: 2002-2003 Marc Mutz <mutz@kde.org>

    SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include "ksieve_export.h"

namespace KSieve
{
class ScriptBuilder;
class Error;

/** @short Parser for the Sieve grammar.
    @author Marc Mutz <mutz@kde.org>
**/
class KSIEVE_EXPORT Parser
{
public:
    Parser(const char *scursor, const char *const send, int options = 0);
    ~Parser();

    void setScriptBuilder(ScriptBuilder *builder);
    ScriptBuilder *scriptBuilder() const;

    [[nodiscard]] bool parse();

    const Error &error() const;

    class Impl;

private:
    Impl *i = nullptr;

private:
    const Parser &operator=(const Parser &) = delete;
    Parser(const Parser &) = delete;
};
} // namespace KSieve
