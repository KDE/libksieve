/*
  SPDX-FileCopyrightText: 2015-2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "sievetexteditwidgettest.h"
#include "../sievetexteditwidget.h"
#include <KPIMTextEdit/PlainTextEditFindBar>
#include <KPIMTextEdit/SlideContainer>
#include <QTest>
#include <editor/sievetextedit.h>

SieveTextEditWidgetTest::SieveTextEditWidgetTest(QObject *parent)
    : QObject(parent)
{
}

SieveTextEditWidgetTest::~SieveTextEditWidgetTest() = default;

void SieveTextEditWidgetTest::shouldHaveDefaultValue()
{
    KSieveUi::SieveTextEditWidget w;
    auto textedit = w.findChild<KSieveUi::SieveTextEdit *>(QStringLiteral("textedit"));
    QVERIFY(textedit);
    QVERIFY(!textedit->isReadOnly());

    auto slider = w.findChild<KPIMTextEdit::SlideContainer *>(QStringLiteral("slidercontainer"));
    QVERIFY(slider);
    QWidget *contentWidget = slider->content();
    QVERIFY(contentWidget);
    QVERIFY(dynamic_cast<KPIMTextEdit::PlainTextEditFindBar *>(contentWidget));

    QVERIFY(w.textEdit());
}

QTEST_MAIN(SieveTextEditWidgetTest)
