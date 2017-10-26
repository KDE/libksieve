/*
   Copyright (C) 2013-2017 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef MULTIIMAPVACATIONDIALOG_H
#define MULTIIMAPVACATIONDIALOG_H

#include <QDialog>
#include <QUrl>
#include "ksieveui_export.h"
#include "util_p.h"

namespace KSieveUi {
class VacationCreateScriptJob;
class MultiImapVacationManager;
class MultiImapVacationDialogPrivate;
class KSIEVEUI_EXPORT MultiImapVacationDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MultiImapVacationDialog(MultiImapVacationManager *manager, QWidget *parent = nullptr);
    ~MultiImapVacationDialog();

    QList<VacationCreateScriptJob *> listCreateJob() const;

    void switchToServerNamePage(const QString &serverName);

Q_SIGNALS:
    void okClicked();
    void cancelClicked();

private:
    void slotOkClicked();
    void slotDefaultClicked();
    void slotCanceled();
    void createPage(const QString &serverName, const KSieveUi::Util::AccountInfo &info);
    void init();
    void readConfig();
    void writeConfig();
    MultiImapVacationDialogPrivate *const d;
};
}

#endif // MULTIIMAPVACATIONDIALOG_H
