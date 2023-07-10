/*
    sievedebugdialog.cpp

    SPDX-FileCopyrightText: 2005 Martijn Klingens <klingens@kde.org>

    SPDX-License-Identifier: GPL-2.0-only
*/

#include "sievedebugdialog.h"
#include "util/findaccountinfojob.h"
#include "util/util.h"
#include <KPIMTextEdit/PlainTextEditor>
#include <KPIMTextEdit/PlainTextEditorWidget>

#include "libksieveui_debug.h"
#include <KLocalizedString>
#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/SyntaxHighlighter>
#include <KSyntaxHighlighting/Theme>
#include <kmanagesieve/sievejob.h>

#include <KConfigGroup>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QWindow>

using namespace KSieveUi;
namespace
{
static const char mySieveDebugDialogGroupName[] = "SieveDebugDialog";
}
SieveDebugDialog::SieveDebugDialog(KSieveCore::SieveImapPasswordProvider *passwordProvider, QWidget *parent)
    : QDialog(parent)
    , mPasswordProvider(passwordProvider)
{
    setWindowTitle(i18nc("@title:window", "Sieve Diagnostics"));
    auto mainLayout = new QVBoxLayout(this);

    // Collect all accounts
    mResourceIdentifier = KSieveCore::Util::sieveImapResourceNames();

    mEdit = new KPIMTextEdit::PlainTextEditorWidget(this);
    mEdit->setReadOnly(true);
    const KSyntaxHighlighting::Definition def = mRepo.definitionForName(QStringLiteral("Sieve"));
    if (!def.isValid()) {
        qCWarning(LIBKSIEVEUI_LOG) << "Invalid definition name";
    }

    auto hl = new KSyntaxHighlighting::SyntaxHighlighter(mEdit->editor()->document());
    hl->setTheme((palette().color(QPalette::Base).lightness() < 128) ? mRepo.defaultTheme(KSyntaxHighlighting::Repository::DarkTheme)
                                                                     : mRepo.defaultTheme(KSyntaxHighlighting::Repository::LightTheme));
    hl->setDefinition(def);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
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
    qCDebug(LIBKSIEVEUI_LOG);
    writeConfig();
}

void SieveDebugDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(640, 480));
    KConfigGroup group(KSharedConfig::openStateConfig(), mySieveDebugDialogGroupName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void SieveDebugDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), mySieveDebugDialogGroupName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
    group.sync();
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
    const QString ident = mResourceIdentifier.constFirst();

    mEdit->editor()->appendPlainText(i18n("Collecting data for account '%1'...\n", ident));
    mEdit->editor()->appendPlainText(i18n("------------------------------------------------------------\n"));

    auto job = new KSieveCore::FindAccountInfoJob(this);
    connect(job, &KSieveCore::FindAccountInfoJob::findAccountInfoFinished, this, &SieveDebugDialog::slotFindAccountInfoFinished);
    job->setIdentifier(ident);
    job->setProvider(mPasswordProvider);
    job->start();
}

void SieveDebugDialog::slotFindAccountInfoFinished(const KSieveCore::Util::AccountInfo &info)
{
    // Detect URL for this IMAP account
    const QUrl url = info.sieveUrl;
    if (!url.isValid()) {
        mEdit->editor()->appendPlainText(i18n("(Account does not support Sieve)\n\n"));
    } else {
        mUrl = url;

        mSieveJob = KManageSieve::SieveJob::list(mUrl);

        // Laurent: not necessary as it's a readonly dialog
        // mSieveJob->setProperty("sieveimapaccountsettings", QVariant::fromValue(info.sieveImapAccountSettings));
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

    const QString scriptFile = mScriptList.constFirst();
    mScriptList.pop_front();

    mEdit->editor()->appendPlainText(i18n("Contents of script '%1':\n", scriptFile));

    auto job = new KSieveCore::FindAccountInfoJob(this);
    connect(job, &KSieveCore::FindAccountInfoJob::findAccountInfoFinished, this, &SieveDebugDialog::slotFindAccountInfoForScriptFinished);
    job->setIdentifier(mResourceIdentifier.constFirst());
    job->setProvider(mPasswordProvider);
    job->setProperty("scriptfile", scriptFile);
    job->start();
}

void SieveDebugDialog::slotFindAccountInfoForScriptFinished(const KSieveCore::Util::AccountInfo &info)
{
    mUrl = info.sieveUrl;

    mUrl = mUrl.adjusted(QUrl::RemoveFilename);
    const QString scriptFile = sender()->property("scriptfile").toString();
    mUrl.setPath(mUrl.path() + QLatin1Char('/') + scriptFile);

    mSieveJob = KManageSieve::SieveJob::get(mUrl);

    connect(mSieveJob, &KManageSieve::SieveJob::gotScript, this, &SieveDebugDialog::slotGetScript);
}

void SieveDebugDialog::slotGetScript(KManageSieve::SieveJob *job, bool success, const QString &script, bool active)
{
    qCDebug(LIBKSIEVEUI_LOG) << "( ??," << success << ", ?," << active << ")" << Qt::endl << "script:" << Qt::endl << script;
    mSieveJob = nullptr; // job deletes itself after returning from this slot!

    if (!success) {
        mEdit->editor()->appendPlainText(
            i18n("Retrieving the script failed.\n"
                 "The server responded:\n%1",
                 job->errorString()));
    } else if (script.isEmpty()) {
        mEdit->editor()->appendPlainText(i18n("(This script is empty)\n\n"));
    } else {
        mEdit->editor()->appendPlainText(
            i18n("------------------------------------------------------------\n"
                 "%1\n"
                 "------------------------------------------------------------\n\n",
                 script));
    }

    // Fetch next script
    QTimer::singleShot(0, this, &SieveDebugDialog::slotDiagNextScript);
}

void SieveDebugDialog::slotGetScriptList(KManageSieve::SieveJob *job, bool success, const QStringList &scriptList, const QString &activeScript)
{
    if (mShutDownJob->isActive()) {
        mShutDownJob->stop();
    }
    qCDebug(LIBKSIEVEUI_LOG) << "Success:" << success << ", List:" << scriptList.join(QLatin1Char(',')) << ", active:" << activeScript;
    mSieveJob = nullptr; // job deletes itself after returning from this slot!

    mEdit->editor()->appendPlainText(i18n("Sieve capabilities:\n"));
    const QStringList caps = job->sieveCapabilities();
    if (caps.isEmpty()) {
        mEdit->editor()->appendPlainText(i18n("(No special capabilities available)"));
    } else {
        for (const auto &str : caps) {
            mEdit->editor()->appendPlainText(QLatin1String("* ") + str + QLatin1Char('\n'));
        }
        mEdit->editor()->appendPlainText(QStringLiteral("\n"));
    }

    mEdit->editor()->appendPlainText(i18n("Available Sieve scripts:\n"));

    if (scriptList.isEmpty()) {
        mEdit->editor()->appendPlainText(i18n("(No Sieve scripts available on this server)\n\n"));
    } else {
        mScriptList = scriptList;
        for (const auto &str : scriptList) {
            mEdit->editor()->appendPlainText(QLatin1String("* ") + str + QLatin1Char('\n'));
        }
        mEdit->editor()->appendPlainText(QStringLiteral("\n"));
        mEdit->editor()->appendPlainText(i18n("Active script: '%1'\n\n", activeScript));
    }

    // Handle next job: dump scripts for this server
    QTimer::singleShot(0, this, &SieveDebugDialog::slotDiagNextScript);
}

#include "moc_sievedebugdialog.cpp"
