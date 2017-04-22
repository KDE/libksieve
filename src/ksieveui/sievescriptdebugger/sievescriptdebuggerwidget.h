/*
   Copyright (C) 2015-2017 Laurent Montel <montel@kde.org>

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

#ifndef SIEVESCRIPTDEBUGGERWIDGET_H
#define SIEVESCRIPTDEBUGGERWIDGET_H

#include <QWidget>
#include "ksieveui_export.h"
class QStackedWidget;
class QLabel;
namespace KSieveUi {
class SieveScriptDebuggerFrontEndWidget;
class KSIEVEUI_EXPORT SieveScriptDebuggerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SieveScriptDebuggerWidget(QWidget *parent = nullptr);
    ~SieveScriptDebuggerWidget();

    void setScript(const QString &script);
    QString script() const;

    bool canAccept() const;

    QList<int> splitterSizes() const;
    void setSplitterSizes(const QList<int> &sizes);

Q_SIGNALS:
    void scriptTextChanged();

private:
    void checkSieveTestApplication();
    QStackedWidget *mStackedWidget;
    SieveScriptDebuggerFrontEndWidget *mSieveScriptFrontEnd;
    QLabel *mSieveNoExistingFrontEnd;
};
}
#endif // SIEVESCRIPTDEBUGGERWIDGET_H
