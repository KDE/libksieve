/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "parsingutil.h"
#include "xmlprintingscriptbuilder.h"
#include <ksieve/parser.h>
using KSieve::Parser;

#include <ksieve/error.h>
#include <ksieve/scriptbuilder.h>

#include "libksieve_debug.h"

using namespace KSieveUi;

QString ParsingUtil::parseScript(const QString &scriptStr, bool &result)
{
    const QByteArray script = scriptStr.toUtf8();

    KSieve::Parser parser(script.begin(), script.begin() + script.length());
    KSieveUi::XMLPrintingScriptBuilder psb;
    parser.setScriptBuilder(&psb);
    if (parser.parse()) {
        result = true;
        return psb.result();
    } else {
        qCDebug(LIBKSIEVE_LOG) << "Impossible to parse file";
        result = false;
    }
    return {};
}
