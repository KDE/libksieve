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
    enum Type {
        None,
        KeyValuePair,
        Action,
        Quantity
    };

    Response() = default;

    [[nodiscard]] Type type() const;
    [[nodiscard]] QByteArray action() const;
    [[nodiscard]] uint quantity() const;
    [[nodiscard]] QByteArray key() const;
    [[nodiscard]] QByteArray value() const;
    [[nodiscard]] QByteArray extra() const;

    enum Result {
        Ok,
        No,
        Bye,
        Other,
    };

    [[nodiscard]] Result operationResult() const;
    [[nodiscard]] bool operationSuccessful() const;

    void clear();
    [[nodiscard]] bool parseResponse(const QByteArray &line);

private:
    Type m_type = None;
    uint m_quantity = 0;
    QByteArray m_key;
    QByteArray m_value;
    QByteArray m_extra;
};
}
