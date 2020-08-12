/*
   SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef ABSTRACTMOVEIMAPFOLDERWIDGET_H
#define ABSTRACTMOVEIMAPFOLDERWIDGET_H

#include <QWidget>
#include "ksieveui_export.h"
namespace KSieveUi {
class SieveImapAccountSettings;
/**
 * @brief The AbstractMoveImapFolderWidget class
 * @author Laurent Montel <montel@kde.org>
 */
class KSIEVEUI_EXPORT AbstractMoveImapFolderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractMoveImapFolderWidget(QWidget *parent = nullptr);
    ~AbstractMoveImapFolderWidget();

    virtual void setText(const QString &str) = 0;
    virtual QString text() const = 0;
    virtual void setSieveImapAccountSettings(const KSieveUi::SieveImapAccountSettings &account) = 0;
Q_SIGNALS:
    void textChanged(const QString &);
};
}

#endif // ABSTRACTMOVEIMAPFOLDERWIDGET_H
