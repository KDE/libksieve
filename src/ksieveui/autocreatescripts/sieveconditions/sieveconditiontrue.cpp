/*
   Copyright (C) 2013-2016 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#include "sieveconditiontrue.h"
#include "editor/sieveeditorutil.h"
#include <KLocalizedString>

#include <QHBoxLayout>
#include <QLabel>

using namespace KSieveUi;

SieveConditionTrue::SieveConditionTrue(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, QStringLiteral("true"), i18n("True"), parent)
{
}

QWidget *SieveConditionTrue::createParamWidget(QWidget *parent) const
{
    QWidget *w = new QWidget(parent);
    QHBoxLayout *lay = new QHBoxLayout;
    lay->setMargin(0);
    w->setLayout(lay);

    QLabel *label = new QLabel(i18n("true"));
    lay->addWidget(label);
    return w;
}

QString SieveConditionTrue::code(QWidget *) const
{
    return QStringLiteral("true");
}

QString SieveConditionTrue::help() const
{
    return i18n("The \"true\" test always evaluates to true.");
}

bool SieveConditionTrue::setParamWidgetValue(const QDomElement &, QWidget *, bool, QString &)
{
    //Nothing
    return true;
}

QUrl SieveConditionTrue::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}
