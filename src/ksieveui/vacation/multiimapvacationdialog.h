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
 * \brief The MultiImapVacationDialog class
 * \author Laurent Montel <montel@kde.org>
 */
class KSIEVEUI_EXPORT MultiImapVacationDialog : public QDialog
{
    Q_OBJECT
public:
    /*!
     */
    explicit MultiImapVacationDialog(KSieveCore::MultiImapVacationManager *manager, QWidget *parent = nullptr);
    /*!
     */
    ~MultiImapVacationDialog() override;

    /*!
     */
    [[nodiscard]] QList<KSieveCore::VacationCreateScriptJob *> listCreateJob() const;

    /*!
     */
    void switchToServerNamePage(const QString &serverName);
    /*!
     */
    void reject() override;
Q_SIGNALS:
    /*!
     */
    void okClicked();
    /*!
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
