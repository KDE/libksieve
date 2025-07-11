/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveactionvacation.h"
using namespace Qt::Literals::StringLiterals;

#include "autocreatescripts/autocreatescriptutil_p.h"
#include "autocreatescripts/sieveeditorgraphicalmodewidget.h"
#include "editor/sieveeditorutil.h"
#include "widgets/multilineedit.h"
#include "widgets/selectvacationcombobox.h"

#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QLineEdit>

#include "libksieveui_debug.h"
#include <KSieveUi/AbstractSelectEmailLineEdit>
#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>
#include <QXmlStreamReader>

using namespace KSieveUi;

SieveActionVacation::SieveActionVacation(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveAction(sieveGraphicalModeWidget, u"vacation"_s, i18n("Vacation"), parent)
{
    mHasVacationSecondsSupport = sieveCapabilities().contains(QLatin1StringView("vacation-seconds"));
}

QWidget *SieveActionVacation::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto grid = new QGridLayout;
    grid->setContentsMargins({});
    w->setLayout(grid);

    QLabel *lab = nullptr;
    if (mHasVacationSecondsSupport) {
        auto vacation = new SelectVacationComboBox;
        vacation->setObjectName(QLatin1StringView("vacationcombobox"));
        connect(vacation, &SelectVacationComboBox::valueChanged, this, &SieveActionVacation::valueChanged);
        grid->addWidget(vacation, 0, 0);
    } else {
        lab = new QLabel(i18nc("@label:textbox", "day:"));
        grid->addWidget(lab, 0, 0);
    }

    auto day = new QSpinBox;
    day->setMinimum(1);
    day->setMaximum(999);
    day->setObjectName(QLatin1StringView("day"));
    connect(day, &QSpinBox::valueChanged, this, &SieveActionVacation::valueChanged);
    grid->addWidget(day, 0, 1);

    lab = new QLabel(i18nc("@label:textbox", "Message subject:"));
    grid->addWidget(lab, 1, 0);

    auto subject = new QLineEdit;
    KLineEditEventHandler::catchReturnKey(subject);
    subject->setObjectName(QLatin1StringView("subject"));
    connect(subject, &QLineEdit::textChanged, this, &SieveActionVacation::valueChanged);
    grid->addWidget(subject, 1, 1);

    lab = new QLabel(i18nc("@label:textbox", "Additional email:"));
    grid->addWidget(lab, 2, 0);

    AbstractSelectEmailLineEdit *addresses = AutoCreateScriptUtil::createSelectEmailsWidget();
    addresses->setObjectName(QLatin1StringView("addresses"));
    addresses->setMultiSelection(true);
    connect(addresses, &AbstractSelectEmailLineEdit::valueChanged, this, &SieveActionVacation::valueChanged);
    grid->addWidget(addresses, 2, 1);

    lab = new QLabel(i18nc("@label:textbox", "Vacation reason:"));
    grid->addWidget(lab, 3, 0);

    auto text = new MultiLineEdit;
    connect(text, &MultiLineEdit::valueChanged, this, &SieveActionVacation::valueChanged);
    text->setObjectName(QLatin1StringView("text"));
    grid->addWidget(text, 3, 1);

    return w;
}

