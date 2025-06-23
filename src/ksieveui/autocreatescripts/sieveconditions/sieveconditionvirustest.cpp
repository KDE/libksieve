/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveconditionvirustest.h"
using namespace Qt::Literals::StringLiterals;

#include "autocreatescripts/autocreatescriptutil_p.h"
#include "editor/sieveeditorutil.h"
#include "widgets/selectcomparatorcombobox.h"
#include "widgets/selectrelationalmatchtype.h"

#include <KLocalizedString>

#include "libksieveui_debug.h"
#include <QSpinBox>
#include <QVBoxLayout>
#include <QXmlStreamReader>

using namespace KSieveUi;

SieveConditionVirusTest::SieveConditionVirusTest(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, u"virustest"_s, i18n("Virus Test"), parent)
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
    relation->setObjectName(QLatin1StringView("relation"));
    lay->addWidget(relation);

    auto comparator = new SelectComparatorComboBox(mSieveGraphicalModeWidget);
    connect(comparator, &SelectComparatorComboBox::valueChanged, this, &SieveConditionVirusTest::valueChanged);
    comparator->setObjectName(QLatin1StringView("comparator"));
    lay->addWidget(comparator);

    auto spinbox = new QSpinBox;
    spinbox->setMaximum(5);
    spinbox->setMinimum(0);
    spinbox->setObjectName(QLatin1StringView("value"));
    connect(spinbox, &QSpinBox::valueChanged, this, &SieveConditionVirusTest::valueChanged);
    lay->addWidget(spinbox);
    return w;
}

QString SieveConditionVirusTest::code(QWidget *w) const
{
    const SelectRelationalMatchType *relation = w->findChild<SelectRelationalMatchType *>(u"relation"_s);
    const QString relationStr = relation->code();

    const SelectComparatorComboBox *comparator = w->findChild<SelectComparatorComboBox *>(u"comparator"_s);
    const QString comparatorStr = comparator->code();

    const QSpinBox *spinbox = w->findChild<QSpinBox *>(u"value"_s);
    const QString value = QString::number(spinbox->value());

    return u"virustest %1 %2 \"%3\""_s.arg(relationStr, comparatorStr, value) + AutoCreateScriptUtil::generateConditionComment(comment());
}

bool SieveConditionVirusTest::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveConditionVirusTest::serverNeedsCapability() const
{
    return u"virustest"_s;
}

QStringList SieveConditionVirusTest::needRequires(QWidget *w) const
{
    const SelectComparatorComboBox *comparator = w->findChild<SelectComparatorComboBox *>(u"comparator"_s);
    const QString comparatorRequires = comparator->require();
    QStringList lst;
    lst << u"spamtest"_s << u"relational"_s;
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
        const QStringView tagName = element.name();

        if (tagName == QLatin1StringView("tag")) {
            const QString tagValue = element.readElementText();
            if (tagValue == QLatin1StringView("count") || tagValue == QLatin1StringView("value")) {
                if (element.readNextStartElement()) {
                    if (element.name() == QLatin1StringView("str")) {
                        auto relation = w->findChild<SelectRelationalMatchType *>(u"relation"_s);
                        relation->setCode(AutoCreateScriptUtil::tagValue(tagValue), element.readElementText(), name(), error);
                    }
                }
            } else if (tagValue == QLatin1StringView("comparator")) {
                if (element.readNextStartElement()) {
                    if (element.name() == QLatin1StringView("str")) {
                        auto comparator = w->findChild<SelectComparatorComboBox *>(u"comparator"_s);
                        comparator->setCode(element.readElementText(), name(), error);
                    }
                }
            } else {
                unknownTagValue(tagValue, error);
                qCDebug(LIBKSIEVEUI_LOG) << " SieveConditionVirusTest::setParamWidgetValue unknown tagValue " << tagValue;
            }
        } else if (tagName == QLatin1StringView("str")) {
            auto spinbox = w->findChild<QSpinBox *>(u"value"_s);
            spinbox->setValue(element.readElementText().toInt());
        } else if (tagName == QLatin1StringView("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1StringView("comment")) {
            commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, element.readElementText());
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVEUI_LOG) << " SieveConditionVirusTest::setParamWidgetValue unknown tagName " << tagName;
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

#include "moc_sieveconditionvirustest.cpp"
