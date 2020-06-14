/*
   Copyright (C) 2013-2020 Laurent Montel <montel@kde.org>

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
#include "sievehelpbutton.h"

#include <KLocalizedString>
#include <QIcon>
#include <QWhatsThisClickedEvent>
#include <QWhatsThis>
#include <KIO/JobUiDelegate>
#include <KIO/OpenUrlJob>

using namespace KSieveUi;
SieveHelpButton::SieveHelpButton(QWidget *parent)
    : QToolButton(parent)
{
    setToolTip(i18n("Help"));
    setIcon(QIcon::fromTheme(QStringLiteral("help-hint")));
}

SieveHelpButton::~SieveHelpButton()
{
}

bool SieveHelpButton::event(QEvent *event)
{
    if (event->type() == QEvent::WhatsThisClicked) {
        QWhatsThisClickedEvent *clicked = static_cast<QWhatsThisClickedEvent *>(event);
        KIO::OpenUrlJob *job = new KIO::OpenUrlJob(QUrl(clicked->href()));
        job->setUiDelegate(new KIO::JobUiDelegate(KJobUiDelegate::AutoHandlingEnabled, this));
        job->start();
        return true;
    }
    return QToolButton::event(event);
}
