/*
   SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveconditionsize.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "editor/sieveeditorutil.h"
#include "widgets/selectsizewidget.h"
#include <KLocalizedString>

#include "libksieveui_debug.h"
#include <QComboBox>
#include <QHBoxLayout>
#include <QXmlStreamReader>

using namespace KSieveUi;

SieveConditionSize::SieveConditionSize(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveCondition(sieveGraphicalModeWidget, QStringLiteral("size"), i18n("Size"), parent)
{
}

QWidget *SieveConditionSize::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto lay = new QHBoxLayout;
    lay->setContentsMargins({});
    w->setLayout(lay);

    auto combo = new QComboBox;
    combo->setObjectName(QLatin1StringView("combosize"));
    combo->addItem(i18n("under"), QStringLiteral(":under"));
    combo->addItem(i18n("over"), QStringLiteral(":over"));
    lay->addWidget(combo);
    connect(combo, &QComboBox::activated, this, &SieveConditionSize::valueChanged);

    auto sizeWidget = new SelectSizeWidget;
    connect(sizeWidget, &SelectSizeWidget::valueChanged, this, &SieveConditionSize::valueChanged);
    sizeWidget->setObjectName(QLatin1StringView("sizewidget"));
    lay->addWidget(sizeWidget);

    return w;
}

QString SieveConditionSize::code(QWidget *w) const
{
    const QComboBox *combo = w->findChild<QComboBox *>(QStringLiteral("combosize"));
    const QString comparison = combo->itemData(combo->currentIndex()).toString();
    const SelectSizeWidget *sizeWidget = w->findChild<SelectSizeWidget *>(QStringLiteral("sizewidget"));
    return QStringLiteral("size %1 %2").arg(comparison, sizeWidget->code()) + AutoCreateScriptUtil::generateConditionComment(comment());
}

QString SieveConditionSize::help() const
{
    return i18n(
        "The \"size\" test deals with the size of a message.  It takes either a tagged argument of \":over\" or \":under\", followed by a number representing "
        "the size of the message.");
}

void SieveConditionSize::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, bool /*notCondition*/, QString &error)
{
    QString commentStr;
    while (element.readNextStartElement()) {
        const QStringView tagName = element.name();
        if (tagName == QLatin1StringView("tag")) {
            const QString tagValue = element.readElementText();
            auto combo = w->findChild<QComboBox *>(QStringLiteral("combosize"));
            const int index = combo->findData(AutoCreateScriptUtil::tagValue(tagValue));
            if (index != -1) {
                combo->setCurrentIndex(index);
            }
        } else if (tagName == QLatin1StringView("num")) {
            QString numIdentifier;
            if (element.attributes().hasAttribute(QLatin1StringView("quantifier"))) {
                numIdentifier = element.attributes().value(QLatin1StringView("quantifier")).toString();
            }
            const qlonglong tagValue = element.readElementText().toLongLong();
            auto sizeWidget = w->findChild<SelectSizeWidget *>(QStringLiteral("sizewidget"));
            sizeWidget->setCode(tagValue, numIdentifier, name(), error);
        } else if (tagName == QLatin1StringView("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1StringView("comment")) {
            commentStr = AutoCreateScriptUtil::loadConditionComment(commentStr, element.readElementText());
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVEUI_LOG) << " SieveConditionSize::setParamWidgetValue unknown tagName " << tagName;
        }
    }
    if (!commentStr.isEmpty()) {
        setComment(commentStr);
    }
}

QUrl SieveConditionSize::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

#include "moc_sieveconditionsize.cpp"
