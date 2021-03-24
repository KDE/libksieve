/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "ksieveui_private_export.h"
#include <QWidget>

namespace KPIMTextEdit
{
class PlainTextEditorWidget;
}
class KUrlRequester;
class QProcess;
class QLineEdit;
class QSplitter;
namespace KSieveUi
{
class SieveTextEditWidget;
class SieveScriptDebuggerWarning;
class KSIEVEUI_TESTS_EXPORT SieveScriptDebuggerFrontEndWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SieveScriptDebuggerFrontEndWidget(QWidget *parent = nullptr);
    ~SieveScriptDebuggerFrontEndWidget() override;

    Q_REQUIRED_RESULT QString script() const;
    void setScript(const QString &script);
    Q_REQUIRED_RESULT bool canAccept() const;

    Q_REQUIRED_RESULT QList<int> splitterSizes() const;
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
    QLineEdit *mExtension = nullptr;
    QSplitter *mSplitter = nullptr;
};
}
