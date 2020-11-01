/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveactionredirect.h"
#include "editor/sieveeditorutil.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "autocreatescripts/sieveeditorgraphicalmodewidget.h"
#include "widgets/addresslineedit.h"

#include <KLocalizedString>

#include <QHBoxLayout>
#include <QCheckBox>
#include <QXmlStreamReader>
#include "libksieve_debug.h"

using namespace KSieveUi;

SieveActionRedirect::SieveActionRedirect(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveAction(sieveGraphicalModeWidget, QStringLiteral("redirect"), i18n("Redirect"), parent)
{
    mHasCopySupport = sieveCapabilities().contains(QLatin1String("copy"));
    mHasListSupport = sieveCapabilities().contains(QLatin1String("extlists"));
}

QWidget *SieveActionRedirect::createParamWidget(QWidget *parent) const
{
    QWidget *w = new QWidget(parent);
    auto *lay = new QHBoxLayout;
    lay->setContentsMargins({});
    w->setLayout(lay);
    if (mHasCopySupport) {
        QCheckBox *copy = new QCheckBox(i18n("Keep a copy"));
        copy->setObjectName(QStringLiteral("copy"));
        connect(copy, &QCheckBox::clicked, this, &SieveActionRedirect::valueChanged);
        lay->addWidget(copy);
    }
    if (mHasListSupport) {
        QCheckBox *list = new QCheckBox(i18n("Use list"));
        list->setObjectName(QStringLiteral("list"));
        connect(list, &QCheckBox::clicked, this, &SieveActionRedirect::valueChanged);
        lay->addWidget(list);
    }

    KSieveUi::AbstractSelectEmailLineEdit *edit = AutoCreateScriptUtil::createSelectEmailsWidget();
    edit->setObjectName(QStringLiteral("RedirectEdit"));
    connect(edit, &AddressLineEdit::valueChanged, this, &SieveActionRedirect::valueChanged);
    lay->addWidget(edit);
    return w;
}

void SieveActionRedirect::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, QString &error)
{
    while (element.readNextStartElement()) {
        const QStringRef tagName = element.name();
        if (tagName == QLatin1String("str")) {
            auto *edit = w->findChild<AbstractSelectEmailLineEdit *>(QStringLiteral("RedirectEdit"));
            const QString tagValue = element.readElementText();
            edit->setText(AutoCreateScriptUtil::quoteStr(tagValue));
        } else if (tagName == QLatin1String("tag")) {
            const QString tagValue = element.readElementText();
            if (tagValue == QLatin1String("copy")) {
                if (mHasCopySupport) {
                    auto *copy = w->findChild<QCheckBox *>(QStringLiteral("copy"));
                    copy->setChecked(true);
                } else {
                    serverDoesNotSupportFeatures(QStringLiteral("copy"), error);
                }
            } else if (tagValue == QLatin1String("list")) {
                if (mHasListSupport) {
                    auto *list = w->findChild<QCheckBox *>(QStringLiteral("list"));
                    list->setChecked(true);
                } else {
                    serverDoesNotSupportFeatures(QStringLiteral("list"), error);
                }
            } else {
                unknownTagValue(tagValue, error);
                qCDebug(LIBKSIEVE_LOG) << " SieveActionRedirect::setParamWidgetValue tagValue unknown" << tagValue;
            }
        } else if (tagName == QLatin1String("crlf")) {
            element.skipCurrentElement();
            //nothing
        } else if (tagName == QLatin1String("comment")) {
            setComment(element.readElementText());
            //implement in the future ?
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVE_LOG) << " SieveActionRedirect::setParamWidgetValue unknown tagName " << tagName;
        }
    }
}

QString SieveActionRedirect::code(QWidget *w) const
{
    QString result = QStringLiteral("redirect ");
    const AbstractSelectEmailLineEdit *edit = w->findChild<AbstractSelectEmailLineEdit *>(QStringLiteral("RedirectEdit"));
    const QString text = edit->text();

    if (mHasCopySupport) {
        const QCheckBox *copy = w->findChild<QCheckBox *>(QStringLiteral("copy"));
        if (copy->isChecked()) {
            result += QLatin1String(":copy ");
        }
    }

    if (mHasListSupport) {
        const QCheckBox *list = w->findChild<QCheckBox *>(QStringLiteral("list"));
        if (list->isChecked()) {
            result += QLatin1String(":list ");
        }
    }

    return result + QStringLiteral("\"%1\";").arg(text);
}

QStringList SieveActionRedirect::needRequires(QWidget *parent) const
{
    QStringList lst;
    if (mHasCopySupport) {
        const QCheckBox *copy = parent->findChild<QCheckBox *>(QStringLiteral("copy"));
        if (copy->isChecked()) {
            lst << QStringLiteral("copy");
        }
    }
    if (mHasListSupport) {
        const QCheckBox *list = parent->findChild<QCheckBox *>(QStringLiteral("list"));
        if (list->isChecked()) {
            lst << QStringLiteral("extlists");
        }
    }
    return lst;
}

QString SieveActionRedirect::help() const
{
    QString helpStr = i18n(
        "The \"redirect\" action is used to send the message to another user at a supplied address, as a mail forwarding feature does.  The \"redirect\" action makes no changes to the message body or existing headers, but it may add new headers.");
    if (mHasCopySupport) {
        helpStr += QLatin1Char('\n') + i18n(
            "If the optional \":copy\" keyword is specified, the tagged command does not cancel the implicit \"keep\". Instead, it redirects a copy in addition to whatever else is happening to the message.");
    }
    //TODO add list info
    return helpStr;
}

QUrl SieveActionRedirect::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}
