/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "sievewidgetpageabstract.h"
class QCheckBox;
class QLineEdit;
class QXmlStreamReader;

namespace KSieveUi
{
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
