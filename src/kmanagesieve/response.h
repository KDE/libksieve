/*
    SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
    SPDX-FileContributor: Volker Krause <volker.krause@kdab.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QByteArray>

namespace KManageSieve
{
/** A response from a managesieve server.
 * @internal
 */
class Response
{
public:
    enum Type { None, KeyValuePair, Action, Quantity };

    Response() = default;

    Q_REQUIRED_RESULT Type type() const;
    Q_REQUIRED_RESULT QByteArray action() const;
    Q_REQUIRED_RESULT uint quantity() const;
    Q_REQUIRED_RESULT QByteArray key() const;
    Q_REQUIRED_RESULT QByteArray value() const;
    Q_REQUIRED_RESULT QByteArray extra() const;

    enum Result { Ok, No, Bye, Other };

    Q_REQUIRED_RESULT Result operationResult() const;
    Q_REQUIRED_RESULT bool operationSuccessful() const;

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
