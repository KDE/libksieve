/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksieveui_export.h"

#include <QDialog>
namespace KSieveCore
{
class SieveImapAccountSettings;
}
namespace KSieveUi
{
class SieveEditorGraphicalModeWidget;
/*!
 * \class KSieveUi::AutoCreateScriptDialog
 * \brief The AutoCreateScriptDialog class
 * \author Laurent Montel <montel@kde.org>
 * \inmodule KSieveUi
 * \inheaderfile KSieveUi/AutoCreateScriptDialog
 */
class KSIEVEUI_EXPORT AutoCreateScriptDialog : public QDialog
{
    Q_OBJECT
public:
    /*!
     * Constructs an AutoCreateScriptDialog with the given parent widget.
     * \param parent The parent widget.
     */
    explicit AutoCreateScriptDialog(QWidget *parent = nullptr);
    /*!
     * Destroys the AutoCreateScriptDialog.
     */
    ~AutoCreateScriptDialog() override;

    /*!
     * Returns the generated script and list of required extensions.
     * \param required The list of required extensions.
     * \return The generated Sieve script.
     */
    [[nodiscard]] QString script(QStringList &required) const;
    /*!
     * Sets the Sieve capabilities supported by the server.
     * \param capabilities A list of capability strings.
     */
    void setSieveCapabilities(const QStringList &capabilities);
    /*!
     * Sets the list of include files available for Sieve scripts.
     * \param listOfIncludeFile The list of include file names.
     */
    void setListOfIncludeFile(const QStringList &listOfIncludeFile);

    /*!
     * Loads a Sieve script for editing.
     * \param doc The Sieve script document.
     * \param error Error message if loading fails.
     */
    void loadScript(const QString &doc, QString &error);

    /*!
     * Sets the IMAP account settings for Sieve.
     * \param sieveImapAccountSettings The account settings.
     */
    void setSieveImapAccountSettings(const KSieveCore::SieveImapAccountSettings &sieveImapAccountSettings);

protected:
    /*!
     * Handles events for the dialog.
     * \param e The event to handle.
     * \return true if the event was handled.
     */
    bool event(QEvent *e) override;

private:
    KSIEVEUI_NO_EXPORT void readConfig();
    KSIEVEUI_NO_EXPORT void writeConfig();

private:
    SieveEditorGraphicalModeWidget *const mEditor;
};
}
