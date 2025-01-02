/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sievewidgetpageabstract.h"

using namespace KSieveUi;

SieveWidgetPageAbstract::SieveWidgetPageAbstract(QWidget *parent)
    : QWidget(parent)
{
}

SieveWidgetPageAbstract::~SieveWidgetPageAbstract() = default;

void SieveWidgetPageAbstract::setPageType(PageType type)
{
    mType = type;
}

SieveWidgetPageAbstract::PageType SieveWidgetPageAbstract::pageType() const
{
    return mType;
}

#include "moc_sievewidgetpageabstract.cpp"
