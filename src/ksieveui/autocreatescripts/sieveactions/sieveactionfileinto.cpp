/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveactionfileinto.h"
using namespace Qt::Literals::StringLiterals;

#include "autocreatescripts/autocreatescriptutil_p.h"
#include "autocreatescripts/sieveeditorgraphicalmodewidget.h"
#include "editor/sieveeditorutil.h"
#include "widgets/moveimapfolderwidget.h"
#include <KLocalizedString>

#include "libksieveui_debug.h"
#include <QCheckBox>
#include <QHBoxLayout>
#include <QXmlStreamReader>
// Add support for adding flags
using namespace KSieveUi;
SieveActionFileInto::SieveActionFileInto(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveAction(sieveGraphicalModeWidget, u"fileinto"_s, i18n("File Into"), parent)
{
    mHasCopySupport = sieveCapabilities().contains(QLatin1StringView("copy"));
    mHasMailBoxSupport = sieveCapabilities().contains(QLatin1StringView("mailbox"));
}

QString SieveActionFileInto::code(QWidget *w) const
{
    QString result = u"fileinto "_s;
    const KSieveUi::AbstractMoveImapFolderWidget *edit = w->findChild<KSieveUi::AbstractMoveImapFolderWidget *>(u"fileintolineedit"_s);
    const QString text = edit->text();
    if (mHasCopySupport) {
        const QCheckBox *copy = w->findChild<QCheckBox *>(u"copy"_s);
        if (copy->isChecked()) {
            result += QLatin1StringView(":copy ");
        }
    }
    if (mHasMailBoxSupport) {
        const QCheckBox *create = w->findChild<QCheckBox *>(u"create"_s);
        if (create->isChecked()) {
            result += QLatin1StringView(":create ");
        }
    }
    return result + u"\"%1\";"_s.arg(text);
}

void SieveActionFileInto::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, QString &error)
{
    while (element.readNextStartElement()) {
        const QStringView tagName = element.name();
        if (tagName == QLatin1StringView("tag")) {
            const QString tagValue = element.readElementText();
            if (tagValue == QLatin1StringView("copy")) {
                if (mHasCopySupport) {
                    auto copy = w->findChild<QCheckBox *>(u"copy"_s);
                    copy->setChecked(true);
                } else {
                    error += i18n("Action \"fileinto\" has \"copy\" argument but current server does not support it") + u'\n';
                    qCDebug(LIBKSIEVEUI_LOG) << "SieveActionFileInto::setParamWidgetValue has not copy support ";
                }
            } else if (tagValue == QLatin1StringView("create")) {
                if (mHasMailBoxSupport) {
                    auto create = w->findChild<QCheckBox *>(u"create"_s);
                    create->setChecked(true);
                } else {
                    serverDoesNotSupportFeatures(u"create"_s, error);
                    qCDebug(LIBKSIEVEUI_LOG) << "SieveActionFileInto::setParamWidgetValue server has not create support ";
                }
            } else {
                serverDoesNotSupportFeatures(tagValue, error);
                qCDebug(LIBKSIEVEUI_LOG) << "SieveActionFileInto::setParamWidgetValue server has not flags support ";
            }
        } else if (tagName == QLatin1StringView("str")) {
            const QString tagValue = element.readElementText();
            auto edit = w->findChild<KSieveUi::AbstractMoveImapFolderWidget *>(u"fileintolineedit"_s);
            edit->setText(AutoCreateScriptUtil::protectSlash(tagValue));
        } else if (tagName == QLatin1StringView("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1StringView("comment")) {
            element.skipCurrentElement();
            // implement in the future ?
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVEUI_LOG) << " SieveActionFileInto::setParamWidgetValue unknown tagName " << tagName;
        }
    }
}

QWidget *SieveActionFileInto::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto lay = new QHBoxLayout(w);
    lay->setContentsMargins({});

    if (mHasCopySupport) {
        auto copy = new QCheckBox(i18nc("@option:check", "Keep a copy"));
        copy->setObjectName(QLatin1StringView("copy"));
        lay->addWidget(copy);
        connect(copy, &QCheckBox::clicked, this, &SieveActionFileInto::valueChanged);
    }
    if (mHasMailBoxSupport) {
        auto create = new QCheckBox(i18nc("@option:check", "Create folder"));
        create->setObjectName(QLatin1StringView("create"));
        connect(create, &QCheckBox::clicked, this, &SieveActionFileInto::valueChanged);
        lay->addWidget(create);
    }

    KSieveUi::AbstractMoveImapFolderWidget *edit = AutoCreateScriptUtil::createImapFolderWidget();
    edit->setSieveImapAccountSettings(sieveImapAccountSettings());
    connect(edit, &KSieveUi::AbstractMoveImapFolderWidget::textChanged, this, &SieveActionFileInto::valueChanged);
    lay->addWidget(edit);
    edit->setObjectName(QLatin1StringView("fileintolineedit"));
    return w;
}

QStringList SieveActionFileInto::needRequires(QWidget *parent) const
{
    QStringList lst;
    lst << u"fileinto"_s;
    if (mHasCopySupport) {
        const QCheckBox *copy = parent->findChild<QCheckBox *>(u"copy"_s);
        if (copy->isChecked()) {
            lst << u"copy"_s;
        }
    }
    if (mHasMailBoxSupport) {
        const QCheckBox *create = parent->findChild<QCheckBox *>(u"create"_s);
        if (create->isChecked()) {
            lst << u"mailbox"_s;
        }
    }
    return lst;
}

bool SieveActionFileInto::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveActionFileInto::serverNeedsCapability() const
{
    return u"fileinto"_s;
}

QString SieveActionFileInto::help() const
{
    QString helpStr = i18n("The \"fileinto\" action delivers the message into the specified mailbox.");
    if (mHasMailBoxSupport) {
        helpStr += u'\n'
            + i18n("If the optional \":create\" argument is specified, it instructs the Sieve interpreter to create the specified mailbox, if needed, before "
                   "attempting to deliver the message into the specified mailbox.");
    }
    if (mHasCopySupport) {
        helpStr += u'\n'
            + i18n("If the optional \":copy\" keyword is specified, the tagged command does not cancel the implicit \"keep\". Instead, it merely files or "
                   "redirects a copy in addition to whatever else is happening to the message.");
    }
    return helpStr;
}

QUrl KSieveUi::SieveActionFileInto::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

#include "moc_sieveactionfileinto.cpp"
