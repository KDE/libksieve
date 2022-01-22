/*
   SPDX-FileCopyrightText: 2015-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sievescriptdebuggerfrontendwidget.h"
#include "sievescriptdebuggerresulteditor.h"
#include "sievescriptdebuggertextedit.h"
#include "sievescriptdebuggerwarning.h"

#include "editor/sievetextedit.h"
#include "editor/sievetexteditwidget.h"
#include <KLineEdit>
#include <KLocalizedString>
#include <KPIMTextEdit/PlainTextEditorWidget>
#ifndef DONT_HAVE_TEXT_TO_SPEACH_SUPPORT
#include <KPIMTextEdit/TextToSpeechWidget>
#endif
#include <KUrlRequester>
#include <Libkdepim/LineEditCatchReturnKey>
#include <QDate>
#include <QFormLayout>
#include <QLabel>
#include <QProcess>
#include <QScrollBar>
#include <QSplitter>
#include <QTemporaryFile>
#include <QTextStream>
#include <QVBoxLayout>

using namespace KSieveUi;

SieveScriptDebuggerFrontEndWidget::SieveScriptDebuggerFrontEndWidget(QWidget *parent)
    : QWidget(parent)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});

    auto formLayout = new QFormLayout;
    mainLayout->addLayout(formLayout);

    auto emailLab = new QLabel(i18n("Email path:"), this);
    emailLab->setObjectName(QStringLiteral("emaillab"));

    mEmailPath = new KUrlRequester(this);
    mEmailPath->setObjectName(QStringLiteral("emailpath"));
    mEmailPath->lineEdit()->setTrapReturnKey(true);
    mEmailPath->lineEdit()->setClearButtonEnabled(true);
    connect(mEmailPath->lineEdit(), &KLineEdit::textChanged, this, &SieveScriptDebuggerFrontEndWidget::slotEmailChanged);

    formLayout->addRow(emailLab, mEmailPath);

    auto extensionLayout = new QHBoxLayout;
    mainLayout->addLayout(extensionLayout);

    auto extensionLab = new QLabel(i18n("Extension:"));
    extensionLab->setObjectName(QStringLiteral("extensionlab"));

    mExtension = new QLineEdit(this);
    mExtension->setObjectName(QStringLiteral("extension"));
    mExtension->setPlaceholderText(i18n("Activate extension with \"+<name of extension>\", deactivate it with \"-<name of extension>\""));
    mExtension->setClearButtonEnabled(true);
    new KPIM::LineEditCatchReturnKey(mExtension, this);

    formLayout->addRow(extensionLab, mExtension);

    mSplitter = new QSplitter(Qt::Vertical);
    mSplitter->setObjectName(QStringLiteral("splitter"));
    mainLayout->addWidget(mSplitter);

    auto sieveEditorWidget = new QWidget(this);
    auto vboxSieveEditorLayout = new QVBoxLayout;
    sieveEditorWidget->setLayout(vboxSieveEditorLayout);
    vboxSieveEditorLayout->setContentsMargins({});
#ifndef DONT_HAVE_TEXT_TO_SPEACH_SUPPORT
    auto textToSpeechWidget = new KPIMTextEdit::TextToSpeechWidget(this);
    textToSpeechWidget->setObjectName(QStringLiteral("texttospeechwidget"));
    vboxSieveEditorLayout->addWidget(textToSpeechWidget);
#endif
    auto textEdit = new KSieveUi::SieveScriptDebuggerTextEdit(this);
    connect(textEdit, &KSieveUi::SieveScriptDebuggerTextEdit::textChanged, this, &SieveScriptDebuggerFrontEndWidget::slotScriptTextChanged);
    mSieveTextEditWidget = new KSieveUi::SieveTextEditWidget(textEdit, this);
    mSieveTextEditWidget->setObjectName(QStringLiteral("sievetexteditwidget"));
    vboxSieveEditorLayout->addWidget(mSieveTextEditWidget);
#ifndef DONT_HAVE_TEXT_TO_SPEACH_SUPPORT
    connect(mSieveTextEditWidget->textEdit(), &SieveTextEdit::say, textToSpeechWidget, &KPIMTextEdit::TextToSpeechWidget::say);
#endif
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
    disconnect(mSieveTextEditWidget->textEdit(),
               &KSieveUi::SieveScriptDebuggerTextEdit::textChanged,
               this,
               &SieveScriptDebuggerFrontEndWidget::slotScriptTextChanged);
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
    Q_UNUSED(text)
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

    auto temporaryFile = new QTemporaryFile();
    if (!temporaryFile->open()) {
        mSieveScriptDebuggerWarning->setErrorMessage(i18n("Failed to open temporary file."));
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
    connect(mProcess, &QProcess::finished, this, &SieveScriptDebuggerFrontEndWidget::slotDebugFinished);
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
