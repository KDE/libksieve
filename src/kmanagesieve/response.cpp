/*
    SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
    SPDX-FileContributor: Volker Krause <volker.krause@kdab.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "response.h"
#include "kmanagersieve_debug.h"

void KManageSieve::Response::clear()
{
    m_type = None;
    m_key.clear();
    m_value.clear();
    m_extra.clear();
    m_quantity = 0;
}

QByteArray KManageSieve::Response::action() const
{
    return m_key;
}

QByteArray KManageSieve::Response::extra() const
{
    return m_extra;
}

QByteArray KManageSieve::Response::key() const
{
    return m_key;
}

QByteArray KManageSieve::Response::value() const
{
    return m_value;
}

uint KManageSieve::Response::quantity() const
{
    return m_quantity;
}

KManageSieve::Response::Type KManageSieve::Response::type() const
{
    return m_type;
}

static uint parseQuantity(const QByteArray &line, int start, bool *ok = nullptr)
{
    // expecting {quantity} at start
    int end = line.indexOf("+}", start + 1);
    // some older versions of Cyrus enclose the literal size just in { } instead of { +}
    if (end == -1) {
        end = line.indexOf('}', start + 1);
    }

    return line.mid(start + 1, end - start - 1).toUInt(ok);
}

bool KManageSieve::Response::parseResponse(const QByteArray &line)
{
    clear();

    switch (line.at(0)) {
    case '{':
    {
        m_type = Quantity;
        bool ok = false;
        m_quantity = parseQuantity(line, 0, &ok);
        return ok;
    }
    case '"':
        // expecting "key" "value" pairs
        m_type = KeyValuePair;
        break;
    default:
    {
        // expecting single string
        m_type = Action;
        m_key = line;

        // Sometimes NO is followed by a quantity (multiline error message). Ex:
        // S:  "NO {62}"
        // S:  "script errors:"
        // S:  line 17: syntax error, unexpected $undefined
        const int bracePos = line.indexOf('{');
        if (bracePos > 0) {
            m_quantity = parseQuantity(line, bracePos);
        }
        return true;
    }
    }

    int start = 0;
    int end = line.indexOf('"', start + 1);
    if (end == -1) {
        qCDebug(KMANAGERSIEVE_LOG) << "Invalid protocol in:" << line;
        m_key = line.right(line.length() - start);
        return true;
    }
    m_key = line.mid(start + 1, end - start - 1);

    start = line.indexOf('"', end + 1);
    if (start == -1) {
        if (line.length() > end) {
            // skip " and space
            m_extra = line.right(line.length() - end - 2);
        }
        return true;
    }

    end = line.indexOf('"', start + 1);
    if (end == -1) {
        qCDebug(KMANAGERSIEVE_LOG) << "Invalid protocol in:" << line;
        m_value = line.right(line.length() - start);
        return true;
    }

    m_value = line.mid(start + 1, end - start - 1);
    return true;
}

KManageSieve::Response::Result KManageSieve::Response::operationResult() const
{
    if (m_type == Action) {
        const QByteArray response = m_key.left(2);
        if (response == "OK") {
            return Ok;
        } else if (response == "NO") {
            return No;
        } else if (response == "BY" /*E*/) {
            return Bye;
        }
    }
    return Other;
}

bool KManageSieve::Response::operationSuccessful() const
{
    return operationResult() == Ok;
}
