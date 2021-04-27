/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveeditorloadprogressindicator.h"

#include <KIconLoader>

#include <QPixmap>
#include <QTimer>
using namespace KSieveUi;
SieveEditorLoadProgressIndicator::SieveEditorLoadProgressIndicator(QObject *parent)
    : QObject(parent)
    , mProgressTimer(new QTimer(this))
{
    mProgressPix = KIconLoader::global()->loadPixmapSequence(QStringLiteral("process-working"), KIconLoader::SizeSmallMedium);
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
