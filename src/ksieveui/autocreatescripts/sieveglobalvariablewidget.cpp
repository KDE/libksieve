/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sieveglobalvariablewidget.h"
using namespace Qt::Literals::StringLiterals;

#include "autocreatescriptutil_p.h"
#include "commonwidgets/sievehelpbutton.h"
#include "editor/sieveeditorutil.h"
#include "sievescriptblockwidget.h"
#include "widgets/lineeditvalidator.h"

#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QIcon>
#include <QLineEdit>
#include <QPushButton>

#include "libksieveui_debug.h"
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>
#include <QWhatsThis>
#include <QXmlStreamReader>

using namespace KSieveUi;
static const int MINIMUMGLOBALVARIABLEACTION = 1;
static const int MAXIMUMGLOBALVARIABLEACTION = 15;

SieveGlobalVariableActionWidget::SieveGlobalVariableActionWidget(QWidget *parent)
    : QWidget(parent)
{
    initWidget();
}

SieveGlobalVariableActionWidget::~SieveGlobalVariableActionWidget() = default;

void SieveGlobalVariableActionWidget::generatedScript(QString &script)
{
    const QString variableName = mVariableName->text();
    if (variableName.trimmed().isEmpty()) {
        return;
    }
    script += QLatin1StringView("global ");
    script += u"\"%1\";\n"_s.arg(variableName);
    if (mSetValueTo->isChecked() && !mVariableValue->text().isEmpty()) {
        script += u"set \"%1\" \"%2\";\n"_s.arg(variableName, mVariableValue->text());
    }
}

void SieveGlobalVariableActionWidget::initWidget()
{
    mLayout = new QGridLayout(this);
    mLayout->setContentsMargins({});

    auto lab = new QLabel(i18nc("@label:textbox", "Variable name:"), this);
    mLayout->addWidget(lab, 1, 0);

    mVariableName = new LineEditValidator(this);
    connect(mVariableName, &QLineEdit::textChanged, this, &SieveGlobalVariableActionWidget::valueChanged);
    mLayout->addWidget(mVariableName, 1, 1);

    mSetValueTo = new QCheckBox(i18nc("@option:check", "Set value to:"), this);
    connect(mSetValueTo, &QCheckBox::toggled, this, &SieveGlobalVariableActionWidget::valueChanged);
    mLayout->addWidget(mSetValueTo, 1, 2);
    mSetValueTo->setChecked(false);

    mVariableValue = new QLineEdit(this);
    KLineEditEventHandler::catchReturnKey(mVariableValue);
    connect(mVariableValue, &QLineEdit::textChanged, this, &SieveGlobalVariableActionWidget::valueChanged);
    mVariableValue->setEnabled(false);
    mLayout->addWidget(mVariableValue, 1, 3);

    connect(mSetValueTo, &QCheckBox::clicked, mVariableValue, &QLineEdit::setEnabled);

    mAdd = new QPushButton(this);
    mAdd->setIcon(QIcon::fromTheme(u"list-add"_s));
    mAdd->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

    mRemove = new QPushButton(this);
    mRemove->setIcon(QIcon::fromTheme(u"list-remove"_s));
    mRemove->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    mLayout->addWidget(mAdd, 1, 4);
    mLayout->addWidget(mRemove, 1, 5);

    connect(mAdd, &QPushButton::clicked, this, &SieveGlobalVariableActionWidget::slotAddWidget);
    connect(mRemove, &QPushButton::clicked, this, &SieveGlobalVariableActionWidget::slotRemoveWidget);
}

void SieveGlobalVariableActionWidget::clear()
{
    mVariableName->clear();
    mSetValueTo->setChecked(false);
    mVariableValue->setEnabled(false);
    mVariableValue->clear();
}

bool SieveGlobalVariableActionWidget::isInitialized() const
{
    return !mVariableName->text().isEmpty();
}

QString SieveGlobalVariableActionWidget::variableName() const
{
    const QString varName = mVariableName->text();
    if (varName.trimmed().isEmpty()) {
        return {};
    }
    return varName;
}

void SieveGlobalVariableActionWidget::setVariableValue(const QString &name)
{
    mSetValueTo->setChecked(true);
    mVariableValue->setText(name);
    mVariableValue->setEnabled(true);
}

