/*
  SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef VACATIONPAGEWIDGET_H
#define VACATIONPAGEWIDGET_H

#include <QWidget>
#include <QUrl>
class QStackedWidget;
namespace KManageSieve {
}

namespace KSieveUi {
class VacationEditWidget;
class VacationWarningWidget;
class VacationCreateScriptJob;
class MultiImapVacationManager;
class SieveImapAccountSettings;
class VacationPageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VacationPageWidget(QWidget *parent = nullptr);
    ~VacationPageWidget();

    void setServerUrl(const QUrl &url);
    void setServerName(const QString &serverName);
    Q_REQUIRED_RESULT KSieveUi::VacationCreateScriptJob *writeScript(bool &errorFound);
    void setDefault();
    void setVacationManager(MultiImapVacationManager *vacationManager);
    void setSieveImapAccountSettings(const KSieveUi::SieveImapAccountSettings &account);

    bool wasChanged() const;
private Q_SLOTS:
    void slotGetResult(const QString &serverName, const QStringList &sieveCapabilities, const QString &scriptName, const QString &script, bool active);

private:

    enum PageType {
        Script = 0,
        ScriptNotSupported = 1
    };

    QString mServerName;
    QUrl mUrl;
    QStackedWidget *mStackWidget = nullptr;
    VacationEditWidget *mVacationEditWidget = nullptr;
    VacationWarningWidget *mVacationWarningWidget = nullptr;
    MultiImapVacationManager *mVacationManager = nullptr;
    PageType mPageScript = Script;
    bool mWasActive = false;
    bool mHasDateSupport = false;
};
}

#endif // VACATIONPAGEWIDGET_H
