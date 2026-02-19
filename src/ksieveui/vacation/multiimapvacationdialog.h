/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksieveui_export.h"
#include <KSieveCore/Util>
#include <QDialog>
#include <memory>
class QDialogButtonBox;
namespace KSieveCore
{
class VacationCreateScriptJob;
class MultiImapVacationManager;
}
namespace KSieveUi
{
class MultiImapVacationDialogPrivate;
/*!
 * \class KSieveUi::MultiImapVacationDialog
 * \brief The MultiImapVacationDialog class
 * \author Laurent Montel <montel@kde.org>
 * \inmodule KSieveUi
 * \inheaderfile KSieveUi/MultiImapVacationDialog
 */
class KSIEVEUI_EXPORT MultiImapVacationDialog : public QDialog
{
    Q_OBJECT
public:
    /*!
     * Constructs a MultiImapVacationDialog with the given manager and parent widget.
     * \param manager The MultiImapVacationManager instance.
     * \param parent The parent widget.
     */
    explicit MultiImapVacationDialog(KSieveCore::MultiImapVacationManager *manager, QWidget *parent = nullptr);
    /*!
     * Destroys the MultiImapVacationDialog.
     */
    ~MultiImapVacationDialog() override;

    /*!
     * Returns the list of vacation creation jobs.
     * \return A list of VacationCreateScriptJob pointers.
     */
    [[nodiscard]] QList<KSieveCore::VacationCreateScriptJob *> listCreateJob() const;

    /*!
     * Switches to the vacation page for the given server.
     * \param serverName The name of the server.
     */
    void switchToServerNamePage(const QString &serverName);
    /*!
     * Rejects the dialog.
     */
    void reject() override;
Q_SIGNALS:
    /*!
     * Emitted when the OK button is clicked.
     */
    void okClicked();
    /*!
     * Emitted when the Cancel button is clicked.
     */
    void cancelClicked();

private:
    KSIEVEUI_NO_EXPORT void slotSearchServerWithVacationSupportFinished(const QMap<QString, KSieveCore::Util::AccountInfo> &info);
    KSIEVEUI_NO_EXPORT void slotOkClicked();
    KSIEVEUI_NO_EXPORT void slotDefaultClicked();
    KSIEVEUI_NO_EXPORT void slotCanceled();
    KSIEVEUI_NO_EXPORT void createPage(const QString &serverName, const KSieveCore::Util::AccountInfo &info);
    KSIEVEUI_NO_EXPORT void init();
    KSIEVEUI_NO_EXPORT void readConfig();
    KSIEVEUI_NO_EXPORT void writeConfig();
    KSIEVEUI_NO_EXPORT void initialize();
    QDialogButtonBox *mButtonBox = nullptr;
    std::unique_ptr<MultiImapVacationDialogPrivate> const d;
};
}
