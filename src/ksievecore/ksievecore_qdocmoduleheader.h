/*
  SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

// Module header for qdoc PCH
#include "ksievecore_private_export.h"
#include "managescriptsjob/checkkolabkep14supportjob.h"
#include "managescriptsjob/checkscriptjob.h"
#include "managescriptsjob/generateglobalscriptjob.h"
#include "managescriptsjob/parseuserscriptjob.h"
#include "managescriptsjob/renamescriptjob.h"
#include "scriptsparsing/parsingutil.h"
#include "scriptsparsing/xmlprintingscriptbuilder.h"
#include "sieveimapinstance/sieveimapinstance.h"
#include "sieveimapinstance/sieveimapinstanceinterface.h"
#include "sieveimapinstance/sieveimapinstanceinterfacemanager.h"
#include "util/abstractakonadiimapsettinginterface.h"
#include "util/akonadiimapsettinginterface.h"
#include "util/findaccountinfojob.h"
#include "util/sieveimapaccountsettings.h"
#include "util/sieveimappasswordprovider.h"
#include "util/util.h"
#include "vacation/multiimapvacationmanager.h"
#include "vacation/searchserverwithvacationsupportjob.h"
#include "vacation/vacationcheckjob.h"
#include "vacation/vacationcreatescriptjob.h"
#include "vacation/vacationscriptextractor.h"
#include "vacation/vacationutils.h"
