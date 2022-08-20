/*
   SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sievehelpbutton.h"

#include <kio_version.h>
#if KIO_VERSION >= QT_VERSION_CHECK(5, 98, 0)
#include <KIO/JobUiDelegateFactory>
#else
#include <KIO/JobUiDelegate>
#endif
#include <KIO/OpenUrlJob>
#include <KLocalizedString>
#include <QIcon>
#include <QWhatsThis>
#include <QWhatsThisClickedEvent>

using namespace KSieveUi;
SieveHelpButton::SieveHelpButton(QWidget *parent)
    : QToolButton(parent)
{
    setToolTip(i18n("Help"));
    setIcon(QIcon::fromTheme(QStringLiteral("help-hint")));
}

SieveHelpButton::~SieveHelpButton() = default;

bool SieveHelpButton::event(QEvent *event)
{
    if (event->type() == QEvent::WhatsThisClicked) {
        auto clicked = static_cast<QWhatsThisClickedEvent *>(event);
        auto job = new KIO::OpenUrlJob(QUrl(clicked->href()));
#if KIO_VERSION >= QT_VERSION_CHECK(5, 98, 0)
        job->setUiDelegate(KIO::createDefaultJobUiDelegate(KJobUiDelegate::AutoHandlingEnabled, this));
#else
        job->setUiDelegate(new KIO::JobUiDelegate(KJobUiDelegate::AutoHandlingEnabled, this));
#endif
        job->start();
        return true;
    }
    return QToolButton::event(event);
}
