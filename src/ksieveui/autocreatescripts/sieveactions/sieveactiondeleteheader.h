/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "sieveactionabstracteditheader.h"
namespace KSieveUi
{
class SieveActionDeleteHeader : public SieveActionAbstractEditHeader
{
    Q_OBJECT
public:
    explicit SieveActionDeleteHeader(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent = nullptr);
    Q_REQUIRED_RESULT QString code(QWidget *w) const override;
    Q_REQUIRED_RESULT QWidget *createParamWidget(QWidget *parent) const override;
    void setParamWidgetValue(QXmlStreamReader &element, QWidget *parent, QString &error) override;
    Q_REQUIRED_RESULT QString help() const override;
    Q_REQUIRED_RESULT QUrl href() const override;
    Q_REQUIRED_RESULT QStringList needRequires(QWidget *parent) const override;

private:
    void parseValue(QXmlStreamReader &element, QWidget *w, QString &error, bool isNegative);
};
}
