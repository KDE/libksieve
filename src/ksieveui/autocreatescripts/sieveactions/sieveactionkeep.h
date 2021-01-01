/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#ifndef SIEVEACTIONKEEP_H
#define SIEVEACTIONKEEP_H
#include "sieveaction.h"
namespace KSieveUi {
class SieveActionKeep : public SieveAction
{
    Q_OBJECT
public:
    explicit SieveActionKeep(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent = nullptr);

    Q_REQUIRED_RESULT QString code(QWidget *) const override;
    Q_REQUIRED_RESULT QString help() const override;
    Q_REQUIRED_RESULT QWidget *createParamWidget(QWidget *parent) const override;
    void setParamWidgetValue(QXmlStreamReader &element, QWidget *w, QString &error) override;
    Q_REQUIRED_RESULT QStringList needRequires(QWidget *) const override;
    Q_REQUIRED_RESULT QUrl href() const override;

private:
    bool mHasFlagSupport = false;
    bool mHasImapFlag4Support = false;
};
}
#endif // SIEVEACTIONKEEP_H
