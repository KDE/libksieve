/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveactionreplace.h"
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

#include <KSieveUi/AbstractSelectEmailLineEdit>

using namespace KSieveUi;
SieveActionReplace::SieveActionReplace(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveAction(sieveGraphicalModeWidget, u"replace"_s, i18n("Replace"), parent)
{
}

QWidget *SieveActionReplace::createParamWidget(QWidget *parent) const
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
    connect(subject, &QLineEdit::textChanged, this, &SieveActionReplace::valueChanged);
    grid->addWidget(subject, 0, 1);

    lab = new QLabel(i18nc("@label:textbox", "from:"));
    grid->addWidget(lab, 1, 0);

    KSieveUi::AbstractSelectEmailLineEdit *headers = AutoCreateScriptUtil::createSelectEmailsWidget();
    headers->setObjectName(QLatin1StringView("from"));
    connect(headers, &AbstractSelectEmailLineEdit::valueChanged, this, &SieveActionReplace::valueChanged);
    grid->addWidget(headers, 1, 1);

    lab = new QLabel(i18nc("@label:textbox", "text:"));
    grid->addWidget(lab, 2, 0);

    auto text = new MultiLineEdit;
    text->setObjectName(QLatin1StringView("text"));
    connect(text, &MultiLineEdit::textChanged, this, &SieveActionReplace::valueChanged);
    grid->addWidget(text, 2, 1);

    return w;
}

void SieveActionReplace::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, QString &error)
{
    while (element.readNextStartElement()) {
        const QStringView tagName = element.name();
        if (tagName == QLatin1StringView("str")) {
            auto edit = w->findChild<MultiLineEdit *>(u"text"_s);
            edit->setPlainText(element.readElementText());
        } else if (tagName == QLatin1StringView("tag")) {
            const QString tagValue = element.readElementText();
            if (tagValue == QLatin1StringView("subject")) {
                const QString strValue = AutoCreateScriptUtil::strValue(element);
                if (!strValue.isEmpty()) {
                    auto subject = w->findChild<QLineEdit *>(u"subject"_s);
                    subject->setText(strValue);
                }
            } else if (tagValue == QLatin1StringView("from")) {
                const QString strValue = AutoCreateScriptUtil::strValue(element);
                if (!strValue.isEmpty()) {
                    auto headers = w->findChild<KSieveUi::AbstractSelectEmailLineEdit *>(u"from"_s);
                    headers->setText(strValue);
                }
            } else {
                unknownTagValue(tagValue, error);
                qCDebug(LIBKSIEVEUI_LOG) << " SieveActionReplace::setParamWidgetValue unknown tagValue " << tagValue;
            }
        } else if (tagName == QLatin1StringView("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1StringView("comment")) {
            element.skipCurrentElement();
            // implement in the future ?
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVEUI_LOG) << " SieveActionReplace::setParamWidgetValue unknown tagName " << tagName;
        }
    }
}

QString SieveActionReplace::code(QWidget *w) const
{
    QString result = u"replace "_s;
    const QLineEdit *subject = w->findChild<QLineEdit *>(u"subject"_s);
    const QString subjectStr = subject->text();
    if (!subjectStr.isEmpty()) {
        result += u":subject \"%1\" "_s.arg(subjectStr);
    }

    const KSieveUi::AbstractSelectEmailLineEdit *headers = w->findChild<KSieveUi::AbstractSelectEmailLineEdit *>(u"from"_s);
    const QString headersStr = headers->text();
    if (!headersStr.isEmpty()) {
        result += u":from \"%1\" "_s.arg(headersStr);
    }

    const MultiLineEdit *edit = w->findChild<MultiLineEdit *>(u"text"_s);
    const QString text = edit->toPlainText();
    if (!text.isEmpty()) {
        result += u"text:%1"_s.arg(AutoCreateScriptUtil::createMultiLine(text));
    }

    return result;
}

QStringList SieveActionReplace::needRequires(QWidget *) const
{
    return QStringList() << u"replace"_s;
}

bool SieveActionReplace::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveActionReplace::serverNeedsCapability() const
{
    return u"replace"_s;
}

QString SieveActionReplace::help() const
{
    return i18n("The \"replace\" command is defined to allow a MIME part to be replaced with the text supplied in the command.");
}

QUrl SieveActionReplace::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

#include "moc_sieveactionreplace.cpp"
