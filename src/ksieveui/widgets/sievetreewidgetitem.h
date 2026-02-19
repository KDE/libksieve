/*
  SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

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
/*!
 * \class KSieveUi::SieveTreeWidgetItem
 * \brief The SieveTreeWidgetItem class
 * \author Laurent Montel <montel@kde.org>
 * \inmodule KSieveUi
 * \inheaderfile KSieveUi/SieveTreeWidgetItem
 */
class KSIEVEUI_EXPORT SieveTreeWidgetItem : public QTreeWidgetItem
{
public:
    /*!
     * Constructs a SieveTreeWidgetItem with the given tree widget and parent item.
     * \param treeWidget The parent tree widget.
     * \param item The parent tree widget item.
     */
    SieveTreeWidgetItem(QTreeWidget *treeWidget, QTreeWidgetItem *item);
    /*!
     * Destroys the SieveTreeWidgetItem.
     */
    ~SieveTreeWidgetItem() override;

    /*!
     * Starts an animation for this item.
     */
    void startAnimation();
    /*!
     * Stops the animation for this item.
     */
    void stopAnimation();
    /*!
     * Sets the item to display its default icon.
     */
    void setDefaultIcon();
    /*!
     * Sets a progress animation pixmap for this item.
     * \param pix The pixmap to use for animation.
     */
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
    const KPixmapSequence mProgressPix;
    QTimer *const mProgressTimer;
    SieveTreeWidgetItem *const mItem;
};
}
