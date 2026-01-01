/*
  SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "sievetexteditwidget.h"
using namespace Qt::Literals::StringLiterals;

#include "editor/sievetextedit.h"
#include <QVBoxLayout>

#include <TextAddonsWidgets/SlideContainer>
#include <TextCustomEditor/PlainTextEditFindBar>
using namespace KSieveUi;

class KSieveUi::SieveTextEditWidgetPrivate
{
public:
    SieveTextEditWidgetPrivate() = default;

    KSieveUi::SieveTextEdit *mTextEdit = nullptr;
    TextAddonsWidgets::SlideContainer *mSliderContainer = nullptr;
    TextCustomEditor::PlainTextEditFindBar *mFindBar = nullptr;
};

SieveTextEditWidget::SieveTextEditWidget(KSieveUi::SieveTextEdit *customTextEdit, QWidget *parent)
    : QWidget(parent)
    , d(new KSieveUi::SieveTextEditWidgetPrivate)
{
    initialize(customTextEdit);
}

SieveTextEditWidget::SieveTextEditWidget(QWidget *parent)
    : QWidget(parent)
    , d(new KSieveUi::SieveTextEditWidgetPrivate)
{
    initialize();
}

SieveTextEditWidget::~SieveTextEditWidget() = default;

void SieveTextEditWidget::initialize(KSieveUi::SieveTextEdit *custom)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    if (custom) {
        d->mTextEdit = custom;
    } else {
        d->mTextEdit = new KSieveUi::SieveTextEdit;
    }
    d->mTextEdit->setObjectName(QLatin1StringView("textedit"));
    d->mTextEdit->setShowHelpMenu(false);
    mainLayout->addWidget(d->mTextEdit);

    d->mSliderContainer = new TextAddonsWidgets::SlideContainer(this);
    d->mSliderContainer->setObjectName(QLatin1StringView("slidercontainer"));
    d->mFindBar = new TextCustomEditor::PlainTextEditFindBar(d->mTextEdit, this);
    d->mFindBar->setObjectName(QLatin1StringView("findbar"));
    d->mFindBar->setHideWhenClose(false);
    connect(d->mFindBar, &TextCustomEditor::TextEditFindBarBase::hideFindBar, d->mSliderContainer, &TextAddonsWidgets::SlideContainer::slideOut);
    d->mSliderContainer->setContent(d->mFindBar);
    mainLayout->addWidget(d->mSliderContainer);
    connect(d->mTextEdit, &SieveTextEdit::findText, this, &SieveTextEditWidget::slotFind);
    connect(d->mTextEdit, &SieveTextEdit::replaceText, this, &SieveTextEditWidget::slotReplace);
}

void SieveTextEditWidget::setReadOnly(bool readOnly)
{
    d->mTextEdit->setReadOnly(readOnly);
}

void SieveTextEditWidget::slotReplace()
{
    if (d->mTextEdit->textCursor().hasSelection()) {
        d->mFindBar->setText(d->mTextEdit->textCursor().selectedText());
    }
    d->mFindBar->showReplace();
    d->mSliderContainer->slideIn();
    d->mFindBar->focusAndSetCursor();
}

void SieveTextEditWidget::slotFind()
{
    if (d->mTextEdit->textCursor().hasSelection()) {
        d->mFindBar->setText(d->mTextEdit->textCursor().selectedText());
    }
    d->mTextEdit->moveCursor(QTextCursor::Start);
    d->mFindBar->showFind();
    d->mSliderContainer->slideIn();
    d->mFindBar->focusAndSetCursor();
}

SieveTextEdit *SieveTextEditWidget::textEdit() const
{
    return d->mTextEdit;
}

#include "moc_sievetexteditwidget.cpp"
