/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <KPixmapSequence>
#include <QObject>
class QTimer;
namespace KSieveUi
{
class SieveEditorLoadProgressIndicator : public QObject
{
    Q_OBJECT
public:
    explicit SieveEditorLoadProgressIndicator(QObject *parent = nullptr);
    ~SieveEditorLoadProgressIndicator() override;

    void startAnimation();
    void stopAnimation(bool success);

Q_SIGNALS:
    void pixmapChanged(const QPixmap &);
    void loadFinished(bool success);

private:
    Q_DISABLE_COPY(SieveEditorLoadProgressIndicator)
    void slotTimerDone();
    int mProgressCount = 0;
    KPixmapSequence mProgressPix;
    QTimer *const mProgressTimer;
};
}

