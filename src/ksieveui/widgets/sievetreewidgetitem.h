/*
  SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include "ksieveui_export.h"
#include <KPixmapSequence>
#include <QTreeWidgetItem>
#include <memory>

class QTimer;
class QTreeWidget;

namespace KSieveUi
{
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
    ~SieveTreeWidgetItem() override;

    void startAnimation();
    void stopAnimation();
    void setDefaultIcon();
    void setProgressAnimation(const QPixmap &pix);

private:
    std::unique_ptr<SieveTreeWidgetItemPrivate> const d;
};

class SieveTreeWidgetProgress : public QObject
{
    Q_OBJECT
public:
    explicit SieveTreeWidgetProgress(SieveTreeWidgetItem *item, QObject *parent = nullptr);
    ~SieveTreeWidgetProgress() override;

    void startAnimation();
    void stopAnimation();

private:
    Q_DISABLE_COPY(SieveTreeWidgetProgress)
    void slotTimerDone();
    int mProgressCount = 0;
    KPixmapSequence mProgressPix;
    QTimer *mProgressTimer = nullptr;
    SieveTreeWidgetItem *const mItem;
};
}
