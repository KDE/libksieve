/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksieveui_export.h"
#include <QDialog>
#include <ksievecore/util/util_p.h>
#include <memory>
class QDialogButtonBox;
namespace KSieveUi
{
class VacationCreateScriptJob;
class MultiImapVacationManager;
class MultiImapVacationDialogPrivate;
/**
 * @brief The MultiImapVacationDialog class
 * @author Laurent Montel <montel@kde.org>
 */
class KSIEVEUI_EXPORT MultiImapVacationDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MultiImapVacationDialog(MultiImapVacationManager *manager, QWidget *parent = nullptr);
    ~MultiImapVacationDialog() override;

    Q_REQUIRED_RESULT QList<VacationCreateScriptJob *> listCreateJob() const;

    void switchToServerNamePage(const QString &serverName);
    void reject() override;
Q_SIGNALS:
    void okClicked();
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
