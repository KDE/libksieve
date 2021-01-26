/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveconditionvirustest.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "editor/sieveeditorutil.h"
#include "widgets/selectcomparatorcombobox.h"
#include "widgets/selectrelationalmatchtype.h"

#include <KLocalizedString>

#include "libksieve_debug.h"
#include <QSpinBox>
#include <QVBoxLayout>
#include <QXmlStreamReader>

using namespace KSieveUi;

SieveConditionVirusTest::SieveConditionVirusTest(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, QStringLiteral("virustest"), i18n("Virus Test"), parent)
{
}

QWidget *SieveConditionVirusTest::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto lay = new QVBoxLayout;
    lay->setContentsMargins({});
    w->setLayout(lay);

    auto relation = new SelectRelationalMatchType;
    connect(relation, &SelectRelationalMatchType::valueChanged, this, &SieveConditionVirusTest::valueChanged);
    relation->setObjectName(QStringLiteral("relation"));
    lay->addWidget(relation);

    auto comparator = new SelectComparatorComboBox(mSieveGraphicalModeWidget);
    connect(comparator, &SelectComparatorComboBox::valueChanged, this, &SieveConditionVirusTest::valueChanged);
    comparator->setObjectName(QStringLiteral("comparator"));
    lay->addWidget(comparator);

    auto spinbox = new QSpinBox;
    spinbox->setMaximum(5);
    spinbox->setMinimum(0);
    spinbox->setObjectName(QStringLiteral("value"));
    connect(spinbox, QOverload<int>::of(&QSpinBox::valueChanged), this, &SieveConditionVirusTest::valueChanged);
    lay->addWidget(spinbox);
    return w;
}

QString SieveConditionVirusTest::code(QWidget *w) const
{
    const SelectRelationalMatchType *relation = w->findChild<SelectRelationalMatchType *>(QStringLiteral("relation"));
    const QString relationStr = relation->code();

    const SelectComparatorComboBox *comparator = w->findChild<SelectComparatorComboBox *>(QStringLiteral("comparator"));
    const QString comparatorStr = comparator->code();

    const QSpinBox *spinbox = w->findChild<QSpinBox *>(QStringLiteral("value"));
    const QString value = QString::number(spinbox->value());

    return QStringLiteral("virustest %1 %2 \"%3\"").arg(relationStr, comparatorStr, value) + AutoCreateScriptUtil::generateConditionComment(comment());
}

bool SieveConditionVirusTest::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveConditionVirusTest::serverNeedsCapability() const
{
    return QStringLiteral("virustest");
}

QStringList SieveConditionVirusTest::needRequires(QWidget *w) const
{
    const SelectComparatorComboBox *comparator = w->findChild<SelectComparatorComboBox *>(QStringLiteral("comparator"));
    const QString comparatorRequires = comparator->require();
    QStringList lst;
    lst << QStringLiteral("spamtest") << QStringLiteral("relational");
    if (!comparatorRequires.isEmpty()) {
        lst << comparatorRequires;
    }
    return lst;
}

QString SieveConditionVirusTest::help() const
{
    return i18n("Sieve implementations that implement the \"virustest\" test have an identifier of \"virustest\" for use with the capability mechanism.");
}

void SieveConditionVirusTest::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, bool /*notCondition*/, QString &error)
{
    QString commentStr;
    while (element.readNextStartElement()) {
        const QStringRef tagName = element.name();

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
            } else {
                unknownTagValue(tagValue, error);
                qCDebug(LIBKSIEVE_LOG) << " SieveConditionVirusTest::setParamWidgetValue unknown tagValue " << tagValue;
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
            qCDebug(LIBKSIEVE_LOG) << " SieveConditionVirusTest::setParamWidgetValue unknown tagName " << tagName;
        }
    }
    if (!commentStr.isEmpty()) {
        setComment(commentStr);
    }
}

QUrl SieveConditionVirusTest::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}
