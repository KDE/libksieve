/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SIEVEFOREVERYPARTWIDGET_H
#define SIEVEFOREVERYPARTWIDGET_H

#include "sievewidgetpageabstract.h"
class QCheckBox;
class QLineEdit;
class QXmlStreamReader;

namespace KSieveUi {
class SieveHelpButton;
class SieveForEveryPartWidget : public SieveWidgetPageAbstract
{
    Q_OBJECT
public:
    explicit SieveForEveryPartWidget(QWidget *parent = nullptr);
    ~SieveForEveryPartWidget() override;

    void generatedScript(QString &script, QStringList &required, bool inForEveryPartLoop) override;
    void loadScript(QXmlStreamReader &element, QString &error);

private:
    void slotHelp();
    QCheckBox *mForLoop = nullptr;
    QLineEdit *mName = nullptr;
    SieveHelpButton *mHelpButton = nullptr;
};
}

#endif // SIEVEFOREVERYPARTWIDGET_H
