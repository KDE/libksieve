/*
   SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sievehelpbutton.h"

#include <KIO/JobUiDelegateFactory>
#include <KIO/OpenUrlJob>
#include <KLocalizedString>
#include <QIcon>
#include <QWhatsThis>
#include <QWhatsThisClickedEvent>

using namespace KSieveUi;
SieveHelpButton::SieveHelpButton(QWidget *parent)
    : QToolButton(parent)
{
    setToolTip(i18nc("@info:tooltip", "Help"));
    setIcon(QIcon::fromTheme(QStringLiteral("help-hint")));
}

SieveHelpButton::~SieveHelpButton() = default;

bool SieveHelpButton::event(QEvent *event)
{
    if (event->type() == QEvent::WhatsThisClicked) {
        auto clicked = static_cast<QWhatsThisClickedEvent *>(event);
        auto job = new KIO::OpenUrlJob(QUrl(clicked->href()));
        job->setUiDelegate(KIO::createDefaultJobUiDelegate(KJobUiDelegate::AutoHandlingEnabled, this));
        job->start();
        return true;
    }
    return QToolButton::event(event);
}

#include "moc_sievehelpbutton.cpp"
