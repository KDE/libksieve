/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#ifndef SIEVEEDITORLOADPROGRESSINDICATOR_H
#define SIEVEEDITORLOADPROGRESSINDICATOR_H

#include <QObject>
#include <KPixmapSequence>
class QTimer;
namespace KSieveUi {
class SieveEditorLoadProgressIndicator : public QObject
{
    Q_OBJECT
public:
    explicit SieveEditorLoadProgressIndicator(QObject *parent = nullptr);
    ~SieveEditorLoadProgressIndicator();

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
    QTimer *mProgressTimer = nullptr;
};
}

#endif // SIEVEEDITORLOADPROGRESSINDICATOR_H
