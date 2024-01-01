/*
   SPDX-FileCopyrightText: 2013-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sieveactionwidgetlister.h"
#include "autocreatescriptutil_p.h"
#include "commonwidgets/sievehelpbutton.h"
#include "sieveactions/sieveaction.h"
#include "sieveactions/sieveactionlist.h"
#include "sieveeditorgraphicalmodewidget.h"
#include "sievescriptdescriptiondialog.h"

#include <KLocalizedString>
#include <QIcon>
#include <QPushButton>
#include <QUrl>

#include "libksieveui_debug.h"
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QPointer>
#include <QToolButton>
#include <QWhatsThis>

#include <autocreatescripts/sieveactions/sieveactionsetvariable.h>

using namespace KSieveUi;

static int MINIMUMACTION = 1;
static int MAXIMUMACTION = 8;

SieveActionWidget::SieveActionWidget(SieveEditorGraphicalModeWidget *graphicalModeWidget, QWidget *parent)
    : QWidget(parent)
    , mSieveGraphicalModeWidget(graphicalModeWidget)
{
    initWidget();
}

SieveActionWidget::~SieveActionWidget()
{
    qDeleteAll(mActionList);
    mActionList.clear();
}

bool SieveActionWidget::isConfigurated() const
{
    return mComboBox->currentIndex() != (mComboBox->count() - 1);
}

void SieveActionWidget::setFilterAction(QWidget *widget)
{
    if (mLayout->itemAtPosition(1, 3)) {
        delete mLayout->itemAtPosition(1, 3)->widget();
    }

    if (widget) {
        mLayout->addWidget(widget, 1, 3);
    } else {
        mLayout->addWidget(new QLabel(i18n("Please select an action."), this), 1, 3);
    }
}

void SieveActionWidget::generatedScript(QString &script, QStringList &required, bool onlyActions, bool inForEveryPartLoop)
{
    const int index = mComboBox->currentIndex();
    if (index != mComboBox->count() - 1) {
        KSieveUi::SieveAction *widgetAction = mActionList.at(mComboBox->currentIndex());
        QWidget *currentWidget = mLayout->itemAtPosition(1, 3)->widget();
        const QStringList lstRequires = widgetAction->needRequires(currentWidget);
        for (const QString &r : lstRequires) {
            if (!required.contains(r)) {
                required.append(r);
            }
        }
        QString comment = widgetAction->comment();
        QString indent;
        if (!onlyActions) {
            indent += AutoCreateScriptUtil::indentation();
        }
        if (inForEveryPartLoop) {
            indent += AutoCreateScriptUtil::indentation();
        }
        if (!comment.trimmed().isEmpty()) {
            const QList<QStringView> commentList = QStringView(comment).split(QLatin1Char('\n'));
            for (const QStringView str : commentList) {
                if (str.isEmpty()) {
                    script += QLatin1Char('\n');
                } else {
                    script += indent + QLatin1Char('#') + str + QLatin1Char('\n');
                }
            }
        }
        script += indent + widgetAction->code(currentWidget) + QLatin1Char('\n');
    }
}

void SieveActionWidget::initWidget()
{
    mLayout = new QGridLayout(this);
    mLayout->setContentsMargins({});

    mComboBox = new QComboBox;
    mComboBox->setEditable(false);
    mComboBox->setMinimumWidth(50);
    const QList<KSieveUi::SieveAction *> list = KSieveUi::SieveActionList::actionList(mSieveGraphicalModeWidget);
    QStringList listCapabilities = mSieveGraphicalModeWidget->sieveCapabilities();
    // imapflags was old name of imap4flags but still used.
    if (listCapabilities.contains(QLatin1String("imap4flags"))) {
        listCapabilities.append(QStringLiteral("imapflags"));
    }
    for (const auto &action : list) {
        if (action->needCheckIfServerHasCapability()) {
            if (listCapabilities.contains(action->serverNeedsCapability())) {
                // append to the list of actions:
                mActionList.append(action);
                connect(action, &SieveAction::valueChanged, this, &SieveActionWidget::valueChanged);
                // add (i18n-ized) name to combo box
                mComboBox->addItem(action->label(), action->name());
            } else {
                delete (action);
            }
        } else {
            // append to the list of actions:
            mActionList.append(action);
            connect(action, &SieveAction::valueChanged, this, &SieveActionWidget::valueChanged);
            // add (i18n-ized) name to combo box
            mComboBox->addItem(action->label(), action->name());
        }
    }

    mHelpButton = new SieveHelpButton(this);
    connect(mHelpButton, &SieveHelpButton::clicked, this, &SieveActionWidget::slotHelp);
    mLayout->addWidget(mHelpButton, 1, 0);

    mCommentButton = new QToolButton(this);
    mCommentButton->setToolTip(i18n("Add comment"));
    mLayout->addWidget(mCommentButton, 1, 1);
    mCommentButton->setIcon(QIcon::fromTheme(QStringLiteral("view-pim-notes")));
    connect(mCommentButton, &QToolButton::clicked, this, &SieveActionWidget::slotAddComment);

    mComboBox->addItem(QLatin1String(""));

    mComboBox->setMaxCount(mComboBox->count());
    mComboBox->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
    setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
    mComboBox->adjustSize();
    mLayout->addWidget(mComboBox, 1, 2);

    updateGeometry();

    connect(mComboBox, &QComboBox::activated, this, &SieveActionWidget::slotActionChanged);

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

    connect(mAdd, &QPushButton::clicked, this, &SieveActionWidget::slotAddWidget);
    connect(mRemove, &QPushButton::clicked, this, &SieveActionWidget::slotRemoveWidget);

    clear();
}

void SieveActionWidget::slotHelp()
{
    const int index = mComboBox->currentIndex();
    if (index < mActionList.count()) {
        KSieveUi::SieveAction *action = mActionList.at(index);
        const QString help = action->help();
        const QUrl href = action->href();
        const QString fullWhatsThis = AutoCreateScriptUtil::createFullWhatsThis(help, href.toString());
        QWhatsThis::showText(QCursor::pos(), fullWhatsThis, mHelpButton);
    }
}

void SieveActionWidget::clear()
{
    mComboBox->setCurrentIndex(mComboBox->count() - 1);
    setFilterAction(nullptr);
    mCommentButton->setEnabled(false);
    mHelpButton->setEnabled(false);
}

void SieveActionWidget::slotAddComment()
{
    const int index = mComboBox->currentIndex();
    if (index < mActionList.count()) {
        KSieveUi::SieveAction *action = mActionList.at(index);
        const QString comment = action->comment();
        QPointer<SieveScriptDescriptionDialog> dlg = new SieveScriptDescriptionDialog;
        dlg->setDescription(comment);
        if (dlg->exec()) {
            action->setComment(dlg->description());
            Q_EMIT valueChanged();
        }
        delete dlg;
    }
}

void SieveActionWidget::slotActionChanged(int index)
{
    if (index < mActionList.count()) {
        KSieveUi::SieveAction *action = mActionList.at(index);
        mHelpButton->setEnabled(!action->help().isEmpty());
        mCommentButton->setEnabled(true);
        setFilterAction(action->createParamWidget(this));
        // All actions after stop will not execute => don't allow to add more actions.
        const bool enableAddAction = (action->name() != QLatin1String("stop"));
        mAdd->setEnabled(enableAddAction);
    } else {
        mAdd->setEnabled(true);
        mCommentButton->setEnabled(false);
        setFilterAction(nullptr);
        mHelpButton->setEnabled(false);
    }
    Q_EMIT valueChanged();
}

void SieveActionWidget::slotAddWidget()
{
    Q_EMIT valueChanged();
    Q_EMIT addWidget(this);
}

void SieveActionWidget::slotRemoveWidget()
{
    Q_EMIT valueChanged();
    Q_EMIT removeWidget(this);
}

void SieveActionWidget::updateAddRemoveButton(bool addButtonEnabled, bool removeButtonEnabled)
{
    mAdd->setEnabled(addButtonEnabled);
    mRemove->setEnabled(removeButtonEnabled);
}

void SieveActionWidget::setLocaleVariable(const SieveGlobalVariableActionWidget::VariableElement &var)
{
    const int index = mComboBox->findData(QStringLiteral("set"));
    if (index != -1) {
        mComboBox->setCurrentIndex(index);
        slotActionChanged(index);
        auto localVar = qobject_cast<KSieveUi::SieveActionSetVariable *>(mActionList.at(index));
        if (localVar) {
            localVar->setLocalVariable(this, var);
        }
    } else {
        // error += i18n("Script contains unsupported feature \"%1\"", actionName) + QLatin1Char('\n');
        // qCDebug(LIBKSIEVEUI_LOG) << "Action " << actionName << " not supported";
    }
}

void SieveActionWidget::setAction(const QString &actionName, QXmlStreamReader &element, const QString &comment, QString &error)
{
    const int index = mComboBox->findData(actionName);
    if (index != -1) {
        mComboBox->setCurrentIndex(index);
        slotActionChanged(index);
        KSieveUi::SieveAction *action = mActionList.at(index);
        action->setParamWidgetValue(element, this, error);
        action->setComment(comment);
    } else {
        error += i18n("Script contains unsupported feature \"%1\"", actionName) + QLatin1Char('\n');
        qCDebug(LIBKSIEVEUI_LOG) << "Action " << actionName << " not supported";
        element.skipCurrentElement();
    }
}

SieveActionWidgetLister::SieveActionWidgetLister(SieveEditorGraphicalModeWidget *graphicalModeWidget, QWidget *parent)
    : KPIM::KWidgetLister(false, MINIMUMACTION, MAXIMUMACTION, parent)
    , mSieveGraphicalModeWidget(graphicalModeWidget)
{
    slotClear();
    updateAddRemoveButton();
}

SieveActionWidgetLister::~SieveActionWidgetLister() = default;

void SieveActionWidgetLister::slotAddWidget(QWidget *w)
{
    addWidgetAfterThisWidget(w);
    updateAddRemoveButton();
}

void SieveActionWidgetLister::slotRemoveWidget(QWidget *w)
{
    removeWidget(w);
    updateAddRemoveButton();
}

void SieveActionWidgetLister::updateAddRemoveButton()
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
        auto w = qobject_cast<SieveActionWidget *>(*wIt);
        w->updateAddRemoveButton(addButtonEnabled, removeButtonEnabled);
    }
}

void SieveActionWidgetLister::generatedScript(QString &script, QStringList &requireModules, bool onlyActions, bool inForEveryPartLoop)
{
    const QList<QWidget *> widgetList = widgets();
    QList<QWidget *>::ConstIterator wIt = widgetList.constBegin();
    QList<QWidget *>::ConstIterator wEnd = widgetList.constEnd();
    for (; wIt != wEnd; ++wIt) {
        auto w = qobject_cast<SieveActionWidget *>(*wIt);
        w->generatedScript(script, requireModules, onlyActions, inForEveryPartLoop);
    }
}

void SieveActionWidgetLister::reconnectWidget(SieveActionWidget *w)
{
    connect(w, &SieveActionWidget::addWidget, this, &SieveActionWidgetLister::slotAddWidget, Qt::UniqueConnection);
    connect(w, &SieveActionWidget::removeWidget, this, &SieveActionWidgetLister::slotRemoveWidget, Qt::UniqueConnection);
    connect(w, &SieveActionWidget::valueChanged, this, &SieveActionWidgetLister::valueChanged, Qt::UniqueConnection);
}

void SieveActionWidgetLister::clearWidget(QWidget *aWidget)
{
    if (aWidget) {
        auto widget = static_cast<SieveActionWidget *>(aWidget);
        widget->clear();
        updateAddRemoveButton();
    }
    Q_EMIT valueChanged();
}

QWidget *SieveActionWidgetLister::createWidget(QWidget *parent)
{
    auto w = new SieveActionWidget(mSieveGraphicalModeWidget, parent);
    reconnectWidget(w);
    return w;
}

int SieveActionWidgetLister::actionNumber() const
{
    return widgets().count();
}

void SieveActionWidgetLister::loadLocalVariable(const SieveGlobalVariableActionWidget::VariableElement &var)
{
    auto w = qobject_cast<SieveActionWidget *>(widgets().constLast());
    if (w->isConfigurated()) {
        addWidgetAfterThisWidget(widgets().constLast());
        w = qobject_cast<SieveActionWidget *>(widgets().constLast());
    }
    w->setLocaleVariable(var);
}

void SieveActionWidgetLister::loadScript(QXmlStreamReader &element, bool onlyActions, QString &error)
{
    QString comment;
    if (onlyActions) {
        const QStringView tagName = element.name();
        if (tagName == QLatin1StringView("action")) {
            if (element.attributes().hasAttribute(QLatin1StringView("name"))) {
                const QString actionName = element.attributes().value(QLatin1StringView("name")).toString();
                auto w = qobject_cast<SieveActionWidget *>(widgets().constLast());
                if (w->isConfigurated()) {
                    addWidgetAfterThisWidget(widgets().constLast());
                    w = qobject_cast<SieveActionWidget *>(widgets().constLast());
                }
                w->setAction(actionName, element, comment, error);
                // comment.clear();
            } else if (tagName == QLatin1StringView("crlf")) {
                element.skipCurrentElement();
                // nothing
            } else {
                qCDebug(LIBKSIEVEUI_LOG) << " SieveActionWidgetLister::loadScript don't have name attribute " << tagName;
            }
        } else {
            qCDebug(LIBKSIEVEUI_LOG) << " SieveActionWidgetLister::loadScript Unknown tag name " << tagName;
        }
    } else {
        bool firstAction = true;
        bool previousActionWasAComment = false;
        while (element.readNextStartElement()) {
            const QStringView tagName = element.name();
            if (tagName == QLatin1StringView("action") || tagName == QLatin1StringView("control") /*for break action*/) {
                if (element.attributes().hasAttribute(QLatin1StringView("name"))) {
                    const QString actionName = element.attributes().value(QLatin1StringView("name")).toString();
                    if (tagName == QLatin1StringView("control") && actionName == QLatin1StringView("if")) {
                        qCDebug(LIBKSIEVEUI_LOG) << "We found an loop if in a loop if. Not supported";
                        error += i18n("We detected a loop if in a loop if. It's not supported") + QLatin1Char('\n');
                    }
                    if (firstAction) {
                        firstAction = false;
                    } else {
                        addWidgetAfterThisWidget(widgets().constLast());
                    }
                    auto w = qobject_cast<SieveActionWidget *>(widgets().constLast());
                    w->setAction(actionName, element, comment, error);
                    comment.clear();
                } else {
                    qCDebug(LIBKSIEVEUI_LOG) << " SieveActionWidgetLister::loadScript don't have name attribute " << tagName;
                }
                previousActionWasAComment = false;
            } else if (tagName == QLatin1StringView("comment")) {
                if (!comment.isEmpty()) {
                    comment += QLatin1Char('\n');
                }
                previousActionWasAComment = true;
                comment += element.readElementText();
            } else if (tagName == QLatin1StringView("crlf")) {
                // Add new line if previous action was a comment
                if (previousActionWasAComment) {
                    comment += QLatin1Char('\n');
                }
                element.skipCurrentElement();
            } else {
                qCDebug(LIBKSIEVEUI_LOG) << " SieveActionWidgetLister::loadScript unknown tagName " << tagName;
            }
        }
    }
}

#include "moc_sieveactionwidgetlister.cpp"
