/*
  SPDX-FileCopyrightText: 2013-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#pragma once

#include <QUrl>
#include <QWidget>
class QStackedWidget;
namespace KManageSieve
{
}

namespace KSieveCore
{
class SieveImapAccountSettings;
class VacationCreateScriptJob;
class MultiImapVacationManager;
}
namespace KSieveUi
{
class VacationEditWidget;
class VacationWarningWidget;
class VacationPageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VacationPageWidget(QWidget *parent = nullptr);
    ~VacationPageWidget() override;

    void setServerUrl(const QUrl &url);
    void setServerName(const QString &serverName);
    [[nodiscard]] KSieveCore::VacationCreateScriptJob *writeScript(bool &errorFound);
    void setDefault();
    void setVacationManager(KSieveCore::MultiImapVacationManager *vacationManager);
    void setSieveImapAccountSettings(const KSieveCore::SieveImapAccountSettings &account);

    bool wasChanged() const;
private Q_SLOTS:
    void slotGetResult(const QString &serverName, const QStringList &sieveCapabilities, const QString &scriptName, const QString &script, bool active);

private:
    enum PageType : uint8_t {
        Script = 0,
        ScriptNotSupported = 1
    };

    QString mServerName;
    QUrl mUrl;
    QStackedWidget *mStackWidget = nullptr;
    VacationEditWidget *mVacationEditWidget = nullptr;
    VacationWarningWidget *mVacationWarningWidget = nullptr;
    KSieveCore::MultiImapVacationManager *mVacationManager = nullptr;
    PageType mPageScript = PageType::Script;
    bool mWasActive = false;
    bool mHasDateSupport = false;
};
}
