/*
  Copyright (C) 2013-2017 Laurent Montel <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
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
    KSieveUi::VacationCreateScriptJob *writeScript();
    void setDefault();
    void setVacationManager(MultiImapVacationManager *vacationManager);
    void setSieveImapAccountSettings(const KSieveUi::SieveImapAccountSettings &account);

private Q_SLOTS:
    void slotGetResult(const QString &serverName, const QStringList &sieveCapabilities, const QString &scriptName, const QString &script, bool active);

private:

    void fillWithDefaults();

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
    PageType mPageScript;
    bool mWasActive = false;
    bool mHasDateSupport = false;
};
}

#endif // VACATIONPAGEWIDGET_H
