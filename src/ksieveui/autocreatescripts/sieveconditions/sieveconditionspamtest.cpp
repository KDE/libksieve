/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveconditionspamtest.h"
using namespace Qt::Literals::StringLiterals;

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
    : SieveCondition(sieveGraphicalModeWidget, u"spamtest"_s, i18n("Spam Test"), parent)
{
    mHasSpamTestPlusSupport = sieveCapabilities().contains(QLatin1StringView("spamtestplus"));
}

QWidget *SieveConditionSpamTest::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto lay = new QVBoxLayout;
    lay->setContentsMargins({});
    w->setLayout(lay);

    if (mHasSpamTestPlusSupport) {
        auto percent = new QCheckBox(i18nc("@option:check", "Percent"));
        percent->setObjectName(QLatin1StringView("percent"));
        connect(percent, &QCheckBox::toggled, this, &SieveConditionSpamTest::valueChanged);
        lay->addWidget(percent);
    }

    auto relation = new SelectRelationalMatchType;
    connect(relation, &SelectRelationalMatchType::valueChanged, this, &SieveConditionSpamTest::valueChanged);
    relation->setObjectName(QLatin1StringView("relation"));
    lay->addWidget(relation);

    auto comparator = new SelectComparatorComboBox(mSieveGraphicalModeWidget);
    connect(comparator, &SelectComparatorComboBox::valueChanged, this, &SieveConditionSpamTest::valueChanged);
    comparator->setObjectName(QLatin1StringView("comparator"));
    lay->addWidget(comparator);

    auto spinbox = new QSpinBox;
    spinbox->setMaximum(10);
    spinbox->setMinimum(0);
    spinbox->setObjectName(QLatin1StringView("value"));
    connect(spinbox, &QSpinBox::valueChanged, this, &SieveConditionSpamTest::valueChanged);
    lay->addWidget(spinbox);
    return w;
}

QString SieveConditionSpamTest::code(QWidget *w) const
{
    QString percentStr;
    if (mHasSpamTestPlusSupport) {
        const QCheckBox *checkbox = w->findChild<QCheckBox *>(u"percent"_s);
        percentStr = checkbox->isChecked() ? u":percent"_s : QString();
    }

    const SelectRelationalMatchType *relation = w->findChild<SelectRelationalMatchType *>(u"relation"_s);
    const QString relationStr = relation->code();

    const SelectComparatorComboBox *comparator = w->findChild<SelectComparatorComboBox *>(u"comparator"_s);
    const QString comparatorStr = comparator->code();

    const QSpinBox *spinbox = w->findChild<QSpinBox *>(u"value"_s);
    const QString value = QString::number(spinbox->value());

    return u"spamtest %1 %2 %3 \"%4\""_s.arg(percentStr, relationStr, comparatorStr, value)
        + AutoCreateScriptUtil::generateConditionComment(comment());
}

bool SieveConditionSpamTest::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveConditionSpamTest::serverNeedsCapability() const
{
    return u"spamtest"_s;
}

QStringList SieveConditionSpamTest::needRequires(QWidget *w) const
{
    const SelectComparatorComboBox *comparator = w->findChild<SelectComparatorComboBox *>(u"comparator"_s);
    QStringList lst;
    lst << u"spamtest"_s << u"relational"_s;
    const QString comparatorRequires = comparator->require();
    if (!comparatorRequires.isEmpty()) {
        lst << comparatorRequires;
    }
    if (mHasSpamTestPlusSupport) {
        lst << u"spamtestplus"_s;
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
            } else if (tagValue == QLatin1StringView("percent")) {
                if (mHasSpamTestPlusSupport) {
                    auto checkbox = w->findChild<QCheckBox *>(u"percent"_s);
                    checkbox->setChecked(true);
                } else {
                    serverDoesNotSupportFeatures(u"percent"_s, error);
                    qCDebug(LIBKSIEVEUI_LOG) << " SieveConditionSpamTest::setParamWidgetValue server has not percent support";
                }
            } else {
                unknownTagValue(tagValue, error);
                qCDebug(LIBKSIEVEUI_LOG) << " SieveConditionSpamTest::setParamWidgetValue unknown tagvalue " << tagValue;
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
            qCDebug(LIBKSIEVEUI_LOG) << " SieveSpamTest::setParamWidgetValue unknown tagName " << tagName;
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
