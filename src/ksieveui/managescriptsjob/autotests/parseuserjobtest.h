/*
  SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#ifndef PARSEUSERJOBTEST_H
#define PARSEUSERJOBTEST_H

#include <QObject>
namespace KSieveUi {
class ParseUserTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testParseEmptyUserJob();
    void testParseUserTwoActiveScriptJob();
    void testParseUserNoActiveScriptJob();
    void testParseUserDuplicateActiveScriptJob();
    void testParseUserErrorScriptJob();
};
}
#endif // PARSEUSERJOBTEST_H
