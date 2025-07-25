/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveactionextracttext.h"
using namespace Qt::Literals::StringLiterals;

#include "editor/sieveeditorutil.h"

#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QLineEdit>

#include "libksieveui_debug.h"
#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>
#include <QXmlStreamReader>

using namespace KSieveUi;
SieveActionExtractText::SieveActionExtractText(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QObject *parent)
    : SieveAction(sieveGraphicalModeWidget, u"extracttext"_s, i18n("Extract Text"), parent)
{
}

QWidget *SieveActionExtractText::createParamWidget(QWidget *parent) const
{
    auto w = new QWidget(parent);
    auto grid = new QGridLayout;
    grid->setContentsMargins({});
    w->setLayout(grid);

    auto lab = new QLabel(i18nc("@label:textbox", "Number of characters:"));
    grid->addWidget(lab, 0, 0);

    auto nbCharacters = new QSpinBox;
    nbCharacters->setMinimum(1);
    nbCharacters->setMaximum(99999);
    nbCharacters->setObjectName(QLatin1StringView("numberOfCharacters"));
    grid->addWidget(nbCharacters, 0, 1);
    connect(nbCharacters, &QSpinBox::valueChanged, this, &SieveActionExtractText::valueChanged);

    lab = new QLabel(i18nc("@label:textbox", "Stored in variable name:"));
    grid->addWidget(lab, 1, 0);

    auto variableName = new QLineEdit;
    KLineEditEventHandler::catchReturnKey(variableName);
    connect(variableName, &QLineEdit::textChanged, this, &SieveActionExtractText::valueChanged);
    variableName->setObjectName(QLatin1StringView("variablename"));
    grid->addWidget(variableName, 1, 1);

    return w;
}

void SieveActionExtractText::setParamWidgetValue(QXmlStreamReader &element, QWidget *w, QString &error)
{
    while (element.readNextStartElement()) {
        const QStringView tagName = element.name();

        if (tagName == QLatin1StringView("tag")) {
            element.skipCurrentElement();
            // TODO ?
        } else if (tagName == QLatin1StringView("num")) {
            auto numberOfCharacters = w->findChild<QSpinBox *>(u"numberOfCharacters"_s);
            numberOfCharacters->setValue(element.readElementText().toInt());
        } else if (tagName == QLatin1StringView("str")) {
            auto variableName = w->findChild<QLineEdit *>(u"variablename"_s);
            variableName->setText(element.readElementText());
        } else if (tagName == QLatin1StringView("crlf")) {
            element.skipCurrentElement();
            // nothing
        } else if (tagName == QLatin1StringView("comment")) {
            element.skipCurrentElement();
            // implement in the future ?
        } else {
            unknownTag(tagName, error);
            qCDebug(LIBKSIEVEUI_LOG) << " SieveActionExtractText::setParamWidgetValue unknown tagName " << tagName;
        }
    }
}

QString SieveActionExtractText::code(QWidget *w) const
{
    const QSpinBox *numberOfCharacters = w->findChild<QSpinBox *>(u"numberOfCharacters"_s);
    const QString numberOfCharactersStr = QString::number(numberOfCharacters->value());

    const QLineEdit *variableName = w->findChild<QLineEdit *>(u"variablename"_s);
    const QString variableNameStr = variableName->text();

    const QString result = u"extracttext :first %1 \"%2\";"_s.arg(numberOfCharactersStr, variableNameStr);
    return result;
}

QStringList SieveActionExtractText::needRequires(QWidget * /*parent*/) const
{
    return QStringList() << u"extracttext"_s;
}

bool SieveActionExtractText::needCheckIfServerHasCapability() const
{
    return true;
}

QString SieveActionExtractText::serverNeedsCapability() const
{
    return u"extracttext"_s;
}

QString SieveActionExtractText::help() const
{
    return i18n("The \"extracttext\" action may be used within the context of a \"foreverypart\" loop and is used to store text into a variable");
}

QUrl SieveActionExtractText::href() const
{
    return SieveEditorUtil::helpUrl(SieveEditorUtil::strToVariableName(name()));
}

#include "moc_sieveactionextracttext.cpp"