void SieveGlobalVariableActionWidget::loadScript(QXmlStreamReader &element, QString &error)
{
    while (element.readNextStartElement()) {
        const QStringView tagName = element.name();
        if (tagName == QLatin1StringView("str")) {
            mVariableName->setText(element.readElementText());
        } else {
            const QString result = tagName.toString();
            error += i18n("Unknown tag \"%1\" during loading of variables.", result) + u'\n';
            qCDebug(LIBKSIEVEUI_LOG) << " SieveGlobalVariableActionWidget::loadScript unknown tagName " << tagName;
        }
    }
}

void SieveGlobalVariableActionWidget::slotAddWidget()
{
    Q_EMIT addWidget(this);
    Q_EMIT valueChanged();
}

void SieveGlobalVariableActionWidget::slotRemoveWidget()
{
    Q_EMIT removeWidget(this);
    Q_EMIT valueChanged();
}

void SieveGlobalVariableActionWidget::updateAddRemoveButton(bool addButtonEnabled, bool removeButtonEnabled)
{
    mAdd->setEnabled(addButtonEnabled);
    mRemove->setEnabled(removeButtonEnabled);
}

SieveGlobalVariableWidget::SieveGlobalVariableWidget(QWidget *parent)
    : SieveWidgetPageAbstract(parent)
{
    auto lay = new QVBoxLayout(this);
    mHelpButton = new SieveHelpButton(this);
    lay->addWidget(mHelpButton);
    connect(mHelpButton, &SieveHelpButton::clicked, this, &SieveGlobalVariableWidget::slotHelp);

    mGlobalVariableLister = new SieveGlobalVariableLister(this);
    connect(mGlobalVariableLister, &SieveGlobalVariableLister::valueChanged, this, &SieveGlobalVariableWidget::valueChanged);

    auto scrollArea = new QScrollArea(this);
    scrollArea->setAutoFillBackground(false);
    scrollArea->setWidget(mGlobalVariableLister);
    scrollArea->setWidgetResizable(true);
    scrollArea->setAlignment(Qt::AlignTop);
    lay->addWidget(scrollArea);

    setPageType(KSieveUi::SieveScriptBlockWidget::GlobalVariable);
}

SieveGlobalVariableWidget::~SieveGlobalVariableWidget() = default;

void SieveGlobalVariableWidget::slotHelp()
{
    const QString help = i18n(
        "A variable has global scope in all scripts that have declared it with the \"global\" command.  If a script uses that variable name without declaring "
        "it global, the name specifies a separate, non-global variable within that script.");
    const QUrl href = KSieveUi::SieveEditorUtil::helpUrl(KSieveUi::SieveEditorUtil::GlobalVariable);
    const QString fullWhatsThis = AutoCreateScriptUtil::createFullWhatsThis(help, href.toString());
    QWhatsThis::showText(QCursor::pos(), fullWhatsThis, mHelpButton);
}

void SieveGlobalVariableWidget::generatedScript(QString &script, QStringList &requireModules, bool inForEveryPartLoop)
{
    Q_UNUSED(inForEveryPartLoop)
    QString result;
    QStringList lst;
    mGlobalVariableLister->generatedScript(result, lst);
    if (!result.isEmpty()) {
        script += result;
        requireModules << lst;
    }
}

void SieveGlobalVariableWidget::loadScript(QXmlStreamReader &element, QString &error)
{
    mGlobalVariableLister->loadScript(element, error);
}

SieveGlobalVariableActionWidget::VariableElement SieveGlobalVariableWidget::loadSetVariable(QXmlStreamReader &element, QString &error)
{
    return mGlobalVariableLister->loadSetVariable(element, error);
}

SieveGlobalVariableLister::SieveGlobalVariableLister(QWidget *parent)
    : KPIM::KWidgetLister(false, MINIMUMGLOBALVARIABLEACTION, MAXIMUMGLOBALVARIABLEACTION, parent)
{
    slotClear();
    updateAddRemoveButton();
}

SieveGlobalVariableLister::~SieveGlobalVariableLister() = default;

void SieveGlobalVariableLister::slotAddWidget(QWidget *w)
{
    addWidgetAfterThisWidget(w);
    updateAddRemoveButton();
}

void SieveGlobalVariableLister::slotRemoveWidget(QWidget *w)
{
    removeWidget(w);
    updateAddRemoveButton();
}

