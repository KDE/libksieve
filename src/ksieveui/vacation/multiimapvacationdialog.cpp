/*
   SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "multiimapvacationdialog.h"
#include "multiimapvacationmanager.h"
#include "vacation/searchserverwithvacationsupportjob.h"
#include "vacationpagewidget.h"

#include <KLocalizedString>
#include <KMessageBox>
#include <KSharedConfig>
#include <QTabWidget>

#include <KConfigGroup>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QTabBar>
#include <QVBoxLayout>

using namespace KSieveUi;
namespace
{
static const char myMultiImapVacationDialogGroupName[] = "MultiImapVacationDialog";
}
class KSieveUi::MultiImapVacationDialogPrivate
{
public:
    MultiImapVacationDialogPrivate() = default;

    QVector<VacationCreateScriptJob *> mListCreateJob;
    QTabWidget *mTabWidget = nullptr;
    QStackedWidget *mStackedWidget = nullptr;
    MultiImapVacationManager *mVacationManager = nullptr;
};

MultiImapVacationDialog::MultiImapVacationDialog(MultiImapVacationManager *manager, QWidget *parent)
    : QDialog(parent)
    , d(new KSieveUi::MultiImapVacationDialogPrivate)
{
    d->mVacationManager = manager;

    setWindowTitle(i18nc("@title:window", "Configure \"Out of Office\" Replies"));
    init();
    readConfig();
}

MultiImapVacationDialog::~MultiImapVacationDialog()
{
    writeConfig();
}

void MultiImapVacationDialog::switchToServerNamePage(const QString &serverName)
{
    const int nbTab(d->mTabWidget->count());
    for (int i = 0; i < nbTab; ++i) {
        if (d->mTabWidget->tabText(i) == serverName) {
            d->mTabWidget->setCurrentIndex(i);
            break;
        }
    }
}

void MultiImapVacationDialog::reject()
{
    bool canCloseDialog = true;
    for (int i = 0; i < d->mTabWidget->count(); ++i) {
        auto vacationPage = qobject_cast<VacationPageWidget *>(d->mTabWidget->widget(i));
        if (vacationPage) {
            if (vacationPage->wasChanged()) {
                const int answer = KMessageBox::questionYesNo(this,
                                                              i18nc("@info", "Do you really want to cancel?"),
                                                              i18nc("@title:window", "Confirmation"),
                                                              KGuiItem(i18nc("@action:button", "Cancel Editing"), QStringLiteral("dialog-ok")),
                                                              KGuiItem(i18nc("@action:button", "Do Not Cancel"), QStringLiteral("dialog-cancel")));
                if (answer == KMessageBox::Yes) {
                    QDialog::reject(); // Discard current changes
                }
                canCloseDialog = false;
                break;
            }
        }
    }
    if (canCloseDialog) {
        QDialog::reject();
    }
}

QVector<VacationCreateScriptJob *> MultiImapVacationDialog::listCreateJob() const
{
    return d->mListCreateJob;
}

void MultiImapVacationDialog::init()
{
    d->mStackedWidget = new QStackedWidget;
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(d->mStackedWidget);
    d->mTabWidget = new QTabWidget;
    d->mStackedWidget->addWidget(d->mTabWidget);

    auto w = new QWidget;
    auto vbox = new QVBoxLayout;
    w->setLayout(vbox);
    auto lab =
        new QLabel(i18n("KMail's Out of Office Reply functionality relies on "
                        "server-side filtering. You have not yet configured an "
                        "IMAP server for this. "
                        "You can do this on the \"Filtering\" tab of the IMAP "
                        "account configuration."));
    lab->setWordWrap(true);
    lab->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QFont font = lab->font();
    font.setBold(true);
    font.setPointSize(font.pointSize() + 2);
    lab->setFont(font);
    vbox->addWidget(lab);
    lab->setWordWrap(true);
    d->mStackedWidget->addWidget(w);
    d->mStackedWidget->setCurrentIndex(0);
    mButtonBox = new QDialogButtonBox(this);
    mButtonBox->setObjectName(QStringLiteral("mButtonBox"));
    mainLayout->addWidget(mButtonBox);
    initialize();
}

void MultiImapVacationDialog::initialize()
{
    auto job = new SearchServerWithVacationSupportJob(this);
    job->setPasswordProvider(d->mVacationManager->passwordProvider());
    connect(job,
            &SearchServerWithVacationSupportJob::searchServerWithVacationSupportFinished,
            this,
            &MultiImapVacationDialog::slotSearchServerWithVacationSupportFinished);
    job->start();
}

void MultiImapVacationDialog::slotSearchServerWithVacationSupportFinished(const QMap<QString, KSieveUi::Util::AccountInfo> &list)
{
    bool foundOneImap = false;

    QMapIterator<QString, KSieveUi::Util::AccountInfo> i(list);
    while (i.hasNext()) {
        i.next();
        createPage(i.key(), i.value());
        foundOneImap = true;
    }
    if (foundOneImap) {
        mButtonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::RestoreDefaults);
        QPushButton *okButton = mButtonBox->button(QDialogButtonBox::Ok);
        okButton->setDefault(true);
        okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
        connect(mButtonBox, &QDialogButtonBox::accepted, this, &MultiImapVacationDialog::slotOkClicked);
        connect(mButtonBox, &QDialogButtonBox::rejected, this, &MultiImapVacationDialog::slotCanceled);
        connect(mButtonBox->button(QDialogButtonBox::RestoreDefaults), &QPushButton::clicked, this, &MultiImapVacationDialog::slotDefaultClicked);
    } else { // Empty
        d->mStackedWidget->setCurrentIndex(1);
        mButtonBox->setStandardButtons(QDialogButtonBox::Close);
        connect(mButtonBox, &QDialogButtonBox::accepted, this, &MultiImapVacationDialog::slotOkClicked);
        connect(mButtonBox, &QDialogButtonBox::rejected, this, &MultiImapVacationDialog::slotCanceled);
    }
    if (d->mTabWidget->count() <= 1) {
        d->mTabWidget->tabBar()->hide();
    }
}

void MultiImapVacationDialog::slotCanceled()
{
    Q_EMIT cancelClicked();
}

void MultiImapVacationDialog::createPage(const QString &serverName, const KSieveUi::Util::AccountInfo &info)
{
    auto page = new VacationPageWidget;
    page->setServerUrl(info.sieveUrl);
    page->setServerName(serverName);
    page->setVacationManager(d->mVacationManager);
    page->setSieveImapAccountSettings(info.sieveImapAccountSettings);
    d->mTabWidget->addTab(page, serverName + QStringLiteral(" (%1)").arg(info.sieveUrl.userName()));
}

void MultiImapVacationDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myMultiImapVacationDialogGroupName);
    const QSize size = group.readEntry("Size", QSize());
    if (size.isValid()) {
        resize(size);
    } else {
        resize(sizeHint().width(), sizeHint().height());
    }
}

void MultiImapVacationDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myMultiImapVacationDialogGroupName);
    group.writeEntry("Size", size());
}

void MultiImapVacationDialog::slotOkClicked()
{
    bool errorFound = false;
    qDeleteAll(d->mListCreateJob);
    d->mListCreateJob.clear();
    for (int i = 0; i < d->mTabWidget->count(); ++i) {
        auto vacationPage = qobject_cast<VacationPageWidget *>(d->mTabWidget->widget(i));
        if (vacationPage) {
            VacationCreateScriptJob *job = vacationPage->writeScript(errorFound);

            if (job && !errorFound) {
                d->mListCreateJob.append(job);
            }
            if (errorFound) {
                // Clean up job
                qDeleteAll(d->mListCreateJob);
                d->mListCreateJob.clear();
                break;
            }
        }
    }
    if (!errorFound) {
        Q_EMIT okClicked();
    }
}

void MultiImapVacationDialog::slotDefaultClicked()
{
    for (int i = 0; i < d->mTabWidget->count(); ++i) {
        auto vacationPage = qobject_cast<VacationPageWidget *>(d->mTabWidget->widget(i));
        if (vacationPage) {
            vacationPage->setDefault();
        }
    }
}
