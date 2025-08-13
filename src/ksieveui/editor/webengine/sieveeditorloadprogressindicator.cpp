/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveeditorloadprogressindicator.h"
using namespace Qt::Literals::StringLiterals;

#include <KIconLoader>
#include <KPixmapSequenceLoader>

#include <QPixmap>
#include <QTimer>
using namespace KSieveUi;
SieveEditorLoadProgressIndicator::SieveEditorLoadProgressIndicator(QObject *parent)
    : QObject(parent)
    , mProgressPix(KPixmapSequenceLoader::load(u"process-working"_s, KIconLoader::SizeSmallMedium))
    , mProgressTimer(new QTimer(this))
{
    connect(mProgressTimer, &QTimer::timeout, this, &SieveEditorLoadProgressIndicator::slotTimerDone);
}

SieveEditorLoadProgressIndicator::~SieveEditorLoadProgressIndicator() = default;

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

#include "moc_sieveeditorloadprogressindicator.cpp"
