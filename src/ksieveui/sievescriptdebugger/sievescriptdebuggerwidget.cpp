/*
   Copyright (C) 2015-2018 Laurent Montel <montel@kde.org>

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

#include "sievescriptdebuggerwidget.h"
#include "sievescriptdebuggerfrontendwidget.h"

#include <KLocalizedString>

#include <QStackedWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QStandardPaths>

using namespace KSieveUi;
SieveScriptDebuggerWidget::SieveScriptDebuggerWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setMargin(0);
    mStackedWidget = new QStackedWidget(this);
    mStackedWidget->setObjectName(QStringLiteral("stackedwidget"));
    mainLayout->addWidget(mStackedWidget);

    mSieveScriptFrontEnd = new SieveScriptDebuggerFrontEndWidget;
    mSieveScriptFrontEnd->setObjectName(QStringLiteral("sievescriptfrontend"));
    mStackedWidget->addWidget(mSieveScriptFrontEnd);
    connect(mSieveScriptFrontEnd, &SieveScriptDebuggerFrontEndWidget::scriptTextChanged, this, &SieveScriptDebuggerWidget::scriptTextChanged);
    connect(mSieveScriptFrontEnd, &SieveScriptDebuggerFrontEndWidget::debugButtonEnabled, this, &SieveScriptDebuggerWidget::debugButtonEnabled);
    connect(this, &SieveScriptDebuggerWidget::debugScriptButtonClicked, mSieveScriptFrontEnd, &SieveScriptDebuggerFrontEndWidget::debugScriptButtonClicked);

    mSieveNoExistingFrontEnd = new QLabel(i18n("\"sieve-test\" was not found on system. Please install it. (See in Dovecot package)"));
    mSieveNoExistingFrontEnd->setAlignment(Qt::AlignHCenter);
    QFont f = mSieveNoExistingFrontEnd->font();
    f.setBold(true);
    mSieveNoExistingFrontEnd->setFont(f);
    mSieveNoExistingFrontEnd->setObjectName(QStringLiteral("sievenoexistingfrontend"));
    mStackedWidget->addWidget(mSieveNoExistingFrontEnd);

    checkSieveTestApplication();
}

SieveScriptDebuggerWidget::~SieveScriptDebuggerWidget()
{
}

QList<int> SieveScriptDebuggerWidget::splitterSizes() const
{
    return mSieveScriptFrontEnd->splitterSizes();
}

void SieveScriptDebuggerWidget::setSplitterSizes(const QList<int> &sizes)
{
    mSieveScriptFrontEnd->setSplitterSizes(sizes);
}

bool SieveScriptDebuggerWidget::canAccept() const
{
    return mSieveScriptFrontEnd->canAccept();
}

void SieveScriptDebuggerWidget::setScript(const QString &script)
{
    if (mStackedWidget->currentWidget() == mSieveScriptFrontEnd) {
        mSieveScriptFrontEnd->setScript(script);
    }
}

QString SieveScriptDebuggerWidget::script() const
{
    if (mStackedWidget->currentWidget() == mSieveScriptFrontEnd) {
        return mSieveScriptFrontEnd->script();
    } else {
        return QString();
    }
}

void SieveScriptDebuggerWidget::checkSieveTestApplication()
{
    if (QStandardPaths::findExecutable(QStringLiteral("sieve-test")).isEmpty()) {
        mStackedWidget->setCurrentWidget(mSieveNoExistingFrontEnd);
    } else {
        mStackedWidget->setCurrentWidget(mSieveScriptFrontEnd);
    }
}
