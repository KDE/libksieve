/*
  SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "sievetreewidgetitem.h"
using namespace Qt::Literals::StringLiterals;

#include <KIconLoader>
#include <KPixmapSequenceLoader>

#include <QTimer>

using namespace KSieveUi;
SieveTreeWidgetProgress::SieveTreeWidgetProgress(SieveTreeWidgetItem *item, QObject *parent)
    : QObject(parent)
    , mProgressPix(KPixmapSequenceLoader::load(u"process-working"_s, KIconLoader::SizeSmallMedium))
    , mProgressTimer(new QTimer(this))
    , mItem(item)
{
    connect(mProgressTimer, &QTimer::timeout, this, &SieveTreeWidgetProgress::slotTimerDone);
}

SieveTreeWidgetProgress::~SieveTreeWidgetProgress() = default;

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
    SieveTreeWidgetItemPrivate() = default;

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

SieveTreeWidgetItem::~SieveTreeWidgetItem() = default;

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
    setIcon(0, QIcon::fromTheme(u"network-server"_s));
}

#include "moc_sievetreewidgetitem.cpp"
