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
     * Constructs an AbstractRegexpEditorLineEdit with the given parent widget.
     * \param parent The parent widget.
     */
    explicit AbstractRegexpEditorLineEdit(QWidget *parent = nullptr);
    /*!
     * Destroys the AbstractRegexpEditorLineEdit.
     */
    ~AbstractRegexpEditorLineEdit() override;

    /*!
     * Switches between regular expression editor and normal line edit modes.
     * \param regexpEditor true to switch to regexp editor, false for normal mode.
     */
    virtual void switchToRegexpEditorLineEdit(bool regexpEditor) = 0;
    /*!
     * Sets the code/pattern in the editor.
     * \param str The code or pattern to set.
     */
    virtual void setCode(const QString &str) = 0;
    /*!
     * Returns the code/pattern from the editor.
     * \return The current code or pattern.
     */
    virtual QString code() const = 0;

    /*!
     * Sets whether the clear button is enabled.
     * \param b true to enable the clear button.
     */
    virtual void setClearButtonEnabled(bool b) = 0;
    /*!
     * Sets the placeholder text in the editor.
     * \param str The placeholder text to display.
     */
    virtual void setPlaceholderText(const QString &str) = 0;

Q_SIGNALS:
    /*!
     * Emitted when the text has changed.
     * \param The new text content.
     */
    void textChanged(const QString &);
};
}
