/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "ksieveui_export.h"
#include <QWidget>
namespace KSieveUi
{
/*!
 * \class KSieveUi::AbstractSelectEmailLineEdit
 * \brief The AbstractSelectEmailLineEdit class
 * \author Laurent Montel <montel@kde.org>
 * \inmodule KSieveUi
 * \inheaderfile KSieveUi/AbstractSelectEmailLineEdit
 */
class KSIEVEUI_EXPORT AbstractSelectEmailLineEdit : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Constructs an AbstractSelectEmailLineEdit with the given parent widget.
     * \param parent The parent widget.
     */
    explicit AbstractSelectEmailLineEdit(QWidget *parent = nullptr);
    /*!
     * Destroys the AbstractSelectEmailLineEdit.
     */
    ~AbstractSelectEmailLineEdit() override;
    /*!
     * Sets the text in the email line edit.
     * \param str The text to set.
     */
    virtual void setText(const QString &str) = 0;
    /*!
     * Returns the text from the email line edit.
     * \return The current text.
     */
    virtual QString text() const = 0;

    /*!
     * Checks if multi-selection is enabled.
     * \return true if multi-selection is enabled.
     */
    [[nodiscard]] bool multiSelection() const;
    /*!
     * Sets multi-selection mode.
     * \param multiSelection true to enable multi-selection.
     */
    void setMultiSelection(bool multiSelection);
    /*!
     * Checks if the current value is valid.
     * \return true if the value is valid.
     */
    [[nodiscard]] virtual bool isValid() const = 0;

Q_SIGNALS:
    /*!
     * Emitted when the value has changed.
     */
    void valueChanged();

private:
    bool mMultiSelection = false;
};
}
