/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveconditionspamtest.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "autocreatescripts/sieveeditorgraphicalmodewidget.h"
#include "editor/sieveeditorutil.h"
#include "widgets/selectcomparatorcombobox.h"
#include "widgets/selectrelationalmatchtype.h"

#include <KLocalizedString>

#include "libksieveui_debug.h"
#include <QCheckBox>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QXmlStreamReader>

using namespace KSieveUi;

SieveConditionSpamTest::SieveConditionSpamTest(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, QStringLiteral("spamtest"), i18n("Spam Test"), parent)
{
    mHasSpamTestPlusSupport = sieveCapabilities().contains(QLatin1String("spamtestplus"));
}

QWidget *SieveConditionSpamTest::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto lay = new QVBoxLayout;
    lay->setContentsMargins({});
    w->setLayout(lay);

    if (mHasSpamTestPlusSupport) {
        auto percent = new QCheckBox(i18n("Percent"));
        percent->setObjectName(QStringLiteral("percent"));
        connect(percent, &QCheckBox::toggled, this, &SieveConditionSpamTest::valueChanged);
        lay->addWidget(percent);
    }

    auto relation = new SelectRelationalMatchType;
    connect(relation, &SelectRelationalMatchType::valueChanged, this, &SieveConditionSpamTest::valueChanged);
    relation->setObjectName(QStringLiteral("relation"));
    lay->addWidget(relation);

    auto comparator = new SelectComparatorComboBox(mSieveGraphicalModeWidget);
    connect(comparator, &SelectComparatorComboBox::valueChanged, this, &SieveConditionSpamTest::valueChanged);
    comparator->setObjectName(QStringLiteral("comparator"));
    lay->addWidget(comparator);

    auto spinbox = new QSpinBox;
    spinbox->setMaximum(10);
    spinbox->setMinimum(0);
    spinbox->setObjectName(QStringLiteral("value"));
    connect(spinbox, &QSpinBox::valueChanged, this, &SieveConditionSpamTest::valueChanged);
    lay->addWidget(spinbox);
    return w;
}

QString SieveConditionSpamTest::code(QWidget *w) const
{
    QString percentStr;
    if (mHasSpamTestPlusSupport) {
        const QCheckBox *checkbox = w->findChild<QCheckBox *>(QStringLiteral("percent"));
        percentStr = checkbox->isChecked() ? QStringLiteral(":percent") : QString();
    }

    const SelectRelationalMatchType *relation = w->findChild<SelectRelationalMatchType *>(QStringLiteral("relation"));
    const QString relationStr = relation->code();

    const SelectComparatorComboBox *comparator = w->findChild<SelectComparatorComboBox *>(QStringLiteral("comparator"));
    const QString comparatorStr = comparator->code();

    const QSpinBox *spinbox = w->findChild<QSpinBox *>(QStringLiteral("value"));
    const QString value = QString::number(spinbox->value());

    return QStringLiteral("spamtest %1 %2 %3 \"%4\"").arg(percentStr, relationStr, comparatorStr, value)
        + AutoCreateScriptUtil::generateConditionComment(comment());
}

bool SieveConditionSpamTest::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveConditionSpamTest::serverNeedsCapability() const
{
    return QStringLiteral("spamtest");
}

QStringList SieveConditionSpamTest::needRequires(QWidget *w) const
{
    const SelectComparatorComboBox *comparator = w->findChild<SelectComparatorComboBox *>(QStringLiteral("comparator"));
    QStringList lst;
    lst << QStringLiteral("spamtest") << QStringLiteral("relational");
    const QString comparatorRequires = comparator->require();
    if (!comparatorRequires.isEmpty()) {
        lst << comparatorRequires;
    }
    if (mHasSpamTestPlusSupport) {
        lst << QStringLiteral("spamtestplus");
    }
    return lst;
}

QString SieveConditionSpamTest::help() const
{
    return i18n(
        "Sieve implementations that implement the \"spamtest\" test use an identifier of either \"spamtest\" or \"spamtestplus\" for use with the capability "
        "mechanism.");
}

void SieveConditionSpamTest::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, bool /*notCondition*/, QString &error)
{
    QString commentStr;
    while (element.readNextStartElement()) {
        const QStringView tagName = element.name();
        if (tagName == QLatin1String("tag")) {
            const QString tagValue = element.readElementText();
            if (tagValue == QLatin1String("count") || tagValue == QLatin1String("value")) {
                if (element.readNextStartElement()) {
                    if (element.name() == QLatin1String("str")) {
                        auto relation = w->findChild<SelectRelationalMatchType *>(QStringLiteral("relation"));
                        relation->setCode(AutoCreateScriptUtil::tagValue(tagValue), element.readElementText(), name(), error);
                    }
                }
            } else if (tagValue == QLatin1String("comparator")) {
                if (element.readNextStartElement()) {
                    if (element.name() == QLatin1String("str")) {
                        auto comparator = w->findChild<SelectComparatorComboBox *>(QStringLiteral("comparator"));
                        comparator->setCode(element.readElementText(), name(), error);
                    }
                }
            } else if (tagValue == QLatin1String("percent")) {
                if (mHasSpamTestPlusSupport) {
                    auto checkbox = w->findChild<QCheckBox *>(QStringLiteral("percent"));
                    checkbox->setChecked(true);
                } else {
                    serverDoesNotSupportFeatures(QStringLiteral("percent"), error);
                    qCDebug(LIBKSIEVE_LOG) << " SieveConditionSpamTest::setParamWidgetValue server has not percent support";
                }
            } else {
                unknownTagValue(tagValue, error);
                qCDebug(LIBKSIEVE_LOG) << " SieveConditionSpamTest::setParamWidgetValue unknown tagvalue " << tagValue;
            }
        } else if (tagName == QLatin1String("str")) {
            auto spinbox = w->findChild<QSpinBox *>(QStringLiteral("value"));
            spinbox->setValue(element.readElementText().toInt());
        } else if (tagName == QLatin1String("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1String("comment")) {
            commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, element.readElementText());
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVE_LOG) << " SieveSpamTest::setParamWidgetValue unknown tagName " << tagName;
        }
    }
    if (!commentStr.isEmpty()) {
        setComment(commentStr);
    }
}

QUrl SieveConditionSpamTest::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

#include "moc_sieveconditionspamtest.cpp"
