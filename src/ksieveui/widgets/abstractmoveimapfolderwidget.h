/*
   SPDX-FileCopyrightText: 2016-2026 Laurent Montel <montel@kde.org>

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
/*!
 * \class KSieveUi::AbstractMoveImapFolderWidget
 * \brief The AbstractMoveImapFolderWidget class
 * \author Laurent Montel <montel@kde.org>
 * \inmodule KSieveUi
 * \inheaderfile KSieveUi/AbstractMoveImapFolderWidget
 */
class KSIEVEUI_EXPORT AbstractMoveImapFolderWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Constructs an AbstractMoveImapFolderWidget with the given parent widget.
     * \param parent The parent widget.
     */
    explicit AbstractMoveImapFolderWidget(QWidget *parent = nullptr);
    /*!
     * Destroys the AbstractMoveImapFolderWidget.
     */
    ~AbstractMoveImapFolderWidget() override;

    /*!
     * Sets the folder path text.
     * \param str The folder path to set.
     */
    virtual void setText(const QString &str) = 0;
    /*!
     * Returns the folder path text.
     * \return The current folder path.
     */
    virtual QString text() const = 0;
    /*!
     * Sets the IMAP account settings.
     * \param account The account settings.
     */
    virtual void setSieveImapAccountSettings(const KSieveCore::SieveImapAccountSettings &account) = 0;
Q_SIGNALS:
    /*!
     * Emitted when the folder text has changed.
     * \param The new folder text.
     */
    void textChanged(const QString &);
};
}
