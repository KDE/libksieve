/*
  Copyright (C) 2016-2018 Laurent Montel <montel@kde.org>

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

#ifndef RENAMESCRIPTJOBTEST_H
#define RENAMESCRIPTJOBTEST_H

#include <QObject>

class RenameScriptJobTest : public QObject
{
    Q_OBJECT
public:
    explicit RenameScriptJobTest(QObject *parent = nullptr);
    ~RenameScriptJobTest();

private Q_SLOTS:
    void canNotStartByDefault();
    void shouldBeStart_data();
    void shouldBeStart();
};

#endif // RENAMESCRIPTJOBTEST_H
