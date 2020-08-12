/*
  SPDX-FileCopyrightText: 2014-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#include "sieveinfodialog.h"
#include "sieveinfowidget.h"
#include <QHBoxLayout>
#include <KLocalizedString>
#include <QDialogButtonBox>

using namespace KSieveUi;
SieveInfoDialog::SieveInfoDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Sieve Server Support"));
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mSieveInfoWidget = new SieveInfoWidget(this);
    mSieveInfoWidget->setObjectName(QStringLiteral("mSieveInfoWidget"));
    mainLayout->addWidget(mSieveInfoWidget);

    QDialogButtonBox *button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &SieveInfoDialog::reject);
    resize(400, 300);
}

SieveInfoDialog::~SieveInfoDialog()
{
}

void SieveInfoDialog::setServerInfo(const QStringList &serverInfos)
{
    mSieveInfoWidget->setServerInfo(serverInfos);
}
