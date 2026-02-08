/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "ksieveui_export.h"
#include <QWidget>

namespace KSieveUi
{
/*!
 * \class KSieveUi::AbstractRegexpEditorLineEdit
 * \brief The AbstractRegexpEditorLineEdit class
 * \author Laurent Montel <montel@kde.org>
 * \inmodule KSieveUi
 * \inheaderfile KSieveUi/AbstractRegexpEditorLineEdit
 */
class KSIEVEUI_EXPORT AbstractRegexpEditorLineEdit : public QWidget
{
    Q_OBJECT
public:
    /*!
     */
    explicit AbstractRegexpEditorLineEdit(QWidget *parent = nullptr);
    /*!
     */
    ~AbstractRegexpEditorLineEdit() override;

    /*!
     */
    virtual void switchToRegexpEditorLineEdit(bool regexpEditor) = 0;
    /*!
     */
    virtual void setCode(const QString &str) = 0;
    /*!
     */
    virtual QString code() const = 0;

    /*!
     */
    virtual void setClearButtonEnabled(bool b) = 0;
    /*!
     */
    virtual void setPlaceholderText(const QString &str) = 0;

Q_SIGNALS:
    /*!
     */
    void textChanged(const QString &);
};
}
