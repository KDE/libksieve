/*
   Copyright (C) 2013-2016 Laurent Montel <montel@kde.org>

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

#include "parsingutil.h"
#include "xmlprintingscriptbuilder.h"
#include <ksieve/parser.h>
using KSieve::Parser;

#include <ksieve/error.h>
#include <ksieve/scriptbuilder.h>

#include "libksieve_debug.h"

using namespace KSieveUi;

QDomDocument ParsingUtil::parseScript(const QString &scriptStr, bool &result)
{
    const QByteArray script = scriptStr.toUtf8();

    KSieve::Parser parser(script.begin(), script.begin() + script.length());
    KSieveUi::XMLPrintingScriptBuilder psb;
    parser.setScriptBuilder(&psb);
    if (parser.parse()) {
        result = true;
        return psb.toDom();
    } else {
        qCDebug(LIBKSIEVE_LOG) << "cannot parse file";
        result = false;
    }
    return QDomDocument();
}
