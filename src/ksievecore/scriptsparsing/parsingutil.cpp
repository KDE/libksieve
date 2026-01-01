/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "parsingutil.h"
#include "parser.h"
#include "xmlprintingscriptbuilder.h"
using KSieve::Parser;

#include "scriptbuilder.h"

#include "libksievecore_debug.h"

using namespace KSieveCore;

QString ParsingUtil::parseScript(const QString &scriptStr, bool &result)
{
    const QByteArray script = scriptStr.toUtf8();

    KSieve::Parser parser(script.begin(), script.begin() + script.length());
    KSieveCore::XMLPrintingScriptBuilder psb;
    parser.setScriptBuilder(&psb);
    if (parser.parse()) {
        result = true;
        return psb.result();
    } else {
        qCDebug(LIBKSIEVECORE_LOG) << "Impossible to parse file";
        result = false;
    }
    return {};
}
