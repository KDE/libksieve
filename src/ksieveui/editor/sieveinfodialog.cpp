/*
  Copyright (C) 2014-2020 Laurent Montel <montel@kde.org>

  This library is free software; you can redistribute it and/or modify it
  under the terms of the GNU Library General Public License as published by
  the Free Software Foundation; either version 2 of the License, or (at your
  option) any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
  License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to the
  Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301, USA.

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
