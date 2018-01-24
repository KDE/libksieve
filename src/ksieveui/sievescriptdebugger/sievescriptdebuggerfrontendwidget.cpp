/*
   Copyright (C) 2015-2018 Laurent Montel <montel@kde.org>

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

#include "sievescriptdebuggerfrontendwidget.h"
#include "sievescriptdebuggertextedit.h"
#include "sievescriptdebuggerwarning.h"
#include "sievescriptdebuggerresulteditor.h"

#include <QScrollBar>
#include <QVBoxLayout>
#include <QSplitter>
#include <QTemporaryFile>
#include <QProcess>
#include <QTextStream>
#include <QFormLayout>
#include "editor/sievetexteditwidget.h"
#include "editor/sievetextedit.h"
#include <kpimtextedit/plaintexteditorwidget.h>
#include <kpimtextedit/texttospeechwidget.h>
#include <QLabel>
#include <KUrlRequester>
#include <KLineEdit>
#include <KLocalizedString>
#include <QDate>

using namespace KSieveUi;

SieveScriptDebuggerFrontEndWidget::SieveScriptDebuggerFrontEndWidget(QWidget *parent)
    : QWidget(parent)
    , mProcess(nullptr)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);

    QFormLayout *formLayout = new QFormLayout;
    mainLayout->addLayout(formLayout);

    QLabel *emailLab = new QLabel(i18n("Email path:"), this);
    emailLab->setObjectName(QStringLiteral("emaillab"));

    mEmailPath = new KUrlRequester(this);
    mEmailPath->setObjectName(QStringLiteral("emailpath"));
    mEmailPath->lineEdit()->setTrapReturnKey(true);
    mEmailPath->lineEdit()->setClearButtonEnabled(true);
    connect(mEmailPath->lineEdit(), &KLineEdit::textChanged, this, &SieveScriptDebuggerFrontEndWidget::slotEmailChanged);

    formLayout->addRow(emailLab, mEmailPath);

    QHBoxLayout *extensionLayout = new QHBoxLayout;
    mainLayout->addLayout(extensionLayout);

    QLabel *extensionLab = new QLabel(i18n("Extension:"));
    extensionLab->setObjectName(QStringLiteral("extensionlab"));

    mExtension = new KLineEdit(this);
    mExtension->setObjectName(QStringLiteral("extension"));
    mExtension->setPlaceholderText(i18n("Activate extension with \"+<name of extension>\", deactivate it with \"-<name of extension>\""));
    mExtension->setClearButtonEnabled(true);
    mExtension->setTrapReturnKey(true);

    formLayout->addRow(extensionLab, mExtension);

    mSplitter = new QSplitter(Qt::Vertical);
    mSplitter->setObjectName(QStringLiteral("splitter"));
    mainLayout->addWidget(mSplitter);

    QWidget *sieveEditorWidget = new QWidget(this);
    QVBoxLayout *vboxSieveEditorLayout = new QVBoxLayout;
    sieveEditorWidget->setLayout(vboxSieveEditorLayout);
    vboxSieveEditorLayout->setMargin(0);

    KPIMTextEdit::TextToSpeechWidget *textToSpeechWidget = new KPIMTextEdit::TextToSpeechWidget(this);
    textToSpeechWidget->setObjectName(QStringLiteral("texttospeechwidget"));
    vboxSieveEditorLayout->addWidget(textToSpeechWidget);

    KSieveUi::SieveScriptDebuggerTextEdit *textEdit = new KSieveUi::SieveScriptDebuggerTextEdit(this);
    connect(textEdit, &KSieveUi::SieveScriptDebuggerTextEdit::textChanged, this, &SieveScriptDebuggerFrontEndWidget::slotScriptTextChanged);
    mSieveTextEditWidget = new KSieveUi::SieveTextEditWidget(textEdit, this);
    mSieveTextEditWidget->setObjectName(QStringLiteral("sievetexteditwidget"));
    vboxSieveEditorLayout->addWidget(mSieveTextEditWidget);
    connect(mSieveTextEditWidget->textEdit(), &SieveTextEdit::say, textToSpeechWidget, &KPIMTextEdit::TextToSpeechWidget::say);

    mSplitter->addWidget(sieveEditorWidget);
    mSplitter->setChildrenCollapsible(false);

    mSieveTestResult = new KPIMTextEdit::PlainTextEditorWidget(new KSieveUi::SieveScriptDebuggerResultEditor(this), this);
    mSieveTestResult->setObjectName(QStringLiteral("sievetextresult"));
    mSieveTestResult->editor()->setPlaceholderText(i18n("Display check script result..."));
    mSieveTestResult->setReadOnly(true);
    mSplitter->addWidget(mSieveTestResult);

    mSieveScriptDebuggerWarning = new KSieveUi::SieveScriptDebuggerWarning(this);
    mSieveScriptDebuggerWarning->setObjectName(QStringLiteral("sievescriptdebuggerwarning"));
    mainLayout->addWidget(mSieveScriptDebuggerWarning);

    connect(this, &SieveScriptDebuggerFrontEndWidget::debugScriptButtonClicked, this, &SieveScriptDebuggerFrontEndWidget::slotDebugScript);
}

SieveScriptDebuggerFrontEndWidget::~SieveScriptDebuggerFrontEndWidget()
{
    disconnect(mSieveTextEditWidget->textEdit(), &KSieveUi::SieveScriptDebuggerTextEdit::textChanged, this, &SieveScriptDebuggerFrontEndWidget::slotScriptTextChanged);
}

void SieveScriptDebuggerFrontEndWidget::updateDebugButton()
{
    Q_EMIT debugButtonEnabled(!mSieveTextEditWidget->textEdit()->document()->isEmpty() && !mEmailPath->lineEdit()->text().trimmed().isEmpty());
}

void SieveScriptDebuggerFrontEndWidget::slotScriptTextChanged()
{
    updateDebugButton();
    Q_EMIT scriptTextChanged();
}

void SieveScriptDebuggerFrontEndWidget::setSplitterSizes(const QList<int> &sizes)
{
    mSplitter->setSizes(sizes);
}

QList<int> SieveScriptDebuggerFrontEndWidget::splitterSizes() const
{
    return mSplitter->sizes();
}

void SieveScriptDebuggerFrontEndWidget::slotEmailChanged(const QString &text)
{
    Q_UNUSED(text);
    updateDebugButton();
}

void SieveScriptDebuggerFrontEndWidget::slotDebugScript()
{
    if (mSieveTextEditWidget->textEdit()->toPlainText().trimmed().isEmpty()) {
        mSieveScriptDebuggerWarning->setErrorMessage(i18n("Script text is empty."));
        return;
    }
    if (!mEmailPath->url().isLocalFile()) {
        mSieveScriptDebuggerWarning->setWarningMessage(i18n("Email file must be installed locally."));
        return;
    }

    QTemporaryFile *temporaryFile = new QTemporaryFile();
    if (!temporaryFile->open()) {
        mSieveScriptDebuggerWarning->setErrorMessage(i18n("Impossible to open temporary file."));
        delete temporaryFile;
        return;
    }
    Q_EMIT debugButtonEnabled(false);
    QTextStream stream(temporaryFile);
    stream << mSieveTextEditWidget->textEdit()->toPlainText();
    temporaryFile->flush();
    mProcess = new QProcess(this);
    temporaryFile->setParent(mProcess);

    QString extensionList;
    if (!mExtension->text().trimmed().isEmpty()) {
        extensionList = QStringLiteral("-x \"%1\"").arg(mExtension->text());
    }

    QStringList arguments;
    if (!extensionList.isEmpty()) {
        arguments << extensionList;
    }

    arguments << temporaryFile->fileName() << mEmailPath->url().toLocalFile();
    mProcess->start(QStringLiteral("sieve-test"), QStringList() << arguments);
    connect(mProcess, &QProcess::readyReadStandardOutput, this, &SieveScriptDebuggerFrontEndWidget::slotReadStandardOutput);
    connect(mProcess, &QProcess::readyReadStandardError, this, &SieveScriptDebuggerFrontEndWidget::slotReadErrorOutput);
    connect(mProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &SieveScriptDebuggerFrontEndWidget::slotDebugFinished);
    mSieveTestResult->editor()->appendPlainText(QStringLiteral("--------------------------------------"));
    mSieveTestResult->editor()->appendPlainText(QLocale().toString(QDateTime::currentDateTime()));
    mSieveTestResult->editor()->appendPlainText(QStringLiteral("\n"));
    if (!mProcess->waitForStarted()) {
        delete mProcess;
        mProcess = nullptr;
        Q_EMIT debugButtonEnabled(true);
    }
}

void SieveScriptDebuggerFrontEndWidget::slotReadStandardOutput()
{
    const QByteArray result = mProcess->readAllStandardOutput();
    mSieveTestResult->editor()->appendPlainText(QString::fromLocal8Bit(result));
}

void SieveScriptDebuggerFrontEndWidget::slotReadErrorOutput()
{
    const QByteArray result = mProcess->readAllStandardError();
    mSieveTestResult->editor()->appendPlainText(QString::fromLocal8Bit(result));
}

void SieveScriptDebuggerFrontEndWidget::slotDebugFinished()
{
    delete mProcess;
    mProcess = nullptr;
    Q_EMIT debugButtonEnabled(true);
}

QString SieveScriptDebuggerFrontEndWidget::script() const
{
    return mSieveTextEditWidget->textEdit()->toPlainText();
}

void SieveScriptDebuggerFrontEndWidget::setScript(const QString &script)
{
    mSieveTextEditWidget->textEdit()->setPlainText(script);
    mSieveTextEditWidget->textEdit()->verticalScrollBar()->setValue(mSieveTextEditWidget->textEdit()->verticalScrollBar()->maximum());
}

bool SieveScriptDebuggerFrontEndWidget::canAccept() const
{
    const QString script = mSieveTextEditWidget->textEdit()->toPlainText();
    if (script.contains(QLatin1String("debug_log")) || script.contains(QLatin1String("vnd.dovecot.debug"))) {
        mSieveScriptDebuggerWarning->setErrorMessage(i18n("Script still contains debug method. Remove it please."));
        return false;
    }
    mSieveScriptDebuggerWarning->hide();
    return true;
}
