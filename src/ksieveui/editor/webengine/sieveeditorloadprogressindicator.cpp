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
#include "sieveeditorloadprogressindicator.h"

#include <KIconLoader>

#include <QTimer>
#include <QPixmap>
using namespace KSieveUi;
SieveEditorLoadProgressIndicator::SieveEditorLoadProgressIndicator(QObject *parent)
    : QObject(parent)
    , mProgressCount(0)
{
    mProgressPix = KIconLoader::global()->loadPixmapSequence(QStringLiteral("process-working"), KIconLoader::SizeSmallMedium);
    mProgressTimer = new QTimer(this);
    connect(mProgressTimer, &QTimer::timeout, this, &SieveEditorLoadProgressIndicator::slotTimerDone);
}

SieveEditorLoadProgressIndicator::~SieveEditorLoadProgressIndicator()
{
}

void SieveEditorLoadProgressIndicator::startAnimation()
{
    mProgressCount = 0;
    mProgressTimer->start(300);
}

void SieveEditorLoadProgressIndicator::stopAnimation(bool success)
{
    if (mProgressTimer->isActive()) {
        mProgressTimer->stop();
    }
    Q_EMIT loadFinished(success);
}

void SieveEditorLoadProgressIndicator::slotTimerDone()
{
    Q_EMIT pixmapChanged(mProgressPix.frameAt(mProgressCount));
    ++mProgressCount;
    if (mProgressCount == 8) {
        mProgressCount = 0;
    }

    mProgressTimer->start(300);
}
