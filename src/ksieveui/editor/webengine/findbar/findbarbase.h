/* SPDX-FileCopyrightText: 2010 Torgny Nyblom <nyblom@kde.org>
 * SPDX-FileCopyrightText: 2010-2021 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#ifndef FINDBARBASE_H
#define FINDBARBASE_H

#include <QWidget>
#include "ksieveui_export.h"
class QAction;
class QPushButton;
class QMenu;
class QLabel;
namespace PimCommon {
class LineEditWithCompleterNg;
}
namespace KSieveUi {
class KSIEVEUI_EXPORT FindBarBase : public QWidget
{
    Q_OBJECT
public:
    explicit FindBarBase(QWidget *parent = nullptr);
    ~FindBarBase() override;

    Q_REQUIRED_RESULT QString text() const;
    void setText(const QString &text);
    void focusAndSetCursor();

protected:
    bool event(QEvent *e) override;
    virtual void clearSelections();
    virtual void updateHighLight(bool);
    virtual void searchText(bool backward, bool isAutoSearch);
    virtual void updateSensitivity(bool);

    void setFoundMatch(bool match);
    QMenu *optionsMenu();

public Q_SLOTS:
    void findNext();
    void findPrev();
    void autoSearch(const QString &str);
    void slotSearchText(bool backward = false, bool isAutoSearch = true);
    void closeBar();

Q_SIGNALS:
    void hideFindBar();

protected Q_SLOTS:
    void caseSensitivityChanged(bool);
    void slotHighlightAllChanged(bool);

protected:
    QString mNotFoundString;
    QString mPositiveBackground;
    QString mNegativeBackground;
    QString mLastSearchStr;
    PimCommon::LineEditWithCompleterNg *mSearch = nullptr;
    QAction *mCaseSensitiveAct = nullptr;

    QPushButton *mFindPrevBtn = nullptr;
    QPushButton *mFindNextBtn = nullptr;
    QMenu *mOptionsMenu = nullptr;
    QLabel *mStatus = nullptr;
    void addToCompletion(const QString &text);
};
}

#endif /* FINDBARBASE_H */
