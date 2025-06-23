/*
   SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sievescriptdebuggerwidget.h"
using namespace Qt::Literals::StringLiterals;

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
    mStackedWidget->setObjectName(QLatin1StringView("stackedwidget"));
    mainLayout->addWidget(mStackedWidget);

    mSieveScriptFrontEnd = new SieveScriptDebuggerFrontEndWidget;
    mSieveScriptFrontEnd->setObjectName(QLatin1StringView("sievescriptfrontend"));
    mStackedWidget->addWidget(mSieveScriptFrontEnd);
    connect(mSieveScriptFrontEnd, &SieveScriptDebuggerFrontEndWidget::scriptTextChanged, this, &SieveScriptDebuggerWidget::scriptTextChanged);
    connect(mSieveScriptFrontEnd, &SieveScriptDebuggerFrontEndWidget::debugButtonEnabled, this, &SieveScriptDebuggerWidget::debugButtonEnabled);
    connect(this, &SieveScriptDebuggerWidget::debugScriptButtonClicked, mSieveScriptFrontEnd, &SieveScriptDebuggerFrontEndWidget::debugScriptButtonClicked);

    mSieveNoExistingFrontEnd = new QLabel(i18nc("@label:textbox", "\"sieve-test\" was not found on system. Please install it. (See in Dovecot package)"));
    mSieveNoExistingFrontEnd->setAlignment(Qt::AlignHCenter);
    QFont f = mSieveNoExistingFrontEnd->font();
    f.setBold(true);
    mSieveNoExistingFrontEnd->setFont(f);
    mSieveNoExistingFrontEnd->setObjectName(QLatin1StringView("sievenoexistingfrontend"));
    mStackedWidget->addWidget(mSieveNoExistingFrontEnd);

    QTimer::singleShot(0, this, &SieveScriptDebuggerWidget::checkSieveTestApplication);
}

SieveScriptDebuggerWidget::~SieveScriptDebuggerWidget() = default;

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
        return {};
    }
}

void SieveScriptDebuggerWidget::checkSieveTestApplication()
{
    if (QStandardPaths::findExecutable(u"sieve-test"_s).isEmpty()) {
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

#include "moc_sievescriptdebuggerwidget.cpp"
