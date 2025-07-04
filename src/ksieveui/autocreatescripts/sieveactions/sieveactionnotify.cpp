/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveactionnotify.h"
using namespace Qt::Literals::StringLiterals;

#include "autocreatescripts/autocreatescriptutil_p.h"
#include "editor/sieveeditorutil.h"
#include "widgets/selectimportancecombobox.h"

#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QLineEdit>

#include "libksieveui_debug.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QXmlStreamReader>

using namespace KSieveUi;

SieveActionNotify::SieveActionNotify(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveAction(sieveGraphicalModeWidget, u"notify"_s, i18n("Notify"), parent)
{
}

QWidget *SieveActionNotify::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto lay = new QHBoxLayout;
    lay->setContentsMargins({});
    w->setLayout(lay);

    auto importanceCombobox = new SelectImportanceCombobox;
    importanceCombobox->setObjectName(QLatin1StringView("importancecombo"));
    connect(importanceCombobox, &SelectImportanceCombobox::valueChanged, this, &SieveActionNotify::valueChanged);
    lay->addWidget(importanceCombobox);

    auto lab = new QLabel(i18nc("@label:textbox", "message:"));
    lay->addWidget(lab);

    auto message = new QLineEdit;
    KLineEditEventHandler::catchReturnKey(message);
    message->setObjectName(QLatin1StringView("message"));
    connect(message, &QLineEdit::textChanged, this, &SieveActionNotify::valueChanged);
    lay->addWidget(message);

    lab = new QLabel(i18nc("@label:textbox", "method:"));
    lay->addWidget(lab);

    auto method = new QLineEdit;
    KLineEditEventHandler::catchReturnKey(method);
    method->setObjectName(QLatin1StringView("method"));
    lay->addWidget(method);
    connect(method, &QLineEdit::textChanged, this, &SieveActionNotify::valueChanged);

    return w;
}

void SieveActionNotify::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, QString &error)
{
    while (element.readNextStartElement()) {
        const QStringView tagName = element.name();
        if (tagName == QLatin1StringView("tag")) {
            const QString tagValue = element.readElementText();
            if (tagValue == QLatin1StringView("message")) {
                const QString strValue = AutoCreateScriptUtil::strValue(element);
                if (!strValue.isEmpty()) {
                    auto message = w->findChild<QLineEdit *>(u"message"_s);
                    message->setText(AutoCreateScriptUtil::quoteStr(strValue));
                }
            } else if (tagValue == QLatin1StringView("importance")) {
                const QString strValue = AutoCreateScriptUtil::strValue(element);
                if (!strValue.isEmpty()) {
                    auto importance = w->findChild<SelectImportanceCombobox *>(u"importancecombo"_s);
                    importance->setCode(strValue, name(), error);
                }
            } else {
                unknownTagValue(tagValue, error);
                qCDebug(LIBKSIEVEUI_LOG) << " SieveActionNotify::setParamWidgetValue unknown tagValue" << tagValue;
            }
        } else if (tagName == QLatin1StringView("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1StringView("comment")) {
            element.skipCurrentElement();
            // implement in the future ?
        } else if (tagName == QLatin1StringView("str")) {
            auto method = w->findChild<QLineEdit *>(u"method"_s);
            method->setText(AutoCreateScriptUtil::quoteStr(element.readElementText()));
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVEUI_LOG) << " SieveActionNotify::setParamWidgetValue unknown tagName " << tagName;
        }
    }
}

QString SieveActionNotify::code(QWidget *w) const
{
    QString result = u"notify"_s;
    const SelectImportanceCombobox *importance = w->findChild<SelectImportanceCombobox *>(u"importancecombo"_s);
    const QString importanceStr = importance->code();
    if (!importanceStr.isEmpty()) {
        result += u" :importance \"%1\""_s.arg(importanceStr);
    }

    const QLineEdit *message = w->findChild<QLineEdit *>(u"message"_s);
    const QString messageStr = message->text();
    if (!messageStr.isEmpty()) {
        result += u" :message \"%1\""_s.arg(messageStr);
    }

    const QLineEdit *method = w->findChild<QLineEdit *>(u"method"_s);
    const QString methodStr = method->text();
    result += u" \"%1\";"_s.arg(methodStr);

    return result;
}

QString SieveActionNotify::serverNeedsCapability() const
{
    return u"enotify"_s;
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
    lst << u"enotify"_s;
    return lst;
}

QUrl SieveActionNotify::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

#include "moc_sieveactionnotify.cpp"
