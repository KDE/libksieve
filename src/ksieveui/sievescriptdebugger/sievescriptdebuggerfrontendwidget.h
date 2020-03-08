/*
   Copyright (C) 2015-2020 Laurent Montel <montel@kde.org>

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

#ifndef SIEVESCRIPTDEBUGGERFRONTENDWIDGET_H
#define SIEVESCRIPTDEBUGGERFRONTENDWIDGET_H

#include <QWidget>
#include "ksieveui_private_export.h"

namespace KPIMTextEdit {
class PlainTextEditorWidget;
}
class KUrlRequester;
class QProcess;
class KLineEdit;
class QSplitter;
namespace KSieveUi {
class SieveTextEditWidget;
class SieveScriptDebuggerWarning;
class KSIEVEUI_TESTS_EXPORT SieveScriptDebuggerFrontEndWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SieveScriptDebuggerFrontEndWidget(QWidget *parent = nullptr);
    ~SieveScriptDebuggerFrontEndWidget();

    QString script() const;
    void setScript(const QString &script);
    bool canAccept() const;

    QList<int> splitterSizes() const;
    void setSplitterSizes(const QList<int> &sizes);

Q_SIGNALS:
    void scriptTextChanged();
    void debugScriptButtonClicked();
    void debugButtonEnabled(bool state);

private Q_SLOTS:
    void slotDebugScript();
    void slotEmailChanged(const QString &text);
    void slotReadStandardOutput();
    void slotReadErrorOutput();
    void slotDebugFinished();
    void slotScriptTextChanged();

private:
    void updateDebugButton();
    KSieveUi::SieveTextEditWidget *mSieveTextEditWidget = nullptr;
    KSieveUi::SieveScriptDebuggerWarning *mSieveScriptDebuggerWarning = nullptr;
    KPIMTextEdit::PlainTextEditorWidget *mSieveTestResult = nullptr;
    KUrlRequester *mEmailPath = nullptr;
    QProcess *mProcess = nullptr;
    KLineEdit *mExtension = nullptr;
    QSplitter *mSplitter = nullptr;
};
}
#endif // SIEVESCRIPTDEBUGGERFONTENDWIDGET_H
