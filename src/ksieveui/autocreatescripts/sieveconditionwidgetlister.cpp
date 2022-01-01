/*
   SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sieveconditionwidgetlister.h"
#include "autocreatescriptdialog.h"
#include "autocreatescriptutil_p.h"
#include "commonwidgets/sievehelpbutton.h"
#include "libksieve_debug.h"
#include "sieveconditions/sievecondition.h"
#include "sieveconditions/sieveconditionlist.h"
#include "sieveeditorgraphicalmodewidget.h"

#include <KLocalizedString>
#include <QComboBox>
#include <QIcon>
#include <QPointer>
#include <QPushButton>
#include <QUrl>

#include "sievescriptdescriptiondialog.h"
#include <QGridLayout>
#include <QLabel>
#include <QWhatsThis>

using namespace KSieveUi;

static const int MINIMUMCONDITION = 1;
static const int MAXIMUMCONDITION = 8;

SieveConditionWidget::SieveConditionWidget(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QWidget *parent)
    : QWidget(parent)
    , mSieveGraphicalModeWidget(sieveGraphicalModeWidget)
{
    initWidget();
}

SieveConditionWidget::~SieveConditionWidget()
{
    qDeleteAll(mConditionList);
    mConditionList.clear();
}

void SieveConditionWidget::setFilterCondition(QWidget *widget)
{
    if (mLayout->itemAtPosition(1, 3)) {
        delete mLayout->itemAtPosition(1, 3)->widget();
    }

    if (widget) {
        mLayout->addWidget(widget, 1, 3);
    } else {
        mLayout->addWidget(new QLabel(i18n("Please select an condition."), this), 1, 3);
    }
}

void SieveConditionWidget::generatedScript(QString &script, QStringList &required, bool inForEveryPartLoop)
{
    Q_UNUSED(inForEveryPartLoop)
    const int index = mComboBox->currentIndex();
    if (index != mComboBox->count() - 1) {
        KSieveUi::SieveCondition *widgetCondition = mConditionList.at(mComboBox->currentIndex());
        QWidget *currentWidget = mLayout->itemAtPosition(1, 3)->widget();
        const QStringList lstRequires = widgetCondition->needRequires(currentWidget);
        for (const QString &r : lstRequires) {
            if (!required.contains(r)) {
                required.append(r);
            }
        }
        script += mConditionList.at(mComboBox->currentIndex())->code(currentWidget) + QLatin1Char('\n');
    }
}

void SieveConditionWidget::initWidget()
{
    mLayout = new QGridLayout(this);
    mLayout->setContentsMargins({});

    mComboBox = new QComboBox;
    mComboBox->setMinimumWidth(50);
    mComboBox->setEditable(false);

    const QList<KSieveUi::SieveCondition *> list = KSieveUi::SieveConditionList::conditionList(mSieveGraphicalModeWidget);
    for (const auto &action : list) {
        if (action->needCheckIfServerHasCapability()) {
            if (mSieveGraphicalModeWidget->sieveCapabilities().contains(action->serverNeedsCapability())) {
                // append to the list of actions:
                mConditionList.append(action);
                connect(action, &SieveCondition::valueChanged, this, &SieveConditionWidget::valueChanged);
                // add (i18n-ized) name to combo box
                mComboBox->addItem(action->label(), action->name());
            } else {
                delete action;
            }
        } else {
            // append to the list of actions:
            mConditionList.append(action);
            connect(action, &SieveCondition::valueChanged, this, &SieveConditionWidget::valueChanged);
            // add (i18n-ized) name to combo box
            mComboBox->addItem(action->label(), action->name());
        }
    }

    mHelpButton = new SieveHelpButton(this);
    mLayout->addWidget(mHelpButton, 1, 0);
    connect(mHelpButton, &SieveHelpButton::clicked, this, &SieveConditionWidget::slotHelp);

    mCommentButton = new QToolButton(this);
    mCommentButton->setToolTip(i18n("Add comment"));
    mLayout->addWidget(mCommentButton, 1, 1);
    mCommentButton->setIcon(QIcon::fromTheme(QStringLiteral("view-pim-notes")));
    connect(mCommentButton, &QToolButton::clicked, this, &SieveConditionWidget::slotAddComment);

    mComboBox->addItem(QLatin1String(""));
    mLayout->addWidget(mComboBox, 1, 2);
    connect(mComboBox, &QComboBox::activated, this, &SieveConditionWidget::slotConditionChanged);

    mComboBox->setMaxCount(mComboBox->count());
    mComboBox->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
    setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
    mComboBox->adjustSize();

    mAdd = new QPushButton(this);
    mAdd->setIcon(QIcon::fromTheme(QStringLiteral("list-add")));
    mAdd->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

    mRemove = new QPushButton(this);
    mRemove->setIcon(QIcon::fromTheme(QStringLiteral("list-remove")));
    mRemove->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

    mLayout->addWidget(mAdd, 1, 4);
    mLayout->addWidget(mRemove, 1, 5);

    // redirect focus to the filter action combo box
    setFocusProxy(mComboBox);

    connect(mAdd, &QPushButton::clicked, this, &SieveConditionWidget::slotAddWidget);
    connect(mRemove, &QPushButton::clicked, this, &SieveConditionWidget::slotRemoveWidget);

    clear();
}

void SieveConditionWidget::slotAddComment()
{
    const int index = mComboBox->currentIndex();
    if (index < mConditionList.count()) {
        KSieveUi::SieveCondition *condition = mConditionList.at(index);
        const QString comment = condition->comment();
        QPointer<SieveScriptDescriptionDialog> dlg = new SieveScriptDescriptionDialog(this);
        dlg->setDescription(comment);
        if (dlg->exec()) {
            condition->setComment(dlg->description());
            Q_EMIT valueChanged();
        }
        delete dlg;
    }
}

void SieveConditionWidget::slotHelp()
{
    const int index = mComboBox->currentIndex();
    if (index < mConditionList.count()) {
        KSieveUi::SieveCondition *condition = mConditionList.at(index);
        const QString help = condition->help();
        const QUrl href = condition->href();
        const QString fullWhatsThis = AutoCreateScriptUtil::createFullWhatsThis(help, href.toString());
        QWhatsThis::showText(QCursor::pos(), fullWhatsThis, mHelpButton);
    }
}

void SieveConditionWidget::slotConditionChanged(int index)
{
    if (index < mConditionList.count()) {
        KSieveUi::SieveCondition *condition = mConditionList.at(index);
        mHelpButton->setEnabled(!condition->help().isEmpty());
        setFilterCondition(condition->createParamWidget(this));
        mCommentButton->setEnabled(true);
    } else {
        setFilterCondition(nullptr);
        mHelpButton->setEnabled(false);
        mCommentButton->setEnabled(false);
    }
    Q_EMIT valueChanged();
}

void SieveConditionWidget::slotAddWidget()
{
    Q_EMIT addWidget(this);
    Q_EMIT valueChanged();
}

void SieveConditionWidget::slotRemoveWidget()
{
    Q_EMIT removeWidget(this);
    Q_EMIT valueChanged();
}

void SieveConditionWidget::clear()
{
    mComboBox->setCurrentIndex(mComboBox->count() - 1);
    setFilterCondition(nullptr);
    mHelpButton->setEnabled(false);
    mCommentButton->setEnabled(false);
}

void SieveConditionWidget::updateAddRemoveButton(bool addButtonEnabled, bool removeButtonEnabled)
{
    mAdd->setEnabled(addButtonEnabled);
    mRemove->setEnabled(removeButtonEnabled);
}

void SieveConditionWidget::setCondition(const QString &conditionName, QXmlStreamReader &element, bool notCondition, QString &error)
{
    const int index = mComboBox->findData(conditionName);
    if (index != -1) {
        mComboBox->setCurrentIndex(index);
        slotConditionChanged(index);
        KSieveUi::SieveCondition *condition = mConditionList.at(index);
        condition->setParamWidgetValue(element, this, notCondition, error);
    } else {
        error += i18n("Script contains unsupported feature \"%1\"", conditionName) + QLatin1Char('\n');
        qCDebug(LIBKSIEVE_LOG) << "Condition " << conditionName << " not supported";
        element.skipCurrentElement();
    }
}

SieveConditionWidgetLister::SieveConditionWidgetLister(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, QWidget *parent)
    : KPIM::KWidgetLister(false, MINIMUMCONDITION, MAXIMUMCONDITION, parent)
    , mSieveGraphicalModeWidget(sieveGraphicalModeWidget)
{
    slotClear();
    updateAddRemoveButton();
}

SieveConditionWidgetLister::~SieveConditionWidgetLister() = default;

void SieveConditionWidgetLister::slotAddWidget(QWidget *w)
{
    addWidgetAfterThisWidget(w);
    updateAddRemoveButton();
}

void SieveConditionWidgetLister::slotRemoveWidget(QWidget *w)
{
    removeWidget(w);
    updateAddRemoveButton();
}

void SieveConditionWidgetLister::updateAddRemoveButton()
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
    QList<QWidget *>::ConstIterator wIt = widgetList.constBegin();
    QList<QWidget *>::ConstIterator wEnd = widgetList.constEnd();
    for (; wIt != wEnd; ++wIt) {
        auto w = qobject_cast<SieveConditionWidget *>(*wIt);
        w->updateAddRemoveButton(addButtonEnabled, removeButtonEnabled);
    }
}

void SieveConditionWidgetLister::reconnectWidget(SieveConditionWidget *w)
{
    connect(w, &SieveConditionWidget::addWidget, this, &SieveConditionWidgetLister::slotAddWidget, Qt::UniqueConnection);
    connect(w, &SieveConditionWidget::removeWidget, this, &SieveConditionWidgetLister::slotRemoveWidget, Qt::UniqueConnection);
    connect(w, &SieveConditionWidget::valueChanged, this, &SieveConditionWidgetLister::valueChanged, Qt::UniqueConnection);
}

void SieveConditionWidgetLister::clearWidget(QWidget *aWidget)
{
    if (aWidget) {
        auto widget = static_cast<SieveConditionWidget *>(aWidget);
        widget->clear();
    }
    Q_EMIT valueChanged();
}

QWidget *SieveConditionWidgetLister::createWidget(QWidget *parent)
{
    auto w = new SieveConditionWidget(mSieveGraphicalModeWidget, parent);
    reconnectWidget(w);
    return w;
}

void SieveConditionWidgetLister::generatedScript(QString &script, int &numberOfCondition, QStringList &requireModules, bool inForEveryPartLoop)
{
    const QList<QWidget *> widgetList = widgets();
    QList<QWidget *>::ConstIterator wIt = widgetList.constBegin();
    QList<QWidget *>::ConstIterator wEnd = widgetList.constEnd();
    bool wasFirst = true;
    for (; wIt != wEnd; ++wIt) {
        QString condition;
        auto w = qobject_cast<SieveConditionWidget *>(*wIt);
        w->generatedScript(condition, requireModules, inForEveryPartLoop);
        if (!condition.isEmpty()) {
            if (!wasFirst) {
                if (inForEveryPartLoop) {
                    script += AutoCreateScriptUtil::indentation();
                }
                script += QLatin1String(", ");
            }
            script += condition;
            wasFirst = false;
            ++numberOfCondition;
        }
    }
}

int SieveConditionWidgetLister::conditionNumber() const
{
    return widgets().count();
}

void SieveConditionWidgetLister::loadTest(QXmlStreamReader &element, bool notCondition, QString &error)
{
    if (notCondition) {
        element.readNextStartElement();
    }
    if (element.attributes().hasAttribute(QLatin1String("name"))) {
        const QString conditionName = element.attributes().value(QLatin1String("name")).toString();
        auto w = qobject_cast<SieveConditionWidget *>(widgets().constLast());
        w->setCondition(conditionName, element, notCondition, error);
    }
    if (notCondition) {
        element.skipCurrentElement();
    }
}

void SieveConditionWidgetLister::loadScript(QXmlStreamReader &element, bool uniqTest, bool notCondition, QString &error)
{
    if (uniqTest) {
        loadTest(element, notCondition, error);
    } else {
        bool firstCondition = true;
        if (notCondition) {
            element.readNextStartElement();
        }
        while (element.readNextStartElement()) {
            const QStringRef tagName = element.name();
            if (tagName == QLatin1String("testlist")) {
                while (element.readNextStartElement()) {
                    const QStringRef testTagName = element.name();
                    if (testTagName == QLatin1String("test")) {
                        if (element.attributes().hasAttribute(QLatin1String("name"))) {
                            QString conditionName = element.attributes().value(QLatin1String("name")).toString();
                            if (firstCondition) {
                                firstCondition = false;
                            } else {
                                addWidgetAfterThisWidget(widgets().constLast());
                            }
                            auto w = qobject_cast<SieveConditionWidget *>(widgets().constLast());
                            if (conditionName == QLatin1String("not")) {
                                notCondition = true;
                                element.readNextStartElement();
                                if (element.attributes().hasAttribute(QLatin1String("name"))) {
                                    conditionName = element.attributes().value(QLatin1String("name")).toString();
                                }
                                w->setCondition(conditionName, element, notCondition, error);
                                element.skipCurrentElement();
                            } else {
                                notCondition = false;
                                w->setCondition(conditionName, element, notCondition, error);
                            }
                        }
                    } else if (testTagName == QLatin1String("crlf")) {
                        element.skipCurrentElement();
                        // nothing
                    } else if (testTagName == QLatin1String("comment")) {
                        qDebug() << "Need to implement comment here ";
                        element.skipCurrentElement();
                        // nothing
                        // implement in the future ?
                    } else {
                        qCDebug(LIBKSIEVE_LOG) << " SieveConditionWidgetLister::loadScript unknown condition tag: " << testTagName;
                    }
                }
            }
        }
    }
}
