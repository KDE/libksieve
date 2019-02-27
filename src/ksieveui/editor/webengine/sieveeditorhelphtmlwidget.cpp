/*
   Copyright (C) 2013-2019 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#include "sieveeditorhelphtmlwidget.h"
#include "sieveeditorloadprogressindicator.h"

#include "sieveeditorwebengineview.h"

#include <QVBoxLayout>

#include <kpimtextedit/slidecontainer.h>

#include "findbar/findbarwebengineview.h"

using namespace KSieveUi;
namespace {
qreal zoomBy()
{
    return 20;
}
}
SieveEditorHelpHtmlWidget::SieveEditorHelpHtmlWidget(QWidget *parent)
    : QWidget(parent)
    , mZoomFactor(100)
{
    QVBoxLayout *lay = new QVBoxLayout(this);
    lay->setContentsMargins(0, 0, 0, 0);
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

SieveEditorHelpHtmlWidget::~SieveEditorHelpHtmlWidget()
{
}

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
