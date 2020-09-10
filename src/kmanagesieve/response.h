/*
    SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
    SPDX-FileContributor: Volker Krause <volker.krause@kdab.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KSIEVE_KMANAGESIEVE_RESPONSE_H
#define KSIEVE_KMANAGESIEVE_RESPONSE_H

#include <QByteArray>

namespace KManageSieve {
/** A response from a managesieve server.
 * @internal
 */
class Response
{
public:
    enum Type {
        None,
        KeyValuePair,
        Action,
        Quantity
    };

    Response()
    {
    }

    Type type() const;
    QByteArray action() const;
    uint quantity() const;
    QByteArray key() const;
    QByteArray value() const;
    QByteArray extra() const;

    enum Result {
        Ok,
        No,
        Bye,
        Other
    };

    Result operationResult() const;
    bool operationSuccessful() const;

    void clear();
    bool parseResponse(const QByteArray &line);

private:
    Type m_type = None;
    uint m_quantity = 0;
    QByteArray m_key;
    QByteArray m_value;
    QByteArray m_extra;
};
}

#endif
