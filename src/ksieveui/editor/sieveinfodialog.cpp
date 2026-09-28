/*
  SPDX-FileCopyrightText: 2014-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#include "sieveinfodialog.h"

#include "sieveinfowidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <TextAddonsWidgets/LoadDialogSizeUtils>

namespace
{
const char myConfigGroupName[] = "SieveInfoDialog";
}

using namespace Qt::Literals::StringLiterals;
using namespace KSieveUi;
SieveInfoDialog::SieveInfoDialog(QWidget *parent)
    : QDialog(parent)
    , mSieveInfoWidget(new SieveInfoWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Sieve Server Support"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QLatin1StringView("mainLayout"));

    mSieveInfoWidget->setObjectName(QLatin1StringView("mSieveInfoWidget"));
    mainLayout->addWidget(mSieveInfoWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QLatin1StringView("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &SieveInfoDialog::reject);
    readConfig();
}

SieveInfoDialog::~SieveInfoDialog() = default;

void SieveInfoDialog::setServerInfo(const QStringList &serverInfos)
{
    mSieveInfoWidget->setServerInfo(serverInfos);
}

void SieveInfoDialog::readConfig()
{
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myConfigGroupName), QSize(400, 300));
}

#include "moc_sieveinfodialog.cpp"
