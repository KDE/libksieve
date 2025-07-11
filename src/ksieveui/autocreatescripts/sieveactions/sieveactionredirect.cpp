/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveactionredirect.h"
using namespace Qt::Literals::StringLiterals;

#include "autocreatescripts/autocreatescriptutil_p.h"
#include "autocreatescripts/sieveeditorgraphicalmodewidget.h"
#include "editor/sieveeditorutil.h"
#include "widgets/addresslineedit.h"

#include <KLocalizedString>

#include "libksieveui_debug.h"
#include <QCheckBox>
#include <QHBoxLayout>
#include <QXmlStreamReader>

using namespace KSieveUi;

SieveActionRedirect::SieveActionRedirect(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveAction(sieveGraphicalModeWidget, u"redirect"_s, i18n("Redirect"), parent)
{
    mHasCopySupport = sieveCapabilities().contains(QLatin1StringView("copy"));
    mHasListSupport = sieveCapabilities().contains(QLatin1StringView("extlists"));
}

QWidget *SieveActionRedirect::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto lay = new QHBoxLayout;
    lay->setContentsMargins({});
    w->setLayout(lay);
    if (mHasCopySupport) {
        auto copy = new QCheckBox(i18nc("@option:check", "Keep a copy"));
        copy->setObjectName(QLatin1StringView("copy"));
        connect(copy, &QCheckBox::clicked, this, &SieveActionRedirect::valueChanged);
        lay->addWidget(copy);
    }
    if (mHasListSupport) {
        auto list = new QCheckBox(i18nc("@option:check", "Use list"));
        list->setObjectName(QLatin1StringView("list"));
        connect(list, &QCheckBox::clicked, this, &SieveActionRedirect::valueChanged);
        lay->addWidget(list);
    }

    KSieveUi::AbstractSelectEmailLineEdit *edit = AutoCreateScriptUtil::createSelectEmailsWidget();
    edit->setObjectName(QLatin1StringView("RedirectEdit"));
    connect(edit, &AddressLineEdit::valueChanged, this, &SieveActionRedirect::valueChanged);
    lay->addWidget(edit);
    return w;
}

void SieveActionRedirect::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, QString &error)
{
    while (element.readNextStartElement()) {
        const QStringView tagName = element.name();
        if (tagName == QLatin1StringView("str")) {
            auto edit = w->findChild<AbstractSelectEmailLineEdit *>(u"RedirectEdit"_s);
            const QString tagValue = element.readElementText();
            edit->setText(AutoCreateScriptUtil::quoteStr(tagValue));
        } else if (tagName == QLatin1StringView("tag")) {
            const QString tagValue = element.readElementText();
            if (tagValue == QLatin1StringView("copy")) {
                if (mHasCopySupport) {
                    auto copy = w->findChild<QCheckBox *>(u"copy"_s);
                    copy->setChecked(true);
                } else {
                    serverDoesNotSupportFeatures(u"copy"_s, error);
                }
            } else if (tagValue == QLatin1StringView("list")) {
                if (mHasListSupport) {
                    auto list = w->findChild<QCheckBox *>(u"list"_s);
                    list->setChecked(true);
                } else {
                    serverDoesNotSupportFeatures(u"list"_s, error);
                }
            } else {
                unknownTagValue(tagValue, error);
                qCDebug(LIBKSIEVEUI_LOG) << " SieveActionRedirect::setParamWidgetValue tagValue unknown" << tagValue;
            }
        } else if (tagName == QLatin1StringView("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1StringView("comment")) {
            setComment(element.readElementText());
            // implement in the future ?
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVEUI_LOG) << " SieveActionRedirect::setParamWidgetValue unknown tagName " << tagName;
        }
    }
}

QString SieveActionRedirect::code(QWidget *w) const
{
    QString result = u"redirect "_s;
    const AbstractSelectEmailLineEdit *edit = w->findChild<AbstractSelectEmailLineEdit *>(u"RedirectEdit"_s);
    const QString text = edit->text();

    if (mHasCopySupport) {
        const QCheckBox *copy = w->findChild<QCheckBox *>(u"copy"_s);
        if (copy->isChecked()) {
            result += QLatin1StringView(":copy ");
        }
    }

    if (mHasListSupport) {
        const QCheckBox *list = w->findChild<QCheckBox *>(u"list"_s);
        if (list->isChecked()) {
            result += QLatin1StringView(":list ");
        }
    }

    return result + u"\"%1\";"_s.arg(text);
}

QStringList SieveActionRedirect::needRequires(QWidget *parent) const
{
    QStringList lst;
    if (mHasCopySupport) {
        const QCheckBox *copy = parent->findChild<QCheckBox *>(u"copy"_s);
        if (copy->isChecked()) {
            lst << u"copy"_s;
        }
    }
    if (mHasListSupport) {
        const QCheckBox *list = parent->findChild<QCheckBox *>(u"list"_s);
        if (list->isChecked()) {
            lst << u"extlists"_s;
        }
    }
    return lst;
}

QString SieveActionRedirect::help() const
{
    QString helpStr = i18n(
        "The \"redirect\" action is used to send the message to another user at a supplied address, as a mail forwarding feature does.  The \"redirect\" "
        "action makes no changes to the message body or existing headers, but it may add new headers.");
    if (mHasCopySupport) {
        helpStr += u'\n'
            + i18n("If the optional \":copy\" keyword is specified, the tagged command does not cancel the implicit \"keep\". Instead, it redirects a copy in "
                   "addition to whatever else is happening to the message.");
    }
    // TODO add list info
    return helpStr;
}

QUrl SieveActionRedirect::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

#include "moc_sieveactionredirect.cpp"
