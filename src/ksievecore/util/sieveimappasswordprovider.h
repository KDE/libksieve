/*
   SPDX-FileCopyrightText: 2017 Albert Astals Cid <aacid@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksievecore_export.h"
#include <QObject>
class QString;

namespace KSieveCore
{
/**
 * @brief The SieveImapPasswordProvider class
 * @author Laurent Montel <montel@kde.org>
 */
class KSIEVECORE_EXPORT SieveImapPasswordProvider : public QObject
{
    Q_OBJECT
public:
    explicit SieveImapPasswordProvider(QObject *parent = nullptr);

    ~SieveImapPasswordProvider() override = default;
    virtual void passwords(const QString &identifier) = 0;
Q_SIGNALS:
    void passwordsRequested(const QString &sievePassword, const QString &sieveCustomPassword);
};
}
