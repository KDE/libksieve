/*
  SPDX-FileCopyrightText: 2015-2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "sievetexteditwidgettest.h"
#include "../sievetexteditwidget.h"
#include "editor/sievetextedit.h"
#include <QTest>
#include <TextAddonsWidgets/SlideContainer>
#include <TextCustomEditor/PlainTextEditFindBar>

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

    auto slider = w.findChild<TextAddonsWidgets::SlideContainer *>(QStringLiteral("slidercontainer"));
    QVERIFY(slider);
    QWidget *contentWidget = slider->content();
    QVERIFY(contentWidget);
    QVERIFY(dynamic_cast<TextCustomEditor::PlainTextEditFindBar *>(contentWidget));

    QVERIFY(w.textEdit());
}

QTEST_MAIN(SieveTextEditWidgetTest)

#include "moc_sievetexteditwidgettest.cpp"
