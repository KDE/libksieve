/*
   SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveactionreplace.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "editor/sieveeditorutil.h"
#include "widgets/multilineedit.h"

#include <KLocalizedString>
#include <Libkdepim/LineEditCatchReturnKey>
#include <QLineEdit>

#include "libksieve_debug.h"
#include <QGridLayout>
#include <QLabel>
#include <QXmlStreamReader>

#include <KSieveUi/AbstractSelectEmailLineEdit>

using namespace KSieveUi;
SieveActionReplace::SieveActionReplace(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveAction(sieveGraphicalModeWidget, QStringLiteral("replace"), i18n("Replace"), parent)
{
}

QWidget *SieveActionReplace::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto grid = new QGridLayout;
    grid->setContentsMargins({});
    w->setLayout(grid);

    auto lab = new QLabel(i18n("Subject:"));
    grid->addWidget(lab, 0, 0);

    auto subject = new QLineEdit;
    new KPIM::LineEditCatchReturnKey(subject, w);
    subject->setObjectName(QStringLiteral("subject"));
    connect(subject, &QLineEdit::textChanged, this, &SieveActionReplace::valueChanged);
    grid->addWidget(subject, 0, 1);

    lab = new QLabel(i18n("from:"));
    grid->addWidget(lab, 1, 0);

    KSieveUi::AbstractSelectEmailLineEdit *headers = AutoCreateScriptUtil::createSelectEmailsWidget();
    headers->setObjectName(QStringLiteral("from"));
    connect(headers, &AbstractSelectEmailLineEdit::valueChanged, this, &SieveActionReplace::valueChanged);
    grid->addWidget(headers, 1, 1);

    lab = new QLabel(i18n("text:"));
    grid->addWidget(lab, 2, 0);

    auto text = new MultiLineEdit;
    text->setObjectName(QStringLiteral("text"));
    connect(text, &MultiLineEdit::textChanged, this, &SieveActionReplace::valueChanged);
    grid->addWidget(text, 2, 1);

    return w;
}

void SieveActionReplace::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, QString &error)
{
    while (element.readNextStartElement()) {
        const QStringRef tagName = element.name();
        if (tagName == QLatin1String("str")) {
            auto edit = w->findChild<MultiLineEdit *>(QStringLiteral("text"));
            edit->setPlainText(element.readElementText());
        } else if (tagName == QLatin1String("tag")) {
            const QString tagValue = element.readElementText();
            if (tagValue == QLatin1String("subject")) {
                const QString strValue = AutoCreateScriptUtil::strValue(element);
                if (!strValue.isEmpty()) {
                    auto subject = w->findChild<QLineEdit *>(QStringLiteral("subject"));
                    subject->setText(strValue);
                }
            } else if (tagValue == QLatin1String("from")) {
                const QString strValue = AutoCreateScriptUtil::strValue(element);
                if (!strValue.isEmpty()) {
                    auto headers = w->findChild<KSieveUi::AbstractSelectEmailLineEdit *>(QStringLiteral("from"));
                    headers->setText(strValue);
                }
            } else {
                unknownTagValue(tagValue, error);
                qCDebug(LIBKSIEVE_LOG) << " SieveActionReplace::setParamWidgetValue unknown tagValue " << tagValue;
            }
        } else if (tagName == QLatin1String("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1String("comment")) {
            element.skipCurrentElement();
            // implement in the future ?
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVE_LOG) << " SieveActionReplace::setParamWidgetValue unknown tagName " << tagName;
        }
    }
}

QString SieveActionReplace::code(QWidget *w) const
{
    QString result = QStringLiteral("replace ");
    const QLineEdit *subject = w->findChild<QLineEdit *>(QStringLiteral("subject"));
    const QString subjectStr = subject->text();
    if (!subjectStr.isEmpty()) {
        result += QStringLiteral(":subject \"%1\" ").arg(subjectStr);
    }

    const KSieveUi::AbstractSelectEmailLineEdit *headers = w->findChild<KSieveUi::AbstractSelectEmailLineEdit *>(QStringLiteral("from"));
    const QString headersStr = headers->text();
    if (!headersStr.isEmpty()) {
        result += QStringLiteral(":from \"%1\" ").arg(headersStr);
    }

    const MultiLineEdit *edit = w->findChild<MultiLineEdit *>(QStringLiteral("text"));
    const QString text = edit->toPlainText();
    if (!text.isEmpty()) {
        result += QStringLiteral("text:%1").arg(AutoCreateScriptUtil::createMultiLine(text));
    }

    return result;
}

QStringList SieveActionReplace::needRequires(QWidget *) const
{
    return QStringList() << QStringLiteral("replace");
}

bool SieveActionReplace::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveActionReplace::serverNeedsCapability() const
{
    return QStringLiteral("replace");
}

QString SieveActionReplace::help() const
{
    return i18n("The \"replace\" command is defined to allow a MIME part to be replaced with the text supplied in the command.");
}

QUrl SieveActionReplace::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}
