/*
   SPDX-FileCopyrightText: 2016-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "ksieveui_export.h"
#include <QWidget>
namespace KSieveCore
{
class SieveImapAccountSettings;
}
namespace KSieveUi
{
/**
 * @brief The AbstractMoveImapFolderWidget class
 * @author Laurent Montel <montel@kde.org>
 */
class KSIEVEUI_EXPORT AbstractMoveImapFolderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractMoveImapFolderWidget(QWidget *parent = nullptr);
    ~AbstractMoveImapFolderWidget() override;

    virtual void setText(const QString &str) = 0;
    virtual QString text() const = 0;
    virtual void setSieveImapAccountSettings(const KSieveCore::SieveImapAccountSettings &account) = 0;
Q_SIGNALS:
    void textChanged(const QString &);
};
}
