/*
    sievedebugdialog.cpp

    Copyright (c) 2005 Martijn Klingens <klingens@kde.org>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License,
    version 2.0, as published by the Free Software Foundation.
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software Foundation,
    Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, US
*/

#include "sievedebugdialog.h"
#include "kpimtextedit/plaintexteditorwidget.h"
#include "kpimtextedit/plaintexteditor.h"
#include "util/util_p.h"

#include "libksieve_debug.h"
#include <KLocalizedString>
#include <kmanagesieve/sievejob.h>
#include <KSyntaxHighlighting/SyntaxHighlighter>
#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/Theme>

#include <QTimer>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <KConfigGroup>
#include <QPushButton>
#include <QVBoxLayout>

using namespace KSieveUi;

SieveDebugDialog::SieveDebugDialog(QWidget *parent)
    : QDialog(parent)
    , mSieveJob(nullptr)
    , mShutDownJob(nullptr)
{
    setWindowTitle(i18n("Sieve Diagnostics"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Collect all accounts
    mResourceIdentifier = KSieveUi::Util::sieveImapResourceNames();

    mEdit = new KPIMTextEdit::PlainTextEditorWidget(this);
    mEdit->setReadOnly(true);
    KSyntaxHighlighting::Definition def;
    def = mRepo.definitionForName(QStringLiteral("Sieve"));

    KSyntaxHighlighting::SyntaxHighlighter *hl = new KSyntaxHighlighting::SyntaxHighlighter(mEdit->editor()->document());
    hl->setTheme((palette().color(QPalette::Base).lightness() < 128)
                 ? mRepo.defaultTheme(KSyntaxHighlighting::Repository::DarkTheme)
                 : mRepo.defaultTheme(KSyntaxHighlighting::Repository::LightTheme));
    hl->setDefinition(def);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SieveDebugDialog::reject);

    mainLayout->addWidget(mEdit);
    mainLayout->addWidget(buttonBox);

    if (!mResourceIdentifier.isEmpty()) {
        mEdit->editor()->setPlainText(i18n("Collecting diagnostic information about Sieve support...\n\n"));
        QTimer::singleShot(0, this, &SieveDebugDialog::slotDiagNextAccount);
    } else {
        mEdit->editor()->setPlainText(i18n("No IMAP resource found."));
    }
    readConfig();
}

SieveDebugDialog::~SieveDebugDialog()
{
    if (mSieveJob) {
        mSieveJob->kill();
        mSieveJob = nullptr;
    }
    qCDebug(LIBKSIEVE_LOG);
    writeConfig();
}

void SieveDebugDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "SieveDebugDialog");
    const QSize sizeDialog = group.readEntry("Size", QSize(640, 480));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void SieveDebugDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "SieveDebugDialog");
    group.writeEntry("Size", size());
}

void SieveDebugDialog::slotShutDownJob()
{
    disconnect(mSieveJob, &KManageSieve::SieveJob::gotList, this, &SieveDebugDialog::slotGetScriptList);
    mSieveJob->kill();
    mSieveJob = nullptr;
    mEdit->editor()->appendPlainText(i18n("Unable to get the info\n\n"));
    mResourceIdentifier.pop_front();
    QTimer::singleShot(0, this, &SieveDebugDialog::slotDiagNextAccount);
}

void SieveDebugDialog::slotDiagNextAccount()
{
    if (mResourceIdentifier.isEmpty()) {
        return;
    }
    if (!mShutDownJob) {
        mShutDownJob = new QTimer(this);
        mShutDownJob->setSingleShot(true);
        connect(mShutDownJob, &QTimer::timeout, this, &SieveDebugDialog::slotShutDownJob);
    }
    mShutDownJob->start(30 * 1000); // 30 seconds
    QString ident = mResourceIdentifier.first();

    mEdit->editor()->appendPlainText(i18n("Collecting data for account '%1'...\n", ident));
    mEdit->editor()->appendPlainText(i18n("------------------------------------------------------------\n"));

    // Detect URL for this IMAP account
    const KSieveUi::Util::AccountInfo info = KSieveUi::Util::fullAccountInfo(ident);
    const QUrl url = info.sieveUrl;
    if (!url.isValid()) {
        mEdit->editor()->appendPlainText(i18n("(Account does not support Sieve)\n\n"));
    } else {
        mUrl = url;

        mSieveJob = KManageSieve::SieveJob::list(mUrl);

        //Laurent: not necessary as it's a readonly dialog
        //mSieveJob->setProperty("sieveimapaccountsettings", QVariant::fromValue(info.sieveImapAccountSettings));
        connect(mSieveJob, &KManageSieve::SieveJob::gotList, this, &SieveDebugDialog::slotGetScriptList);

        // Bypass the singleShot timer -- it's fired when we get our data
        return;
    }

    // Handle next account async
    mResourceIdentifier.pop_front();
    QTimer::singleShot(0, this, &SieveDebugDialog::slotDiagNextAccount);
}

