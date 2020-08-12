/*
  SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "sievetexteditwidgettest.h"
#include "../sievetexteditwidget.h"
#include <QTest>
#include <editor/sievetextedit.h>
#include <KPIMTextEdit/PlainTextEditFindBar>
#include <KPIMTextEdit/SlideContainer>

SieveTextEditWidgetTest::SieveTextEditWidgetTest(QObject *parent)
    : QObject(parent)
{
}

SieveTextEditWidgetTest::~SieveTextEditWidgetTest()
{
}

void SieveTextEditWidgetTest::shouldHaveDefaultValue()
{
    KSieveUi::SieveTextEditWidget w;
    KSieveUi::SieveTextEdit *textedit = w.findChild<KSieveUi::SieveTextEdit *>(QStringLiteral("textedit"));
    QVERIFY(textedit);
    QVERIFY(!textedit->isReadOnly());

    KPIMTextEdit::SlideContainer *slider = w.findChild<KPIMTextEdit::SlideContainer *>(QStringLiteral("slidercontainer"));
    QVERIFY(slider);
    QWidget *contentWidget = slider->content();
    QVERIFY(contentWidget);
    QVERIFY(dynamic_cast<KPIMTextEdit::PlainTextEditFindBar *>(contentWidget));

    QVERIFY(w.textEdit());
}

QTEST_MAIN(SieveTextEditWidgetTest)