void SieveActionVacation::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, QString &error)
{
    while (element.readNextStartElement()) {
        const QStringView tagName = element.name();
        if (tagName == QLatin1StringView("tag")) {
            const QString tagValue = element.readElementText();
            if (tagValue == QLatin1StringView("seconds")) {
                if (mHasVacationSecondsSupport) {
                    auto vacationcombobox = w->findChild<SelectVacationComboBox *>(u"vacationcombobox"_s);
                    vacationcombobox->setCode(AutoCreateScriptUtil::tagValue(tagValue), name(), error);
                } else {
                    serverDoesNotSupportFeatures(u"seconds"_s, error);
                }
            } else if (tagValue == QLatin1StringView("days")) {
                // Nothing wait num tag for it.
            } else if (tagValue == QLatin1StringView("addresses")) {
                auto addresses = w->findChild<AbstractSelectEmailLineEdit *>(u"addresses"_s);
                if (element.readNextStartElement()) {
                    const QStringView textElementTagName = element.name();
                    if (textElementTagName == QLatin1StringView("str")) {
                        addresses->setText(element.readElementText());
                    } else if (textElementTagName == QLatin1StringView("list")) {
                        addresses->setText(AutoCreateScriptUtil::listValueToStr(element));
                    }
                }
            } else if (tagValue == QLatin1StringView("subject")) {
                auto subject = w->findChild<QLineEdit *>(u"subject"_s);
                subject->setText(AutoCreateScriptUtil::strValue(element));
            } else {
                unknownTagValue(tagValue, error);
                qCDebug(LIBKSIEVEUI_LOG) << "SieveActionVacation::setParamWidgetValue unknown tagValue :" << tagValue;
            }
        } else if (tagName == QLatin1StringView("num")) {
            auto day = w->findChild<QSpinBox *>(u"day"_s);
            day->setValue(element.readElementText().toInt());
        } else if (tagName == QLatin1StringView("str")) {
            auto text = w->findChild<MultiLineEdit *>(u"text"_s);
            text->setPlainText(element.readElementText());
        } else if (tagName == QLatin1StringView("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1StringView("comment")) {
            element.skipCurrentElement();
            // implement in the future ?
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVEUI_LOG) << " SieveActionVacation::setParamWidgetValue unknown tagName " << tagName;
        }
    }
}

QString SieveActionVacation::code(QWidget *w) const
{
    QString vacationTypeStr = u":days"_s;
    if (mHasVacationSecondsSupport) {
        const SelectVacationComboBox *vacationcombobox = w->findChild<SelectVacationComboBox *>(u"vacationcombobox"_s);
        vacationTypeStr = vacationcombobox->code();
    }
    const QSpinBox *day = w->findChild<QSpinBox *>(u"day"_s);
    const QString dayStr = QString::number(day->value());

    const MultiLineEdit *text = w->findChild<MultiLineEdit *>(u"text"_s);
    const QString textStr = text->toPlainText();

    const QLineEdit *subject = w->findChild<QLineEdit *>(u"subject"_s);
    const QString subjectStr = subject->text();

    const AbstractSelectEmailLineEdit *addresses = w->findChild<AbstractSelectEmailLineEdit *>(u"addresses"_s);
    const QString addressesStr = addresses->text();
    QString result = u"vacation"_s;
    if (!dayStr.isEmpty()) {
        result += u" %1 %2"_s.arg(vacationTypeStr, dayStr);
    }
    if (!subjectStr.isEmpty()) {
        result += u" :subject \"%1\""_s.arg(subjectStr);
    }
    if (!addressesStr.isEmpty()) {
        result += u" :addresses %1"_s.arg(AutoCreateScriptUtil::createAddressList(addressesStr, false));
    }
    if (!textStr.isEmpty()) {
        result += u" text:%1"_s.arg(AutoCreateScriptUtil::createMultiLine(textStr));
    } else {
        result += u';'; // Be sure to have ";"
    }
    return result;
}

QString SieveActionVacation::serverNeedsCapability() const
{
    return u"vacation"_s;
}

bool SieveActionVacation::needCheckIfServerHasCapability() const
{
    return true;
}

QStringList SieveActionVacation::needRequires(QWidget *) const
{
    QStringList lst;
    if (mHasVacationSecondsSupport) {
        lst << u"vacation-seconds"_s;
    }
    lst << u"vacation"_s;
    return lst;
}

QString SieveActionVacation::help() const
{
    QString helpStr = i18n(
        "The \"vacation\" action implements a vacation autoresponder similar to the vacation command available under many versions of Unix. Its purpose is to "
        "provide correspondents with notification that the user is away for an extended period of time and that they should not expect quick responses.");
    if (mHasVacationSecondsSupport) {
        helpStr = u'\n'
            + i18n("Through the \":days\" parameter, it limits the number of auto-replies to the same sender to one per [n] days, for a specified number of "
                   "days. But there are cases when one needs more granularity, if one would like to generate \"vacation\" replies more frequently.");
        helpStr += u'\n' + i18n("This extension defines a \":seconds\" parameter to provide more granularity for such situations.");
    }
    return helpStr;
}

QUrl SieveActionVacation::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

#include "moc_sieveactionvacation.cpp"