void SieveGlobalVariableLister::updateAddRemoveButton()
{
    QList<QWidget *> widgetList = widgets();
    const int numberOfWidget(widgetList.count());
    bool addButtonEnabled = false;
    bool removeButtonEnabled = false;
    if (numberOfWidget <= widgetsMinimum()) {
        addButtonEnabled = true;
        removeButtonEnabled = false;
    } else if (numberOfWidget >= widgetsMaximum()) {
        addButtonEnabled = false;
        removeButtonEnabled = true;
    } else {
        addButtonEnabled = true;
        removeButtonEnabled = true;
    }
    QList<QWidget *>::ConstIterator with = widgetList.constBegin();
    QList<QWidget *>::ConstIterator wEnd = widgetList.constEnd();
    for (; with != wEnd; ++with) {
        auto w = qobject_cast<SieveGlobalVariableActionWidget *>(*with);
        w->updateAddRemoveButton(addButtonEnabled, removeButtonEnabled);
    }
}

void SieveGlobalVariableLister::generatedScript(QString &script, QStringList &requireModules)
{
    requireModules << u"include"_s;
    const QList<QWidget *> widgetList = widgets();
    QList<QWidget *>::ConstIterator with = widgetList.constBegin();
    QList<QWidget *>::ConstIterator wEnd = widgetList.constEnd();
    for (; with != wEnd; ++with) {
        auto w = qobject_cast<SieveGlobalVariableActionWidget *>(*with);
        w->generatedScript(script);
    }
}

void SieveGlobalVariableLister::reconnectWidget(SieveGlobalVariableActionWidget *w)
{
    connect(w, &SieveGlobalVariableActionWidget::addWidget, this, &SieveGlobalVariableLister::slotAddWidget, Qt::UniqueConnection);
    connect(w, &SieveGlobalVariableActionWidget::removeWidget, this, &SieveGlobalVariableLister::slotRemoveWidget, Qt::UniqueConnection);
    connect(w, &SieveGlobalVariableActionWidget::valueChanged, this, &SieveGlobalVariableLister::valueChanged, Qt::UniqueConnection);
}

void SieveGlobalVariableLister::clearWidget(QWidget *aWidget)
{
    if (aWidget) {
        auto widget = static_cast<SieveGlobalVariableActionWidget *>(aWidget);
        widget->clear();
        updateAddRemoveButton();
    }
    Q_EMIT valueChanged();
}

QWidget *SieveGlobalVariableLister::createWidget(QWidget *parent)
{
    auto w = new SieveGlobalVariableActionWidget(parent);
    reconnectWidget(w);
    return w;
}

void SieveGlobalVariableLister::loadScript(QXmlStreamReader &element, QString &error)
{
    SieveGlobalVariableActionWidget *w = static_cast<SieveGlobalVariableActionWidget *>(widgets().constLast());
    if (w->isInitialized()) {
        addWidgetAfterThisWidget(widgets().constLast());
        w = static_cast<SieveGlobalVariableActionWidget *>(widgets().constLast());
    }
    w->loadScript(element, error);
}

SieveGlobalVariableActionWidget::VariableElement SieveGlobalVariableLister::loadSetVariable(QXmlStreamReader &element, QString & /*error*/)
{
    SieveGlobalVariableActionWidget::VariableElement var;
    QString variableName;
    QString variableValue;
    int index = 0;
    while (element.readNextStartElement()) {
        const QStringView tagName = element.name();
        if (tagName == QLatin1StringView("str")) {
            if (index == 0) {
                variableName = element.readElementText();
            } else if (index == 1) {
                variableValue = element.readElementText();
            } else {
                qCDebug(LIBKSIEVEUI_LOG) << " SieveGlobalVariableLister::loadSetVariable too many argument:" << index;
            }
            ++index;
        } else {
            qCDebug(LIBKSIEVEUI_LOG) << " SieveGlobalVariableLister::loadSetVariable unknown tagName " << tagName;
        }
    }

    const QList<QWidget *> lstWidget = widgets();
    bool globalVariableFound = false;
    for (QWidget *widget : lstWidget) {
        auto w = static_cast<SieveGlobalVariableActionWidget *>(widget);
        if (w->variableName() == variableName) {
            w->setVariableValue(variableValue);
            globalVariableFound = true;
        }
    }
    if (!globalVariableFound) {
        var.variableName = variableName;
        var.variableValue = variableValue;
    }
    return var;
}

#include "moc_sieveglobalvariablewidget.cpp"