void SieveDebugDialog::slotDiagNextScript()
{
    if (mScriptList.isEmpty()) {
        // Continue handling accounts instead
        mScriptList.clear();
        mResourceIdentifier.pop_front();
        QTimer::singleShot(0, this, &SieveDebugDialog::slotDiagNextAccount);
        return;
    }

    QString scriptFile = mScriptList.first();
    mScriptList.pop_front();

    mEdit->editor()->appendPlainText(i18n("Contents of script '%1':\n", scriptFile));

    const KSieveUi::Util::AccountInfo info = KSieveUi::Util::fullAccountInfo(mResourceIdentifier.first());
    mUrl = info.sieveUrl;

    mUrl = mUrl.adjusted(QUrl::RemoveFilename);
    mUrl.setPath(mUrl.path() + scriptFile);

    mSieveJob = KManageSieve::SieveJob::get(mUrl);

    connect(mSieveJob, &KManageSieve::SieveJob::gotScript, this, &SieveDebugDialog::slotGetScript);
}

void SieveDebugDialog::slotGetScript(KManageSieve::SieveJob *job, bool success, const QString &script, bool active)
{
    qCDebug(LIBKSIEVE_LOG) << "( ??," << success
                           << ", ?," << active << ")" << endl
                           << "script:" << endl
                           << script;
    mSieveJob = nullptr; // job deletes itself after returning from this slot!

    if (!success) {
        mEdit->editor()->appendPlainText(i18n("Retrieving the script failed.\n"
                                              "The server responded:\n%1", job->errorString()));
    } else if (script.isEmpty()) {
        mEdit->editor()->appendPlainText(i18n("(This script is empty.)\n\n"));
    } else {
        mEdit->editor()->appendPlainText(i18n(
                                             "------------------------------------------------------------\n"
                                             "%1\n"
                                             "------------------------------------------------------------\n\n", script));
    }

    // Fetch next script
    QTimer::singleShot(0, this, &SieveDebugDialog::slotDiagNextScript);
}

void SieveDebugDialog::slotGetScriptList(KManageSieve::SieveJob *job, bool success, const QStringList &scriptList, const QString &activeScript)
{
    if (mShutDownJob->isActive()) {
        mShutDownJob->stop();
    }
    qCDebug(LIBKSIEVE_LOG) << "Success:" << success << ", List:" << scriptList.join(QLatin1Char(','))
                           <<", active:" << activeScript;
    mSieveJob = nullptr; // job deletes itself after returning from this slot!

    mEdit->editor()->appendPlainText(i18n("Sieve capabilities:\n"));
    const QStringList caps = job->sieveCapabilities();
    if (caps.isEmpty()) {
        mEdit->editor()->appendPlainText(i18n("(No special capabilities available)"));
    } else {
        QStringList::const_iterator end = caps.constEnd();
        for (QStringList::const_iterator it = caps.constBegin(); it != end; ++it) {
            mEdit->editor()->appendPlainText(QLatin1String("* ") + *it + QLatin1Char('\n'));
        }
        mEdit->editor()->appendPlainText(QStringLiteral("\n"));
    }

    mEdit->editor()->appendPlainText(i18n("Available Sieve scripts:\n"));

    if (scriptList.isEmpty()) {
        mEdit->editor()->appendPlainText(i18n("(No Sieve scripts available on this server)\n\n"));
    } else {
        mScriptList = scriptList;
        QStringList::const_iterator end = scriptList.constEnd();
        for (QStringList::const_iterator it = scriptList.constBegin(); it != end; ++it) {
            mEdit->editor()->appendPlainText(QLatin1String("* ") + *it + QLatin1Char('\n'));
        }
        mEdit->editor()->appendPlainText(QStringLiteral("\n"));
        mEdit->editor()->appendPlainText(i18n("Active script: %1\n\n", activeScript));
    }

    // Handle next job: dump scripts for this server
    QTimer::singleShot(0, this, &SieveDebugDialog::slotDiagNextScript);
}
