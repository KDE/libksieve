/*
 * SPDX-FileCopyrightText: 2015 Sandro Knauß <knauss@kolabsys.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#ifndef VACATIONUTILSTEST_H
#define VACATIONUTILSTEST_H

#include <QObject>

namespace KSieveUi {
class VacationUtilsTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testParseEmptyScript();
    void testParseOnlyComment();
    void testParseActivate_data();
    void testParseActivate();
    void testParseScript_data();
    void testParseScript();
    void testParseScriptComplex();
    void testParseScriptComplexTime();
    void testMailAction_data();
    void testMailAction();
    void testWriteScript();
    void testWriteSimpleScript();
    void testUpdateVacationBlock();
    void testMergeRequireLine();
    void testDisableGeneratedScript();
};
}
#endif // VACATIONUTILSTEST_H
