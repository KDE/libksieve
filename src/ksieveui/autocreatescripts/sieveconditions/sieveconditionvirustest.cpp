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
#include "sieveconditionvirustest.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "editor/sieveeditorutil.h"
#include "widgets/selectrelationalmatchtype.h"
#include "widgets/selectcomparatorcombobox.h"

#include <KLocalizedString>

#include <QSpinBox>
#include "libksieve_debug.h"
#include <QXmlStreamReader>
#include <QVBoxLayout>

using namespace KSieveUi;

SieveConditionVirusTest::SieveConditionVirusTest(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, QStringLiteral("virustest"), i18n("Virus Test"), parent)
{
}

QWidget *SieveConditionVirusTest::createParamWidget(QWidget *parent) const
{
    QWidget *w = new QWidget(parent);
    QVBoxLayout *lay = new QVBoxLayout;
    lay->setMargin(0);
    w->setLayout(lay);

    SelectRelationalMatchType *relation = new SelectRelationalMatchType;
    connect(relation, &SelectRelationalMatchType::valueChanged, this, &SieveConditionVirusTest::valueChanged);
    relation->setObjectName(QStringLiteral("relation"));
    lay->addWidget(relation);

    SelectComparatorComboBox *comparator = new SelectComparatorComboBox(mSieveGraphicalModeWidget);
    connect(comparator, &SelectComparatorComboBox::valueChanged, this, &SieveConditionVirusTest::valueChanged);
    comparator->setObjectName(QStringLiteral("comparator"));
    lay->addWidget(comparator);

    QSpinBox *spinbox = new QSpinBox;
    spinbox->setMaximum(5);
    spinbox->setMinimum(0);
    spinbox->setObjectName(QStringLiteral("value"));
    connect(spinbox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &SieveConditionVirusTest::valueChanged);
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

bool SieveConditionVirusTest::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, bool /*notCondition*/, QString &error)
{
    QString commentStr;
    while (element.readNextStartElement()) {
        const QStringRef tagName = element.name();

        if (tagName == QLatin1String("tag")) {
            const QString tagValue = element.readElementText();
#ifdef QDOMELEMENT_FIXME
            if (tagValue == QLatin1String("count") || tagValue == QLatin1String("value")) {
                node = node.nextSibling();
                if (!node.isNull()) {
                    QDomElement relationalElement = node.toElement();
                    if (!relationalElement.isNull()) {
                        if (relationalElement.tagName() == QLatin1String("str")) {
                            SelectRelationalMatchType *relation = w->findChild<SelectRelationalMatchType *>(QStringLiteral("relation"));
                            relation->setCode(AutoCreateScriptUtil::tagValue(tagValue), relationalElement.text(), name(), error);
                        }
                    }
                }
            } else if (tagValue == QLatin1String("comparator")) {
                node = node.nextSibling();
                if (!node.isNull()) {
                    QDomElement comparatorElement = node.toElement();
                    if (!comparatorElement.isNull()) {
                        if (comparatorElement.tagName() == QLatin1String("str")) {
                            SelectComparatorComboBox *comparator = w->findChild<SelectComparatorComboBox *>(QStringLiteral("comparator"));
                            comparator->setCode(comparatorElement.text(), name(), error);
                        }
                    }
                }
            } else {
                unknowTagValue(tagValue, error);
                qCDebug(LIBKSIEVE_LOG) << " SieveConditionVirusTest::setParamWidgetValue unknown tagValue " << tagValue;
            }
#endif
        } else if (tagName == QLatin1String("str")) {
            QSpinBox *spinbox = w->findChild<QSpinBox *>(QStringLiteral("value"));
            spinbox->setValue(element.readElementText().toInt());
        } else if (tagName == QLatin1String("crlf")) {
            //nothing
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

#ifdef REMOVE_QDOMELEMENT
    QDomNode node = element.firstChild();
    QString commentStr;
    while (!node.isNull()) {
        QDomElement e = node.toElement();
        if (!e.isNull()) {
            const QString tagName = e.tagName();
            if (tagName == QLatin1String("tag")) {
                const QString tagValue = e.text();
                if (tagValue == QLatin1String("count") || tagValue == QLatin1String("value")) {
                    node = node.nextSibling();
                    if (!node.isNull()) {
                        QDomElement relationalElement = node.toElement();
                        if (!relationalElement.isNull()) {
                            if (relationalElement.tagName() == QLatin1String("str")) {
                                SelectRelationalMatchType *relation = w->findChild<SelectRelationalMatchType *>(QStringLiteral("relation"));
                                relation->setCode(AutoCreateScriptUtil::tagValue(tagValue), relationalElement.text(), name(), error);
                            }
                        }
                    }
                } else if (tagValue == QLatin1String("comparator")) {
                    node = node.nextSibling();
                    if (!node.isNull()) {
                        QDomElement comparatorElement = node.toElement();
                        if (!comparatorElement.isNull()) {
                            if (comparatorElement.tagName() == QLatin1String("str")) {
                                SelectComparatorComboBox *comparator = w->findChild<SelectComparatorComboBox *>(QStringLiteral("comparator"));
                                comparator->setCode(comparatorElement.text(), name(), error);
                            }
                        }
                    }
                } else {
                    unknowTagValue(tagValue, error);
                    qCDebug(LIBKSIEVE_LOG) << " SieveConditionVirusTest::setParamWidgetValue unknown tagValue " << tagValue;
                }
            } else if (tagName == QLatin1String("str")) {
                QSpinBox *spinbox = w->findChild<QSpinBox *>(QStringLiteral("value"));
                spinbox->setValue(e.text().toInt());
            } else if (tagName == QLatin1String("crlf")) {
                //nothing
            } else if (tagName == QLatin1String("comment")) {
                commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, e.text());
            } else {
                unknownTag(tagName, error);
                qCDebug(LIBKSIEVE_LOG) << " SieveConditionVirusTest::setParamWidgetValue unknown tagName " << tagName;
            }
        }
        node = node.nextSibling();
    }
    if (!commentStr.isEmpty()) {
        setComment(commentStr);
    }
#endif
    return true;
}

QUrl SieveConditionVirusTest::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}
