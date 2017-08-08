/*
   Copyright (C) 2013-2017 Laurent Montel <montel@kde.org>

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
#include "sieveconditionspamtest.h"
#include "widgets/selectrelationalmatchtype.h"
#include "widgets/selectcomparatorcombobox.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "autocreatescripts/sieveeditorgraphicalmodewidget.h"
#include "editor/sieveeditorutil.h"

#include <KLocalizedString>

#include <QSpinBox>
#include "libksieve_debug.h"
#include <QXmlStreamReader>
#include <QCheckBox>
#include <QVBoxLayout>

using namespace KSieveUi;

SieveConditionSpamTest::SieveConditionSpamTest(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, QStringLiteral("spamtest"), i18n("Spam Test"), parent)
{
    mHasSpamTestPlusSupport = sieveCapabilities().contains(QStringLiteral("spamtestplus"));
}

QWidget *SieveConditionSpamTest::createParamWidget(QWidget *parent) const
{
    QWidget *w = new QWidget(parent);
    QVBoxLayout *lay = new QVBoxLayout;
    lay->setMargin(0);
    w->setLayout(lay);

    if (mHasSpamTestPlusSupport) {
        QCheckBox *percent = new QCheckBox(i18n("Percent"));
        percent->setObjectName(QStringLiteral("percent"));
        connect(percent, &QCheckBox::toggled, this, &SieveConditionSpamTest::valueChanged);
        lay->addWidget(percent);
    }

    SelectRelationalMatchType *relation = new SelectRelationalMatchType;
    connect(relation, &SelectRelationalMatchType::valueChanged, this, &SieveConditionSpamTest::valueChanged);
    relation->setObjectName(QStringLiteral("relation"));
    lay->addWidget(relation);

    SelectComparatorComboBox *comparator = new SelectComparatorComboBox(mSieveGraphicalModeWidget);
    connect(comparator, &SelectComparatorComboBox::valueChanged, this, &SieveConditionSpamTest::valueChanged);
    comparator->setObjectName(QStringLiteral("comparator"));
    lay->addWidget(comparator);

    QSpinBox *spinbox = new QSpinBox;
    spinbox->setMaximum(10);
    spinbox->setMinimum(0);
    spinbox->setObjectName(QStringLiteral("value"));
    connect(spinbox, QOverload<int>::of(&QSpinBox::valueChanged), this, &SieveConditionSpamTest::valueChanged);
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

    return QStringLiteral("spamtest %1 %2 %3 \"%4\"").arg(percentStr, relationStr, comparatorStr, value) + AutoCreateScriptUtil::generateConditionComment(comment());
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
    return i18n("Sieve implementations that implement the \"spamtest\" test use an identifier of either \"spamtest\" or \"spamtestplus\" for use with the capability mechanism.");
}

bool SieveConditionSpamTest::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, bool /*notCondition*/, QString &error)
{
    QString commentStr;
    while (element.readNextStartElement()) {
        const QStringRef tagName = element.name();
        if (tagName == QLatin1String("tag")) {
            const QString tagValue = element.readElementText();
            if (tagValue == QLatin1String("count") || tagValue == QLatin1String("value")) {
                if (element.readNextStartElement()) {
                    if (element.name() == QLatin1String("str")) {
                        SelectRelationalMatchType *relation = w->findChild<SelectRelationalMatchType *>(QStringLiteral("relation"));
                        relation->setCode(AutoCreateScriptUtil::tagValue(tagValue), element.readElementText(), name(), error);
                    }
                }
            } else if (tagValue == QLatin1String("comparator")) {
                if (element.readNextStartElement()) {
                    if (element.name() == QLatin1String("str")) {
                        SelectComparatorComboBox *comparator = w->findChild<SelectComparatorComboBox *>(QStringLiteral("comparator"));
                        comparator->setCode(element.readElementText(), name(), error);
                    }
                }
            } else if (tagValue == QLatin1String("percent")) {
                if (mHasSpamTestPlusSupport) {
                    QCheckBox *checkbox = w->findChild<QCheckBox *>(QStringLiteral("percent"));
                    checkbox->setChecked(true);
                } else {
                    serverDoesNotSupportFeatures(QStringLiteral("percent"), error);
                    qCDebug(LIBKSIEVE_LOG) << " SieveConditionSpamTest::setParamWidgetValue server has not percent support";
                }
            } else {
                unknowTagValue(tagValue, error);
                qCDebug(LIBKSIEVE_LOG) << " SieveConditionSpamTest::setParamWidgetValue unknown tagvalue " << tagValue;
            }
        } else if (tagName == QLatin1String("str")) {
            QSpinBox *spinbox = w->findChild<QSpinBox *>(QStringLiteral("value"));
            spinbox->setValue(element.readElementText().toInt());
        } else if (tagName == QLatin1String("crlf")) {
            element.skipCurrentElement();
            //nothing
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
    return true;
}

QUrl SieveConditionSpamTest::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}
