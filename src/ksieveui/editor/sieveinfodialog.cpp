/*
  SPDX-FileCopyrightText: 2014-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#include "sieveinfodialog.h"
#include "sieveinfowidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>

namespace
{
static const char myConfigGroupName[] = "SieveInfoDialog";
}

using namespace KSieveUi;
SieveInfoDialog::SieveInfoDialog(QWidget *parent)
    : QDialog(parent)
    , mSieveInfoWidget(new SieveInfoWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Sieve Server Support"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mSieveInfoWidget->setObjectName(QStringLiteral("mSieveInfoWidget"));
    mainLayout->addWidget(mSieveInfoWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &SieveInfoDialog::reject);
    readConfig();
}

SieveInfoDialog::~SieveInfoDialog()
{
    writeConfig();
}

void SieveInfoDialog::setServerInfo(const QStringList &serverInfos)
{
    mSieveInfoWidget->setServerInfo(serverInfos);
}

void SieveInfoDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigGroupName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void SieveInfoDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigGroupName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
    group.sync();
}
