/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveactionnotify.h"
#include "editor/sieveeditorutil.h"
#include "widgets/selectimportancecombobox.h"
#include "autocreatescripts/autocreatescriptutil_p.h"

#include <KLocalizedString>
#include <QLineEdit>

#include <QHBoxLayout>
#include <QLabel>
#include <QXmlStreamReader>
#include "libksieve_debug.h"

using namespace KSieveUi;

SieveActionNotify::SieveActionNotify(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveAction(sieveGraphicalModeWidget, QStringLiteral("notify"), i18n("Notify"), parent)
{
}

QWidget *SieveActionNotify::createParamWidget(QWidget *parent) const
{
    QWidget *w = new QWidget(parent);
    auto lay = new QHBoxLayout;
    lay->setContentsMargins({});
    w->setLayout(lay);

    auto importanceCombobox = new SelectImportanceCombobox;
    importanceCombobox->setObjectName(QStringLiteral("importancecombo"));
    connect(importanceCombobox, &SelectImportanceCombobox::valueChanged, this, &SieveActionNotify::valueChanged);
    lay->addWidget(importanceCombobox);

    QLabel *lab = new QLabel(i18n("message:"));
    lay->addWidget(lab);

    auto message = new QLineEdit;
    message->setObjectName(QStringLiteral("message"));
    connect(message, &QLineEdit::textChanged, this, &SieveActionNotify::valueChanged);
    lay->addWidget(message);

    lab = new QLabel(i18n("method:"));
    lay->addWidget(lab);

    auto method = new QLineEdit;
    method->setObjectName(QStringLiteral("method"));
    lay->addWidget(method);
    connect(method, &QLineEdit::textChanged, this, &SieveActionNotify::valueChanged);

    return w;
}

void SieveActionNotify::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, QString &error)
{
    while (element.readNextStartElement()) {
        const QStringRef tagName = element.name();
        if (tagName == QLatin1String("tag")) {
            const QString tagValue = element.readElementText();
            if (tagValue == QLatin1String("message")) {
                const QString strValue = AutoCreateScriptUtil::strValue(element);
                if (!strValue.isEmpty()) {
                    auto message = w->findChild<QLineEdit *>(QStringLiteral("message"));
                    message->setText(AutoCreateScriptUtil::quoteStr(strValue));
                }
            } else if (tagValue == QLatin1String("importance")) {
                const QString strValue = AutoCreateScriptUtil::strValue(element);
                if (!strValue.isEmpty()) {
                    auto importance = w->findChild<SelectImportanceCombobox *>(QStringLiteral("importancecombo"));
                    importance->setCode(strValue, name(), error);
                }
            } else {
                unknownTagValue(tagValue, error);
                qCDebug(LIBKSIEVE_LOG) << " SieveActionNotify::setParamWidgetValue unknown tagValue" << tagValue;
            }
        } else if (tagName == QLatin1String("crlf")) {
            element.skipCurrentElement();
            //nothing
        } else if (tagName == QLatin1String("comment")) {
            element.skipCurrentElement();
            //implement in the future ?
        } else if (tagName == QLatin1String("str")) {
            auto method = w->findChild<QLineEdit *>(QStringLiteral("method"));
            method->setText(AutoCreateScriptUtil::quoteStr(element.readElementText()));
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVE_LOG) << " SieveActionNotify::setParamWidgetValue unknown tagName " << tagName;
        }
    }
}

QString SieveActionNotify::code(QWidget *w) const
{
    QString result = QStringLiteral("notify");
    const SelectImportanceCombobox *importance = w->findChild<SelectImportanceCombobox *>(QStringLiteral("importancecombo"));
    const QString importanceStr = importance->code();
    if (!importanceStr.isEmpty()) {
        result += QStringLiteral(" :importance \"%1\"").arg(importanceStr);
    }

    const QLineEdit *message = w->findChild<QLineEdit *>(QStringLiteral("message"));
    const QString messageStr = message->text();
    if (!messageStr.isEmpty()) {
        result += QStringLiteral(" :message \"%1\"").arg(messageStr);
    }

    const QLineEdit *method = w->findChild<QLineEdit *>(QStringLiteral("method"));
    const QString methodStr = method->text();
    result += QStringLiteral(" \"%1\";").arg(methodStr);

    return result;
}

QString SieveActionNotify::serverNeedsCapability() const
{
    return QStringLiteral("enotify");
}

bool SieveActionNotify::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveActionNotify::help() const
{
    return i18n("The \"notify\" action specifies that a notification should be sent to a user.");
}

QStringList SieveActionNotify::needRequires(QWidget *) const
{
    QStringList lst;
    lst << QStringLiteral("enotify");
    return lst;
}

QUrl SieveActionNotify::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}
