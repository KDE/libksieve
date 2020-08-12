/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#ifndef SIEVEACTIONADDHEADER_H
#define SIEVEACTIONADDHEADER_H

#include "sieveactionabstracteditheader.h"
namespace KSieveUi {
class SieveActionAddHeader : public SieveActionAbstractEditHeader
{
    Q_OBJECT
public:
    explicit SieveActionAddHeader(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent = nullptr);
    Q_REQUIRED_RESULT QString code(QWidget *w) const override;
    Q_REQUIRED_RESULT QWidget *createParamWidget(QWidget *parent) const override;
    void setParamWidgetValue(QXmlStreamReader &element, QWidget *parent, QString &error) override;
    Q_REQUIRED_RESULT QString help() const override;
    Q_REQUIRED_RESULT QUrl href() const override;
};
}

#endif // SIEVEACTIONADDHEADER_H
