/*
   Copyright (C) 2013-2017 Laurent Montel <montel@kde.org>

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

#include "multiimapvacationdialog.h"
#include "vacationpagewidget.h"
#include "multiimapvacationmanager.h"


#include <KLocalizedString>
#include <KSharedConfig>
#include <kwindowsystem.h>
#include <QTabWidget>
#include <KIconLoader>
#include <QUrl>

#include <QTabBar>
#include <QApplication>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <KConfigGroup>
#include <QDialogButtonBox>
#include <QPushButton>

using namespace KSieveUi;

class KSieveUi::MultiImapVacationDialogPrivate
{
public:
    MultiImapVacationDialogPrivate()
        : mTabWidget(nullptr)
        , mStackedWidget(nullptr)
        , mVacationManager(nullptr)
    {

    }

    QList<VacationCreateScriptJob *> mListCreateJob;
    QTabWidget *mTabWidget;
    QStackedWidget *mStackedWidget;
    MultiImapVacationManager *mVacationManager;
};

MultiImapVacationDialog::MultiImapVacationDialog(MultiImapVacationManager *manager, QWidget *parent)
    : QDialog(parent),
      d(new KSieveUi::MultiImapVacationDialogPrivate)
{
    d->mVacationManager = manager;

    setWindowTitle(i18n("Configure \"Out of Office\" Replies"));

    KWindowSystem::setIcons(winId(), qApp->windowIcon().pixmap(IconSize(KIconLoader::Desktop), IconSize(KIconLoader::Desktop)), qApp->windowIcon().pixmap(IconSize(KIconLoader::Small), IconSize(KIconLoader::Small)));

    init();
    readConfig();
}

MultiImapVacationDialog::~MultiImapVacationDialog()
{
    writeConfig();
    delete d;
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

QList<VacationCreateScriptJob *> MultiImapVacationDialog::listCreateJob() const
{
    return d->mListCreateJob;
}

void MultiImapVacationDialog::init()
{
    d->mStackedWidget = new QStackedWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(d->mStackedWidget);
    d->mTabWidget = new QTabWidget;
    d->mStackedWidget->addWidget(d->mTabWidget);

    QWidget *w = new QWidget;
    QVBoxLayout *vbox = new QVBoxLayout;
    w->setLayout(vbox);
    QLabel *lab = new QLabel(i18n("KMail's Out of Office Reply functionality relies on "
                                  "server-side filtering. You have not yet configured an "
                                  "IMAP server for this. "
                                  "You can do this on the \"Filtering\" tab of the IMAP "
                                  "account configuration."));
    lab->setWordWrap(true);
    vbox->addWidget(lab);
    vbox->addStretch();
    lab->setWordWrap(true);
    d->mStackedWidget->addWidget(w);
    d->mStackedWidget->setCurrentIndex(0);
    bool foundOneImap = false;
    QDialogButtonBox *buttonBox = nullptr;

    const QMap <QString, QUrl> list = d->mVacationManager->serverList();
    QMapIterator<QString, QUrl> i(list);
    while (i.hasNext()) {
        i.next();
        const QUrl url = i.value();
        createPage(i.key(), url);
        foundOneImap = true;
    }
    if (foundOneImap) {
        buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::RestoreDefaults, this);
        QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
        okButton->setDefault(true);
        okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
        connect(buttonBox, &QDialogButtonBox::accepted, this, &MultiImapVacationDialog::slotOkClicked);
        connect(buttonBox, &QDialogButtonBox::rejected, this, &MultiImapVacationDialog::slotCanceled);
        connect(buttonBox->button(QDialogButtonBox::RestoreDefaults), &QPushButton::clicked, this, &MultiImapVacationDialog::slotDefaultClicked);
    } else {
        d->mStackedWidget->setCurrentIndex(1);
        buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
        connect(buttonBox, &QDialogButtonBox::accepted, this, &MultiImapVacationDialog::slotOkClicked);
        connect(buttonBox, &QDialogButtonBox::rejected, this, &MultiImapVacationDialog::slotCanceled);
    }
    mainLayout->addWidget(buttonBox);
    if (d->mTabWidget->count() <= 1) {
        d->mTabWidget->tabBar()->hide();
    }
}

void MultiImapVacationDialog::slotCanceled()
{
    Q_EMIT cancelClicked();
}

void MultiImapVacationDialog::createPage(const QString &serverName, const QUrl &url)
{
    VacationPageWidget *page = new VacationPageWidget;
    page->setServerUrl(url);
    page->setServerName(serverName);
    page->setVacationManager(d->mVacationManager);
    d->mTabWidget->addTab(page, serverName + QStringLiteral(" (%1)").arg(url.userName()));
}

void MultiImapVacationDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "MultiImapVacationDialog");
    const QSize size = group.readEntry("Size", QSize());
    if (size.isValid()) {
        resize(size);
    } else {
        resize(sizeHint().width(), sizeHint().height());
    }
}

void MultiImapVacationDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "MultiImapVacationDialog");
    group.writeEntry("Size", size());
}

void MultiImapVacationDialog::slotOkClicked()
{
    for (int i = 0; i < d->mTabWidget->count(); ++i) {
        VacationPageWidget *vacationPage = qobject_cast<VacationPageWidget *>(d->mTabWidget->widget(i));
        if (vacationPage) {
            VacationCreateScriptJob *job = vacationPage->writeScript();
            if (job) {
                d->mListCreateJob.append(job);
            }
        }
    }
    Q_EMIT okClicked();
}

void MultiImapVacationDialog::slotDefaultClicked()
{
    for (int i = 0; i < d->mTabWidget->count(); ++i) {
        VacationPageWidget *vacationPage = qobject_cast<VacationPageWidget *>(d->mTabWidget->widget(i));
        if (vacationPage) {
            vacationPage->setDefault();
        }
    }
}

