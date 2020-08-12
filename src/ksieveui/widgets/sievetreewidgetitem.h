/*
  SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef SIEVETREEWIDGETITEM_H
#define SIEVETREEWIDGETITEM_H

#include <KPixmapSequence>
#include <QTreeWidgetItem>

#include "ksieveui_export.h"

class QTimer;
class QTreeWidget;
class QTreeWidgetItem;

namespace KSieveUi {
class SieveTreeWidgetProgress;
class SieveTreeWidgetItemPrivate;
/**
 * @brief The SieveTreeWidgetItem class
 * @author Laurent Montel <montel@kde.org>
 */
class KSIEVEUI_EXPORT SieveTreeWidgetItem : public QTreeWidgetItem
{
public:
    SieveTreeWidgetItem(QTreeWidget *treeWidget, QTreeWidgetItem *item);
    ~SieveTreeWidgetItem();

    void startAnimation();
    void stopAnimation();
    void setDefaultIcon();
    void setProgressAnimation(const QPixmap &pix);

private:
    SieveTreeWidgetItemPrivate *const d;
};

class SieveTreeWidgetProgress : public QObject
{
    Q_OBJECT
public:
    explicit SieveTreeWidgetProgress(SieveTreeWidgetItem *item, QObject *parent = nullptr);
    ~SieveTreeWidgetProgress();

    void startAnimation();
    void stopAnimation();

private:
    Q_DISABLE_COPY(SieveTreeWidgetProgress)
    void slotTimerDone();
    int mProgressCount = 0;
    KPixmapSequence mProgressPix;
    QTimer *mProgressTimer = nullptr;
    SieveTreeWidgetItem *mItem = nullptr;
};
}

#endif // SIEVETREEWIDGETITEM_H
