/*
   SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "sievescriptlistbox.h"
using namespace Qt::Literals::StringLiterals;

#include "sieveeditorgraphicalmodewidget.h"
#include "sieveforeverypartwidget.h"
#include "sieveglobalvariablewidget.h"
#include "sieveincludewidget.h"
#include "sievescriptdescriptiondialog.h"
#include "sievescriptpage.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QHBoxLayout>
#include <QIcon>
#include <QInputDialog>
#include <QPushButton>
#include <QXmlStreamReader>

#include "libksieveui_debug.h"
#include <QListWidget>
#include <QMenu>
#include <QPointer>
#include <QVBoxLayout>

namespace
{
inline const QString defaultScriptName()
{
    return u"SCRIPTNAME: "_s;
}

inline const QString roundcubeRuleName()
{
    return u" rule:"_s;
}
}

using namespace KSieveUi;

SieveScriptListItem::SieveScriptListItem(const QString &text, QListWidget *parent)
    : QListWidgetItem(text, parent)
{
}

SieveScriptListItem::~SieveScriptListItem() = default;

void SieveScriptListItem::setDescription(const QString &desc)
{
    mDescription = desc;
}

QString SieveScriptListItem::description() const
{
    return mDescription;
}

SieveScriptPage *SieveScriptListItem::scriptPage() const
{
    return mScriptPage;
}

void SieveScriptListItem::setScriptPage(SieveScriptPage *page)
{
    mScriptPage = page;
}

QString SieveScriptListItem::generatedScript(QStringList &required) const
{
    QString script;
    if (!mDescription.trimmed().isEmpty()) {
        const QList<QStringView> commentList = QStringView(mDescription).split(u'\n');
        for (const QStringView str : commentList) {
            if (str.isEmpty()) {
                script += u'\n';
            } else {
                script += u'#' + str + u'\n';
            }
        }
    }
    if (mScriptPage) {
        mScriptPage->generatedScript(script, required);
    }
    return script;
}

SieveScriptListBox::SieveScriptListBox(const QString &title, QWidget *parent)
    : QGroupBox(title, parent)
    , mSieveListScript(new QListWidget(this))
{
    auto layout = new QVBoxLayout(this);
    layout->setObjectName(QLatin1StringView("layout"));
    mSieveListScript->setObjectName(QLatin1StringView("mSieveListScript"));
    mSieveListScript->setDragDropMode(QAbstractItemView::InternalMove);
    mSieveListScript->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(mSieveListScript, &QListWidget::customContextMenuRequested, this, &SieveScriptListBox::slotCustomMenuRequested);
    layout->addWidget(mSieveListScript);

    //----------- the first row of buttons
    auto hb = new QWidget(this);
    auto hbHBoxLayout = new QHBoxLayout(hb);
    hbHBoxLayout->setContentsMargins({});
    hbHBoxLayout->setSpacing(4);

    mBtnTop = new QPushButton(hb);
    hbHBoxLayout->addWidget(mBtnTop);
    mBtnTop->setIcon(QIcon::fromTheme(u"go-top"_s));
    mBtnTop->setMinimumSize(mBtnTop->sizeHint() * 1.2);

    mBtnUp = new QPushButton(hb);
    hbHBoxLayout->addWidget(mBtnUp);
    mBtnUp->setAutoRepeat(true);
    mBtnUp->setIcon(QIcon::fromTheme(u"go-up"_s));
    mBtnUp->setMinimumSize(mBtnUp->sizeHint() * 1.2);
    mBtnDown = new QPushButton(hb);
    hbHBoxLayout->addWidget(mBtnDown);
    mBtnDown->setAutoRepeat(true);
    mBtnDown->setIcon(QIcon::fromTheme(u"go-down"_s));
    mBtnDown->setMinimumSize(mBtnDown->sizeHint() * 1.2);

    mBtnBottom = new QPushButton(hb);
    hbHBoxLayout->addWidget(mBtnBottom);
    mBtnBottom->setIcon(QIcon::fromTheme(u"go-bottom"_s));
    mBtnBottom->setMinimumSize(mBtnBottom->sizeHint() * 1.2);

    mBtnUp->setToolTip(i18nc("Move selected filter up.", "Up"));
    mBtnDown->setToolTip(i18nc("Move selected filter down.", "Down"));
    mBtnTop->setToolTip(i18nc("Move selected filter to the top.", "Top"));
    mBtnBottom->setToolTip(i18nc("Move selected filter to the bottom.", "Bottom"));

    layout->addWidget(hb);

    hb = new QWidget(this);
    hbHBoxLayout = new QHBoxLayout(hb);
    hbHBoxLayout->setContentsMargins({});
    hbHBoxLayout->setSpacing(4);

    mBtnNew = new QPushButton(hb);
    hbHBoxLayout->addWidget(mBtnNew);
    mBtnNew->setIcon(QIcon::fromTheme(u"document-new"_s));
    mBtnNew->setToolTip(i18nc("@info:tooltip", "New Script"));
    mBtnNew->setMinimumSize(mBtnNew->sizeHint() * 1.2);

    mBtnDelete = new QPushButton(hb);
    hbHBoxLayout->addWidget(mBtnDelete);
    mBtnDelete->setIcon(QIcon::fromTheme(u"edit-delete"_s));
    mBtnDelete->setToolTip(i18nc("@info:tooltip", "Delete Script"));
    mBtnDelete->setMinimumSize(mBtnDelete->sizeHint() * 1.2);

    mBtnRename = new QPushButton(hb);
    mBtnRename->setToolTip(i18nc("@info:tooltip", "Rename Script"));
    mBtnRename->setIcon(QIcon::fromTheme(u"edit-rename"_s));
    mBtnRename->setMinimumSize(mBtnRename->sizeHint() * 1.2);
    hbHBoxLayout->addWidget(mBtnRename);

    mBtnDescription = new QPushButton(hb);
    mBtnDescription->setToolTip(i18nc("@info:tooltip", "Edit Script Description"));
    mBtnDescription->setIcon(QIcon::fromTheme(u"edit-comment"_s));
    mBtnDescription->setMinimumSize(mBtnDescription->sizeHint() * 1.2);

    hbHBoxLayout->addWidget(mBtnDescription);

    layout->addWidget(hb);

    connect(mBtnNew, &QPushButton::clicked, this, &SieveScriptListBox::slotNew);
    connect(mBtnDelete, &QPushButton::clicked, this, &SieveScriptListBox::slotDelete);
    connect(mBtnRename, &QPushButton::clicked, this, &SieveScriptListBox::slotRename);
    connect(mBtnDescription, &QPushButton::clicked, this, &SieveScriptListBox::slotEditDescription);

    connect(mBtnUp, &QPushButton::clicked, this, &SieveScriptListBox::slotUp);
    connect(mBtnDown, &QPushButton::clicked, this, &SieveScriptListBox::slotDown);
    connect(mBtnTop, &QPushButton::clicked, this, &SieveScriptListBox::slotTop);
    connect(mBtnBottom, &QPushButton::clicked, this, &SieveScriptListBox::slotBottom);

    connect(mSieveListScript, &QListWidget::itemSelectionChanged, this, &SieveScriptListBox::updateButtons);
    connect(mSieveListScript, &QListWidget::itemClicked, this, &SieveScriptListBox::slotItemClicked);
    connect(mSieveListScript, &QListWidget::itemDoubleClicked, this, &SieveScriptListBox::slotEditDescription);
    updateButtons();
}

SieveScriptListBox::~SieveScriptListBox() = default;

void SieveScriptListBox::slotCustomMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    QAction *newScriptAction = menu.addAction(i18nc("@action:inmenu", "New Script"));
    newScriptAction->setIcon(QIcon::fromTheme(u"document-new"_s));
    connect(newScriptAction, &QAction::triggered, this, &SieveScriptListBox::slotNew);

    if (mSieveListScript->itemAt(pos)) {
        QAction *renameScriptAction = menu.addAction(i18nc("@action:inmenu", "Rename Script"));
        renameScriptAction->setIcon(QIcon::fromTheme(u"edit-rename"_s));
        connect(renameScriptAction, &QAction::triggered, this, &SieveScriptListBox::slotRename);

        menu.addSeparator();
        QAction *editScriptDescriptionAction = menu.addAction(i18nc("@action:inmenu", "Edit Script Description"));
        editScriptDescriptionAction->setIcon(QIcon::fromTheme(u"edit-comment"_s));
        connect(editScriptDescriptionAction, &QAction::triggered, this, &SieveScriptListBox::slotEditDescription);

        menu.addSeparator();
        QAction *deleteScriptAction = menu.addAction(i18nc("@action:inmenu", "Delete Script"));
        deleteScriptAction->setIcon(QIcon::fromTheme(u"edit-delete"_s));
        connect(deleteScriptAction, &QAction::triggered, this, &SieveScriptListBox::slotDelete);
    }
    menu.exec(QCursor::pos());
}

void SieveScriptListBox::setSieveEditorGraphicalModeWidget(SieveEditorGraphicalModeWidget *graphicalModeWidget)
{
    mSieveGraphicalModeWidget = graphicalModeWidget;
}

void SieveScriptListBox::slotItemClicked(QListWidgetItem *item)
{
    if (item) {
        auto itemScript = static_cast<SieveScriptListItem *>(item);
        Q_EMIT activatePage(itemScript->scriptPage());
    }
}

void SieveScriptListBox::updateButtons()
{
    const int currentIndex = mSieveListScript->currentRow();
    const bool theFirst = (currentIndex == 0);
    const int numberOfElement(mSieveListScript->count());
    const bool theLast = (currentIndex >= numberOfElement - 1);

    const QList<QListWidgetItem *> lst = mSieveListScript->selectedItems();
    mBtnDelete->setEnabled(!lst.isEmpty());
    mBtnRename->setEnabled(lst.count() == 1);
    mBtnDescription->setEnabled(lst.count() == 1);
    mBtnBottom->setEnabled(!lst.isEmpty() && !theLast);
    mBtnTop->setEnabled(!lst.isEmpty() && !theFirst);
    mBtnDown->setEnabled(!lst.isEmpty() && !theLast);
    mBtnUp->setEnabled(!lst.isEmpty() && !theFirst);
}

SieveScriptPage *SieveScriptListBox::createNewScript(const QString &newName, const QString &description)
{
    auto item = new SieveScriptListItem(newName, mSieveListScript);
    item->setDescription(description);
    auto page = new SieveScriptPage(mSieveGraphicalModeWidget);
    page->setListOfIncludeFile(mSieveGraphicalModeWidget->listOfIncludeFile());
    item->setScriptPage(page);
    Q_EMIT addNewPage(page);
    Q_EMIT enableButtonOk(true);
    mSieveListScript->setCurrentItem(item);
    updateButtons();
    return page;
}

void SieveScriptListBox::slotNew()
{
    bool ok;
    const QString newName = QInputDialog::getText(this, i18nc("@title:window", "New Script"), i18n("New script name:"), {}, {}, &ok);
    if (!newName.trimmed().isEmpty() && ok) {
        createNewScript(newName);
        Q_EMIT valueChanged();
    }
}

void SieveScriptListBox::slotDelete()
{
    QListWidgetItem *item = mSieveListScript->currentItem();
    if (item) {
        const int answer = KMessageBox::warningTwoActions(this,
                                                          i18n("Do you want to delete \"%1\" script?", item->text()),
                                                          i18nc("@title:window", "Delete Script"),
                                                          KStandardGuiItem::del(),
                                                          KStandardGuiItem::cancel());
        if (answer == KMessageBox::ButtonCode::PrimaryAction) {
            auto itemScript = static_cast<SieveScriptListItem *>(item);
            Q_EMIT removePage(itemScript->scriptPage());
            delete item;
            Q_EMIT enableButtonOk(mSieveListScript->count() > 0);
            updateButtons();
            Q_EMIT valueChanged();
        }
    }
}

void SieveScriptListBox::slotRename()
{
    QListWidgetItem *item = mSieveListScript->currentItem();
    if (item) {
        bool ok = false;
        QString newName =
            QInputDialog::getText(this, i18nc("@title:window", "Rename Script"), i18n("New name for the script:"), QLineEdit::Normal, item->text(), &ok);
        if (ok) {
            newName = newName.trimmed();
            if (!newName.isEmpty()) {
                item->setText(newName);
                Q_EMIT valueChanged();
            }
        }
    }
}

void SieveScriptListBox::slotEditDescription()
{
    QListWidgetItem *item = mSieveListScript->currentItem();
    if (item) {
        auto sieveItem = static_cast<SieveScriptListItem *>(item);
        QPointer<SieveScriptDescriptionDialog> dlg = new SieveScriptDescriptionDialog(this);
        dlg->setDescription(sieveItem->description());
        if (dlg->exec()) {
            sieveItem->setDescription(dlg->description());
            Q_EMIT valueChanged();
        }
        delete dlg;
    }
}

void SieveScriptListBox::slotTop()
{
    QListWidgetItem *item = mSieveListScript->currentItem();
    if (item) {
        const int currentIndex = mSieveListScript->currentRow();
        if (currentIndex != 0) {
            item = mSieveListScript->takeItem(currentIndex);
            mSieveListScript->insertItem(0, item);
            mSieveListScript->setCurrentItem(item);
            Q_EMIT valueChanged();
        }
    }
}

void SieveScriptListBox::slotBottom()
{
    QListWidgetItem *item = mSieveListScript->currentItem();
    if (item) {
        const int currentIndex = mSieveListScript->currentRow();
        if (currentIndex != mSieveListScript->count() - 1) {
            item = mSieveListScript->takeItem(currentIndex);
            mSieveListScript->insertItem(mSieveListScript->count(), item);
            mSieveListScript->setCurrentItem(item);
            Q_EMIT valueChanged();
        }
    }
}

void SieveScriptListBox::slotDown()
{
    QListWidgetItem *item = mSieveListScript->currentItem();
    if (item) {
        const int currentIndex = mSieveListScript->currentRow();
        if (currentIndex < mSieveListScript->count() - 1) {
            item = mSieveListScript->takeItem(currentIndex);
            mSieveListScript->insertItem(currentIndex + 1, item);
            mSieveListScript->setCurrentItem(item);
            Q_EMIT valueChanged();
        }
    }
}

void SieveScriptListBox::slotUp()
{
    QListWidgetItem *item = mSieveListScript->currentItem();
    if (item) {
        const int currentIndex = mSieveListScript->currentRow();
        if (currentIndex >= 1) {
            item = mSieveListScript->takeItem(currentIndex);
            mSieveListScript->insertItem(currentIndex - 1, item);
            mSieveListScript->setCurrentItem(item);
            Q_EMIT valueChanged();
        }
    }
}

QString SieveScriptListBox::generatedScript(QStringList &requireModules) const
{
    QString resultScript;
    QStringList lstRequires;
    const int numberOfScripts(mSieveListScript->count());
    for (int i = 0; i < numberOfScripts; ++i) {
        auto item = static_cast<SieveScriptListItem *>(mSieveListScript->item(i));
        if (i != 0) {
            resultScript += QLatin1StringView("\n\n");
        }
        resultScript += u'#' + u"%1[%2]"_s.arg(roundcubeRuleName(), item->text()) + u'\n';
        resultScript += item->generatedScript(lstRequires);
    }
    if (!resultScript.isEmpty()) {
        resultScript += u'\n';
    }

    QStringList endRequires;
    for (const QString &r : std::as_const(lstRequires)) {
        if (!endRequires.contains(r)) {
            endRequires.append(r);
            requireModules += u"require \"%1\";"_s.arg(r);
        }
    }

    return resultScript;
}

void SieveScriptListBox::clear()
{
    mScriptNumber = 0;
    Q_EMIT enableButtonOk(false);
    // Clear tabpage
    mSieveListScript->clear();
    updateButtons();
}

void SieveScriptListBox::loadScript(const QString &doc, QString &error)
{
    clear();
    QXmlStreamReader streamReader(doc);
    if (streamReader.readNextStartElement()) {
        if (streamReader.name() == QLatin1StringView("script")) {
            SieveScriptPage *currentPage = nullptr;
            ParseSieveScriptTypeBlock typeBlock = TypeUnknown;
            loadBlock(streamReader, currentPage, typeBlock, error);
        }
    }
}

void SieveScriptListBox::loadBlock(QXmlStreamReader &n, SieveScriptPage *currentPage, ParseSieveScriptTypeBlock typeBlock, QString &error)
{
    QString scriptName;
    QString comment;
    bool hasCreatedAIfBlock = false;
    bool previousElementWasAComment = false;
    while (n.readNextStartElement()) {
        const QStringView tagName = n.name();
        // qDebug() <<"SieveScriptListBox::loadBlock tagName " << tagName;
        if (tagName == QLatin1StringView("control")) {
            previousElementWasAComment = false;
            // Create a new page when before it was "onlyactions"
            if (typeBlock == TypeBlockAction) {
                currentPage = nullptr;
            }
            if (n.attributes().hasAttribute(QLatin1StringView("name"))) {
                const QString controlType = n.attributes().value(QLatin1StringView("name")).toString();
                // qCDebug(LIBKSIEVEUI_LOG)<<" controlType"<<controlType;
                if (controlType == QLatin1StringView("if")) {
                    typeBlock = TypeBlockIf;
                    if (!currentPage || hasCreatedAIfBlock) {
                        currentPage = createNewScript(scriptName.isEmpty() ? createUniqName() : scriptName, comment);
                    }
                    hasCreatedAIfBlock = true;
                    comment.clear();
                    currentPage->blockIfWidget()->loadScript(n, false, error);
                } else if (controlType == QLatin1StringView("elsif")) {
                    typeBlock = TypeBlockElsif;
                    if (!currentPage) {
                        qCDebug(LIBKSIEVEUI_LOG) << " script is not correct missing if block";
                        currentPage = createNewScript(scriptName.isEmpty() ? createUniqName() : scriptName, comment);
                    }
                    SieveScriptBlockWidget *blockWidget = currentPage->addScriptBlock(KSieveUi::SieveWidgetPageAbstract::BlockElsIf);
                    if (blockWidget) {
                        blockWidget->loadScript(n, false, error);
                    }
                } else if (controlType == QLatin1StringView("else")) {
                    typeBlock = TypeBlockElse;
                    if (!currentPage) {
                        currentPage = createNewScript(scriptName.isEmpty() ? createUniqName() : scriptName, comment);
                        qCDebug(LIBKSIEVEUI_LOG) << " script is not correct missing if block";
                    }
                    SieveScriptBlockWidget *blockWidget = currentPage->addScriptBlock(KSieveUi::SieveWidgetPageAbstract::BlockElse);
                    if (blockWidget) {
                        blockWidget->loadScript(n, false, error);
                    }
                    // We are sure that we can't have another elsif
                    currentPage = nullptr;
                } else if (controlType == QLatin1StringView("foreverypart")) {
                    typeBlock = TypeBlockForeachBlock;
                    if (!currentPage) {
                        currentPage = createNewScript(scriptName.isEmpty() ? createUniqName() : scriptName, comment);
                        comment.clear();
                    }
                    if (currentPage->forEveryPartWidget()) {
                        currentPage->forEveryPartWidget()->loadScript(n, error);
                    } else {
                        error += i18n("forEveryPart is not supported by your server") + u'\n';
                    }
                    // TODO verify it.
#ifdef QDOMELEMENT_FIXME
                    QDomNode block = e.firstChildElement(u"block"_s).firstChild();
                    loadBlock(block, currentPage, typeBlock, error);
#endif
                } else if (controlType == QLatin1StringView("require")) {
                    n.skipCurrentElement();
                    // Nothing. autogenerated
                } else {
                    qCDebug(LIBKSIEVEUI_LOG) << " unknown controlType :" << controlType;
                }
            }
        } else if (tagName == QLatin1StringView("comment")) {
            previousElementWasAComment = true;
#ifdef FIXME_COMMENT
            if (e.hasAttribute(u"hash"_s)) {
                // TODO
            } else if (e.hasAttribute(u"bracket"_s)) {
                // TODO
            }
#endif
            QString str(n.readElementText());
            if (str.contains(defaultScriptName())) {
                scriptName = str.remove(defaultScriptName());
            } else if (str.contains(roundcubeRuleName())) {
                scriptName = str.remove(roundcubeRuleName());
                if (scriptName.startsWith(u'[') && scriptName.endsWith(u']')) {
                    scriptName.removeAt(0);
                    scriptName.removeAt(scriptName.length() - 1);
                }
            } else {
                if (!comment.isEmpty()) {
                    comment += u'\n';
                }
                comment += str;
            }
            // qDebug() << " COMMENT " << comment;
        } else if (tagName == QLatin1StringView("action")) {
            previousElementWasAComment = false;
            if (n.attributes().hasAttribute(QLatin1StringView("name"))) {
                const QString actionName = n.attributes().value(QLatin1StringView("name")).toString();
                if (actionName == QLatin1StringView("include")) {
                    if (!currentPage || (typeBlock == TypeBlockIf) || (typeBlock == TypeBlockElse) || (typeBlock == TypeBlockElsif)) {
                        currentPage = createNewScript(scriptName.isEmpty() ? createUniqName() : scriptName, comment);
                        comment.clear();
                    }
                    typeBlock = TypeBlockInclude;
                    if (currentPage->includeWidget()) {
                        currentPage->includeWidget()->loadScript(n, error);
                    } else {
                        qCDebug(LIBKSIEVEUI_LOG) << " include not supported";
                    }
                } else if (actionName == QLatin1StringView("global")) {
                    if (!currentPage || (typeBlock == TypeBlockIf) || (typeBlock == TypeBlockElse) || (typeBlock == TypeBlockElsif)) {
                        currentPage = createNewScript(scriptName.isEmpty() ? createUniqName() : scriptName, comment);
                        comment.clear();
                    }
                    typeBlock = TypeBlockGlobal;
                    if (currentPage->globalVariableWidget()) {
                        currentPage->globalVariableWidget()->loadScript(n, error);
                    } else {
                        qCDebug(LIBKSIEVEUI_LOG) << " globalVariable not supported";
                    }
                } else if (actionName == QLatin1StringView("set") && (typeBlock == TypeBlockGlobal)) {
                    // FIXME global not global variable.
                    if (currentPage->globalVariableWidget()) {
                        const SieveGlobalVariableActionWidget::VariableElement var = currentPage->globalVariableWidget()->loadSetVariable(n, error);
                        if (var.isValid()) {
                            qCDebug(LIBKSIEVEUI_LOG) << "It's not a global variable";
                            if ((typeBlock == TypeBlockIf) || (typeBlock == TypeBlockElse) || (typeBlock == TypeBlockElsif)) {
                                currentPage = createNewScript(scriptName.isEmpty() ? createUniqName() : scriptName, comment);
                            }
                            typeBlock = TypeBlockAction;
                            comment.clear();
                            currentPage->blockIfWidget()->loadLocalVariable(var);
                        }
                    } else {
                        qCDebug(LIBKSIEVEUI_LOG) << " set not supported";
                    }
                } else {
                    if (!currentPage || (typeBlock == TypeBlockIf) || (typeBlock == TypeBlockElse) || (typeBlock == TypeBlockElsif)) {
                        currentPage = createNewScript(scriptName.isEmpty() ? createUniqName() : scriptName, comment);
                    }
                    typeBlock = TypeBlockAction;
                    comment.clear();
                    currentPage->blockIfWidget()->loadScript(n, true, error);
                }
            }
        } else if (tagName == QLatin1StringView("crlf")) {
            // If it was a comment previously you will create a \n
            if (previousElementWasAComment) {
                comment += u'\n';
            }
            n.skipCurrentElement();
        } else {
            qCDebug(LIBKSIEVEUI_LOG) << " unknown tagname" << tagName;
        }
    }
}

QString SieveScriptListBox::createUniqName()
{
    const QString pattern = i18n("Script part %1", mScriptNumber);
    ++mScriptNumber;
    return pattern;
}

#include "moc_sievescriptlistbox.cpp"
