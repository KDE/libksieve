/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveactionaddheader.h"
#include "autocreatescripts/autocreatescriptutil_p.h"
#include "widgets/selectaddheaderpositioncombobox.h"
#include <KLineEditEventHandler>

#include <KLocalizedString>
#include <QLineEdit>

#include "libksieveui_debug.h"
#include <QGridLayout>
#include <QLabel>
#include <QUrl>
#include <QWidget>
#include <QXmlStreamReader>

using namespace KSieveUi;

SieveActionAddHeader::SieveActionAddHeader(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveActionAbstractEditHeader(sieveGraphicalModeWidget, QStringLiteral("addheader"), i18n("Add header"), parent)
{
}

QWidget *SieveActionAddHeader::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto grid = new QGridLayout;
    grid->setContentsMargins({});
    w->setLayout(grid);

    auto combo = new SelectAddHeaderPositionCombobox;
    combo->setObjectName(QLatin1StringView("selectposition"));
    connect(combo, &SelectAddHeaderPositionCombobox::valueChanged, this, &SieveActionAddHeader::valueChanged);
    grid->addWidget(combo, 0, 0);

    auto lab = new QLabel(i18nc("@label:textbox", "header:"));
    grid->addWidget(lab, 0, 1);

    auto headerEdit = new QLineEdit;
    KLineEditEventHandler::catchReturnKey(headerEdit);
    connect(headerEdit, &QLineEdit::textChanged, this, &SieveActionAddHeader::valueChanged);
    headerEdit->setObjectName(QLatin1StringView("headeredit"));
    grid->addWidget(headerEdit, 0, 2);

    lab = new QLabel(i18nc("@label:textbox", "value:"));
    grid->addWidget(lab, 1, 1);

    auto valueEdit = new QLineEdit;
    KLineEditEventHandler::catchReturnKey(valueEdit);
    connect(valueEdit, &QLineEdit::textChanged, this, &SieveActionAddHeader::valueChanged);
    valueEdit->setObjectName(QLatin1StringView("valueedit"));
    grid->addWidget(valueEdit, 1, 2);

    return w;
}

void SieveActionAddHeader::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, QString &error)
{
    int index = 0;
    while (element.readNextStartElement()) {
        const QStringView tagName = element.name();
        if (tagName == QLatin1StringView("tag")) {
            auto combo = w->findChild<SelectAddHeaderPositionCombobox *>(QStringLiteral("selectposition"));
            combo->setCode(AutoCreateScriptUtil::tagValue(element.readElementText()), name(), error);
        } else if (tagName == QLatin1StringView("str")) {
            if (index == 0) {
                auto edit = w->findChild<QLineEdit *>(QStringLiteral("headeredit"));
                edit->setText(element.readElementText());
            } else if (index == 1) {
                auto value = w->findChild<QLineEdit *>(QStringLiteral("valueedit"));
                value->setText(AutoCreateScriptUtil::quoteStr(element.readElementText()));
            } else {
                tooManyArguments(tagName, index, 2, error);
                qCDebug(LIBKSIEVEUI_LOG) << " SieveActionAddHeader::setParamWidgetValue too many argument :" << index;
            }
            ++index;
        } else if (tagName == QLatin1StringView("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1StringView("comment")) {
            element.skipCurrentElement();
            // implement in the future ?
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVEUI_LOG) << "SieveActionAddHeader::setParamWidgetValue unknown tag " << tagName;
        }
    }
}

QString SieveActionAddHeader::code(QWidget *w) const
{
    const SelectAddHeaderPositionCombobox *combo = w->findChild<SelectAddHeaderPositionCombobox *>(QStringLiteral("selectposition"));
    const QString position = combo->code();

    const QLineEdit *edit = w->findChild<QLineEdit *>(QStringLiteral("headeredit"));
    const QString headerStr = edit->text();

    const QLineEdit *value = w->findChild<QLineEdit *>(QStringLiteral("valueedit"));
    const QString valueStr = value->text();

    if (position.isEmpty()) {
        return QStringLiteral("addheader \"%1\" \"%2\";").arg(headerStr, valueStr);
    } else {
        return QStringLiteral("addheader %1 \"%2\" \"%3\";").arg(position, headerStr, valueStr);
    }
}

QString SieveActionAddHeader::help() const
{
    return i18n("The addheader action adds a header field to the existing message header.");
}

QUrl SieveActionAddHeader::href() const
{
    return QUrl(QStringLiteral("https://tools.ietf.org/html/rfc5293"));
}

#include "moc_sieveactionaddheader.cpp"
