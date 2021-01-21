/*
  SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "sievetreewidgetitem.h"

#include <KIconLoader>

#include <QTimer>

using namespace KSieveUi;
SieveTreeWidgetProgress::SieveTreeWidgetProgress(SieveTreeWidgetItem *item, QObject *parent)
    : QObject(parent)
    , mItem(item)
{
    KIconLoader loader;
    mProgressPix = loader.loadPixmapSequence(QStringLiteral("process-working"), KIconLoader::SizeSmallMedium);
    mProgressTimer = new QTimer(this);
    connect(mProgressTimer, &QTimer::timeout, this, &SieveTreeWidgetProgress::slotTimerDone);
}

SieveTreeWidgetProgress::~SieveTreeWidgetProgress()
{
}

void SieveTreeWidgetProgress::slotTimerDone()
{
    mItem->setProgressAnimation(mProgressPix.frameAt(mProgressCount));
    ++mProgressCount;
    if (mProgressCount == 8) {
        mProgressCount = 0;
    }

    mProgressTimer->start(300);
}

void SieveTreeWidgetProgress::startAnimation()
{
    mProgressCount = 0;
    mProgressTimer->start(300);
}

void SieveTreeWidgetProgress::stopAnimation()
{
    if (mProgressTimer->isActive()) {
        mProgressTimer->stop();
    }
    mItem->setDefaultIcon();
}

class KSieveUi::SieveTreeWidgetItemPrivate
{
public:
    SieveTreeWidgetItemPrivate()
    {
    }

    ~SieveTreeWidgetItemPrivate()
    {
        delete mProgress;
    }

    SieveTreeWidgetProgress *mProgress = nullptr;
};

SieveTreeWidgetItem::SieveTreeWidgetItem(QTreeWidget *treeWidget, QTreeWidgetItem *item)
    : QTreeWidgetItem(treeWidget, item)
    , d(new KSieveUi::SieveTreeWidgetItemPrivate)
{
    d->mProgress = new SieveTreeWidgetProgress(this);
}

SieveTreeWidgetItem::~SieveTreeWidgetItem()
{
}

void SieveTreeWidgetItem::startAnimation()
{
    d->mProgress->startAnimation();
}

void SieveTreeWidgetItem::stopAnimation()
{
    d->mProgress->stopAnimation();
}

void SieveTreeWidgetItem::setProgressAnimation(const QPixmap &pix)
{
    setIcon(0, QIcon(pix));
}

void SieveTreeWidgetItem::setDefaultIcon()
{
    setIcon(0, QIcon::fromTheme(QStringLiteral("network-server")));
}
