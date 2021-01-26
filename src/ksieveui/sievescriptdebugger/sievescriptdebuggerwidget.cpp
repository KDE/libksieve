/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sievescriptdebuggerwidget.h"
#include "sievescriptdebuggerfrontendwidget.h"

#include <KLocalizedString>

#include <QHBoxLayout>
#include <QLabel>
#include <QStackedWidget>
#include <QStandardPaths>
#include <QTimer>

using namespace KSieveUi;
SieveScriptDebuggerWidget::SieveScriptDebuggerWidget(QWidget *parent)
    : QWidget(parent)
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins({});
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

    QTimer::singleShot(0, this, &SieveScriptDebuggerWidget::checkSieveTestApplication);
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
        mHaveDebugApps = false;
        mStackedWidget->setCurrentWidget(mSieveNoExistingFrontEnd);
        Q_EMIT sieveTestNotFound();
    } else {
        mHaveDebugApps = true;
        mStackedWidget->setCurrentWidget(mSieveScriptFrontEnd);
    }
}

bool SieveScriptDebuggerWidget::haveDebugApps() const
{
    return mHaveDebugApps;
}
