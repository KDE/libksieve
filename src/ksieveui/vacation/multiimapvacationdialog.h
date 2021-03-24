/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksieveui_export.h"
#include "util_p.h"
#include <QDialog>
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

    Q_REQUIRED_RESULT QVector<VacationCreateScriptJob *> listCreateJob() const;

    void switchToServerNamePage(const QString &serverName);
    void reject() override;
Q_SIGNALS:
    void okClicked();
    void cancelClicked();

private:
    void slotSearchServerWithVacationSupportFinished(const QMap<QString, KSieveUi::Util::AccountInfo> &info);
    void slotOkClicked();
    void slotDefaultClicked();
    void slotCanceled();
    void createPage(const QString &serverName, const KSieveUi::Util::AccountInfo &info);
    void init();
    void readConfig();
    void writeConfig();
    void initialize();
    QDialogButtonBox *mButtonBox = nullptr;
    std::unique_ptr<MultiImapVacationDialogPrivate> const d;
};
}

