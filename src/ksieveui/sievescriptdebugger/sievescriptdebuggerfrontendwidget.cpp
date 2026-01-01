/*
   SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sievescriptdebuggerfrontendwidget.h"
#include "config-libksieve.h"

#include "sievescriptdebuggerresulteditor.h"
#include "sievescriptdebuggertextedit.h"
#include "sievescriptdebuggerwarning.h"

#include "editor/sievetextedit.h"
#include "editor/sievetexteditwidget.h"
#include <KLineEdit>
#include <KLocalizedString>
#include <TextCustomEditor/PlainTextEditorWidget>
#if HAVE_KTEXTADDONS_TEXT_TO_SPEECH_SUPPORT
#include <TextEditTextToSpeech/TextToSpeechContainerWidget>
#endif
#include <KLineEditEventHandler>
#include <KUrlRequester>
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

using namespace Qt::Literals::StringLiterals;
SieveScriptDebuggerFrontEndWidget::SieveScriptDebuggerFrontEndWidget(QWidget *parent)
    : QWidget(parent)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});

    auto formLayout = new QFormLayout;
    mainLayout->addLayout(formLayout);

    auto emailLab = new QLabel(i18nc("@label:textbox", "Email path:"), this);
    emailLab->setObjectName(QLatin1StringView("emaillab"));

    mEmailPath = new KUrlRequester(this);
    mEmailPath->setObjectName(QLatin1StringView("emailpath"));
    mEmailPath->lineEdit()->setTrapReturnKey(true);
    mEmailPath->lineEdit()->setClearButtonEnabled(true);
    connect(mEmailPath->lineEdit(), &KLineEdit::textChanged, this, &SieveScriptDebuggerFrontEndWidget::slotEmailChanged);

    formLayout->addRow(emailLab, mEmailPath);

    auto extensionLayout = new QHBoxLayout;
    mainLayout->addLayout(extensionLayout);

    auto extensionLab = new QLabel(i18nc("@label:textbox", "Extension:"));
    extensionLab->setObjectName(QLatin1StringView("extensionlab"));

    mExtension = new QLineEdit(this);
    mExtension->setObjectName(QLatin1StringView("extension"));
    mExtension->setPlaceholderText(i18nc("@info:placeholder", "Activate extension with \"+<name of extension>\", deactivate it with \"-<name of extension>\""));
    mExtension->setClearButtonEnabled(true);
    KLineEditEventHandler::catchReturnKey(mExtension);

    formLayout->addRow(extensionLab, mExtension);

    mSplitter = new QSplitter(Qt::Vertical);
    mSplitter->setObjectName(QLatin1StringView("splitter"));
    mainLayout->addWidget(mSplitter);

    auto sieveEditorWidget = new QWidget(this);
    auto vboxSieveEditorLayout = new QVBoxLayout;
    sieveEditorWidget->setLayout(vboxSieveEditorLayout);
    vboxSieveEditorLayout->setContentsMargins({});
#if HAVE_KTEXTADDONS_TEXT_TO_SPEECH_SUPPORT
    auto textToSpeechWidget = new TextEditTextToSpeech::TextToSpeechContainerWidget(this);
    textToSpeechWidget->setObjectName(QLatin1StringView("texttospeechwidget"));
    vboxSieveEditorLayout->addWidget(textToSpeechWidget);
#endif
    auto textEdit = new KSieveUi::SieveScriptDebuggerTextEdit(this);
    connect(textEdit, &KSieveUi::SieveScriptDebuggerTextEdit::textChanged, this, &SieveScriptDebuggerFrontEndWidget::slotScriptTextChanged);
    mSieveTextEditWidget = new KSieveUi::SieveTextEditWidget(textEdit, this);
    mSieveTextEditWidget->setObjectName(QLatin1StringView("sievetexteditwidget"));
    vboxSieveEditorLayout->addWidget(mSieveTextEditWidget);
#if HAVE_KTEXTADDONS_TEXT_TO_SPEECH_SUPPORT
    connect(mSieveTextEditWidget->textEdit(), &SieveTextEdit::say, textToSpeechWidget, &TextEditTextToSpeech::TextToSpeechContainerWidget::enqueue);
#endif
    mSplitter->addWidget(sieveEditorWidget);
    mSplitter->setChildrenCollapsible(false);

    mSieveTestResult = new TextCustomEditor::PlainTextEditorWidget(new KSieveUi::SieveScriptDebuggerResultEditor(this), this);
    mSieveTestResult->setObjectName(QLatin1StringView("sievetextresult"));
    mSieveTestResult->editor()->setPlaceholderText(i18nc("@info:placeholder", "Display check script result…"));
    mSieveTestResult->setReadOnly(true);
    mSplitter->addWidget(mSieveTestResult);

    mSieveScriptDebuggerWarning = new KSieveUi::SieveScriptDebuggerWarning(this);
    mSieveScriptDebuggerWarning->setObjectName(QLatin1StringView("sievescriptdebuggerwarning"));
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
        extensionList = u"-x \"%1\""_s.arg(mExtension->text());
    }

    QStringList arguments;
    if (!extensionList.isEmpty()) {
        arguments << extensionList;
    }

    arguments << temporaryFile->fileName() << mEmailPath->url().toLocalFile();
    mProcess->start(u"sieve-test"_s, QStringList() << arguments);
    connect(mProcess, &QProcess::readyReadStandardOutput, this, &SieveScriptDebuggerFrontEndWidget::slotReadStandardOutput);
    connect(mProcess, &QProcess::readyReadStandardError, this, &SieveScriptDebuggerFrontEndWidget::slotReadErrorOutput);
    connect(mProcess, &QProcess::finished, this, &SieveScriptDebuggerFrontEndWidget::slotDebugFinished);
    mSieveTestResult->editor()->appendPlainText(u"--------------------------------------"_s);
    mSieveTestResult->editor()->appendPlainText(QLocale().toString(QDateTime::currentDateTime()));
    mSieveTestResult->editor()->appendPlainText(u"\n"_s);
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
    if (script.contains(QLatin1StringView("debug_log")) || script.contains(QLatin1StringView("vnd.dovecot.debug"))) {
        mSieveScriptDebuggerWarning->setErrorMessage(i18n("Script still contains debug method. Remove it please."));
        return false;
    }
    mSieveScriptDebuggerWarning->hide();
    return true;
}

#include "moc_sievescriptdebuggerfrontendwidget.cpp"
