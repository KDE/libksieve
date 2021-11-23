/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveeditorhelphtmlwidget.h"
#include "sieveeditorloadprogressindicator.h"

#include "sieveeditorwebengineview.h"

#include <QVBoxLayout>

#include <KPIMTextEdit/SlideContainer>

#include "findbar/findbarwebengineview.h"

using namespace KSieveUi;
namespace
{
qreal zoomBy()
{
    return 20;
}
}
SieveEditorHelpHtmlWidget::SieveEditorHelpHtmlWidget(QWidget *parent)
    : QWidget(parent)
{
    auto lay = new QVBoxLayout(this);
    lay->setContentsMargins({});
    mProgressIndicator = new SieveEditorLoadProgressIndicator(this);
    mProgressIndicator->setObjectName(QStringLiteral("progressindicator"));
    connect(mProgressIndicator, &SieveEditorLoadProgressIndicator::pixmapChanged, this, &SieveEditorHelpHtmlWidget::slotPixmapChanged);
    connect(mProgressIndicator, &SieveEditorLoadProgressIndicator::loadFinished, this, &SieveEditorHelpHtmlWidget::slotLoadFinished);

    mWebView = new SieveEditorWebEngineView(this);
    mWebView->setObjectName(QStringLiteral("webview"));
    connect(mWebView, &SieveEditorWebEngineView::titleChanged, this, &SieveEditorHelpHtmlWidget::slotTitleChanged);
    connect(mWebView, &SieveEditorWebEngineView::loadStarted, this, &SieveEditorHelpHtmlWidget::slotLoadStarted);
    connect(mWebView, &SieveEditorWebEngineView::loadFinished, this, &SieveEditorHelpHtmlWidget::slotFinished);
    connect(mWebView, &SieveEditorWebEngineView::selectionChanged, this, &SieveEditorHelpHtmlWidget::slotSelectionChanged);
    lay->addWidget(mWebView);

    mSliderContainer = new KPIMTextEdit::SlideContainer(this);
    mSliderContainer->setObjectName(QStringLiteral("slidercontainer"));
    lay->addWidget(mSliderContainer);
    mFindBar = new KSieveUi::FindBarWebEngineView(mWebView, this);
    mFindBar->setObjectName(QStringLiteral("findbar"));
    connect(mFindBar, &KSieveUi::FindBarWebEngineView::hideFindBar, mSliderContainer, &KPIMTextEdit::SlideContainer::slideOut);
    mSliderContainer->setContent(mFindBar);
}

SieveEditorHelpHtmlWidget::~SieveEditorHelpHtmlWidget() = default;

QString SieveEditorHelpHtmlWidget::title() const
{
    return mTitle;
}

void SieveEditorHelpHtmlWidget::slotLoadFinished(bool success)
{
    Q_EMIT loadFinished(this, success);
}

QUrl SieveEditorHelpHtmlWidget::currentUrl() const
{
    return mWebView->url();
}

void SieveEditorHelpHtmlWidget::slotPixmapChanged(const QPixmap &pixmap)
{
    Q_EMIT progressIndicatorPixmapChanged(this, pixmap);
}

void SieveEditorHelpHtmlWidget::slotFinished(bool b)
{
    mProgressIndicator->stopAnimation(b);
}

void SieveEditorHelpHtmlWidget::slotLoadStarted()
{
    mProgressIndicator->startAnimation();
}

void SieveEditorHelpHtmlWidget::slotTitleChanged(const QString &title)
{
    if (mTitle != title) {
        mTitle = title;
        Q_EMIT titleChanged(this, title);
    }
}

void SieveEditorHelpHtmlWidget::openUrl(const QUrl &url)
{
    mWebView->setUrl(url);
}

void SieveEditorHelpHtmlWidget::zoomIn()
{
    if (mZoomFactor >= 300) {
        return;
    }
    mZoomFactor += zoomBy();
    if (mZoomFactor > 300) {
        mZoomFactor = 300;
    }
    mWebView->setZoomFactor(mZoomFactor / 100.0);
}

void SieveEditorHelpHtmlWidget::copy()
{
    mWebView->triggerPageAction(QWebEnginePage::Copy);
}

bool SieveEditorHelpHtmlWidget::hasSelection() const
{
    return mWebView->hasSelection();
}

void SieveEditorHelpHtmlWidget::selectAll()
{
    mWebView->triggerPageAction(QWebEnginePage::SelectAll);
}

void SieveEditorHelpHtmlWidget::zoomOut()
{
    if (mZoomFactor <= 10) {
        return;
    }
    mZoomFactor -= zoomBy();
    if (mZoomFactor < 10) {
        mZoomFactor = 10;
    }
    mWebView->setZoomFactor(mZoomFactor / 100.0);
}

void SieveEditorHelpHtmlWidget::resetZoom()
{
    mZoomFactor = 100;
    mWebView->setZoomFactor(1.0);
}

void SieveEditorHelpHtmlWidget::slotSelectionChanged()
{
    Q_EMIT copyAvailable(mWebView->hasSelection());
}

void SieveEditorHelpHtmlWidget::find()
{
    if (mWebView->hasSelection()) {
        mFindBar->setText(mWebView->selectedText());
    }
    mSliderContainer->slideIn();
    mFindBar->focusAndSetCursor();
}
