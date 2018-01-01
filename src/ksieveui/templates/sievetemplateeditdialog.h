/*
   Copyright (C) 2013-2018 Laurent Montel <montel@kde.org>

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

#ifndef SIEVETEMPLATEEDITDIALOG_H
#define SIEVETEMPLATEEDITDIALOG_H

#include <QDialog>

class KLineEdit;
class QPushButton;

namespace KSieveUi {
class SieveTextEditWidget;
class SieveTemplateEditDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SieveTemplateEditDialog(QWidget *parent = nullptr, bool defaultTemplate = false);
    ~SieveTemplateEditDialog();

    void setTemplateName(const QString &name);
    QString templateName() const;

    void setScript(const QString &);
    QString script() const;
    void setSieveCapabilities(const QStringList &capabilities);

private:
    void slotTemplateChanged();
    void readConfig();
    void writeConfig();
    SieveTextEditWidget *mTextEditWidget = nullptr;
    KLineEdit *mTemplateNameEdit = nullptr;
    QPushButton *mOkButton = nullptr;
};
}

#endif // SIEVETEMPLATEEDITDIALOG_H
