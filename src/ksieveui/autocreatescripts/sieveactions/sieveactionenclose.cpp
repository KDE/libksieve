/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveactionenclose.h"
using namespace Qt::Literals::StringLiterals;

#include "autocreatescripts/autocreatescriptutil_p.h"
#include "editor/sieveeditorutil.h"
#include "widgets/multilineedit.h"

#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QLineEdit>

#include "libksieveui_debug.h"
#include <QGridLayout>
#include <QLabel>
#include <QXmlStreamReader>

using namespace KSieveUi;
SieveActionEnclose::SieveActionEnclose(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveAction(sieveGraphicalModeWidget, u"enclose"_s, i18n("Enclose"), parent)
{
}

QWidget *SieveActionEnclose::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto grid = new QGridLayout;
    grid->setContentsMargins({});
    w->setLayout(grid);

    auto lab = new QLabel(i18nc("@label:textbox", "Subject:"));
    grid->addWidget(lab, 0, 0);

    auto subject = new QLineEdit;
    KLineEditEventHandler::catchReturnKey(subject);
    subject->setObjectName(QLatin1StringView("subject"));
    connect(subject, &QLineEdit::textChanged, this, &SieveActionEnclose::valueChanged);
    grid->addWidget(subject, 0, 1);

    lab = new QLabel(i18nc("@label:textbox", "headers:"));
    grid->addWidget(lab, 1, 0);

    auto headers = new QLineEdit;
    KLineEditEventHandler::catchReturnKey(headers);
    headers->setObjectName(QLatin1StringView("headers"));
    connect(headers, &QLineEdit::textChanged, this, &SieveActionEnclose::valueChanged);
    grid->addWidget(headers, 1, 1);

    lab = new QLabel(i18nc("@label:textbox", "text:"));
    grid->addWidget(lab, 2, 0);

    auto text = new MultiLineEdit;
    text->setObjectName(QLatin1StringView("text"));
    connect(text, &MultiLineEdit::valueChanged, this, &SieveActionEnclose::valueChanged);
    grid->addWidget(text, 2, 1);

    return w;
}

void SieveActionEnclose::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, QString &error)
{
    while (element.readNextStartElement()) {
        const QStringView tagName = element.name();
        if (tagName == QLatin1StringView("tag")) {
            const QString tagValue = element.readElementText();
            if (tagValue == QLatin1StringView("headers")) {
                const QString strValue = AutoCreateScriptUtil::strValue(element);
                if (!strValue.isEmpty()) {
                    auto subject = w->findChild<QLineEdit *>(u"headers"_s);
                    subject->setText(strValue);
                }
            } else if (tagValue == QLatin1StringView("subject")) {
                const QString strValue = AutoCreateScriptUtil::strValue(element);
                if (!strValue.isEmpty()) {
                    auto headers = w->findChild<QLineEdit *>(u"subject"_s);
                    headers->setText(strValue);
                }
            } else {
                unknownTagValue(tagValue, error);
                qCDebug(LIBKSIEVEUI_LOG) << " SieveActionEnclose::setParamWidgetValue unknown tag value:" << tagValue;
            }
        } else if (tagName == QLatin1StringView("str")) {
            auto edit = w->findChild<MultiLineEdit *>(u"text"_s);
            edit->setPlainText(element.readElementText());
        } else if (tagName == QLatin1StringView("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1StringView("comment")) {
            element.skipCurrentElement();
            // implement in the future ?
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVEUI_LOG) << " SieveActionEnclose::setParamWidgetValue unknown tagName " << tagName;
        }
    }
}

QString SieveActionEnclose::code(QWidget *w) const
{
    QString result = u"enclose "_s;
    const QLineEdit *subject = w->findChild<QLineEdit *>(u"subject"_s);
    const QString subjectStr = subject->text();
    if (!subjectStr.isEmpty()) {
        result += u":subject \"%1\" "_s.arg(subjectStr);
    }

    const QLineEdit *headers = w->findChild<QLineEdit *>(u"headers"_s);
    const QString headersStr = headers->text();
    if (!headersStr.isEmpty()) {
        result += u":headers \"%1\" "_s.arg(headersStr);
    }

    const MultiLineEdit *edit = w->findChild<MultiLineEdit *>(u"text"_s);
    const QString text = edit->toPlainText();
    if (!text.isEmpty()) {
        result += u"text:%1"_s.arg(AutoCreateScriptUtil::createMultiLine(text));
    } else {
        result += u';';
    }

    return result;
}

QStringList SieveActionEnclose::needRequires(QWidget * /*parent*/) const
{
    return QStringList() << u"enclose"_s;
}

bool SieveActionEnclose::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveActionEnclose::serverNeedsCapability() const
{
    return u"enclose"_s;
}

QString SieveActionEnclose::help() const
{
    return i18n("Enclose action command is defined to allow an entire message to be enclosed as an attachment to a new message.");
}

QUrl SieveActionEnclose::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

#include "moc_sieveactionenclose.cpp"
