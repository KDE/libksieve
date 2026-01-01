/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "sieveconditionlist.h"
#include "sievecondition.h"
#include "sieveconditionaddress.h"
#include "sieveconditionenvelope.h"
#include "sieveconditionexists.h"
#include "sieveconditionfalse.h"
#include "sieveconditionheader.h"
#include "sieveconditionsize.h"
#include "sieveconditiontrue.h"
// RFC5173 (extension)
#include "sieveconditionbody.h"
// rfc5260
#include "sieveconditioncurrentdate.h"
#include "sieveconditiondate.h"

#include "sieveconditionmailboxexists.h"

#include "sieveconditionenvironment.h"
#include "sieveconditionihave.h"
#include "sieveconditionspamtest.h"
#include "sieveconditionvirustest.h"

#include "sieveconditionconvert.h"
#include "sieveconditionhasflag.h"
#include "sieveconditionmetadata.h"
#include "sieveconditionmetadataexists.h"
#include "sieveconditionservermetadata.h"
#include "sieveconditionservermetadataexists.h"

QList<KSieveUi::SieveCondition *> KSieveUi::SieveConditionList::conditionList(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget)
{
    QList<KSieveUi::SieveCondition *> list;
    list.append(new KSieveUi::SieveConditionHeader(sieveGraphicalModeWidget));
    list.append(new KSieveUi::SieveConditionAddress(sieveGraphicalModeWidget));
    list.append(new KSieveUi::SieveConditionSize(sieveGraphicalModeWidget));
    list.append(new KSieveUi::SieveConditionEnvelope(sieveGraphicalModeWidget));
    list.append(new KSieveUi::SieveConditionExists(sieveGraphicalModeWidget));
    list.append(new KSieveUi::SieveConditionTrue(sieveGraphicalModeWidget));
    list.append(new KSieveUi::SieveConditionFalse(sieveGraphicalModeWidget));
    list.append(new KSieveUi::SieveConditionBody(sieveGraphicalModeWidget));
    list.append(new KSieveUi::SieveConditionDate(sieveGraphicalModeWidget));
    list.append(new KSieveUi::SieveConditionCurrentDate(sieveGraphicalModeWidget));
    list.append(new KSieveUi::SieveConditionMailboxExists(sieveGraphicalModeWidget));
    list.append(new KSieveUi::SieveConditionSpamTest(sieveGraphicalModeWidget));
    list.append(new KSieveUi::SieveConditionVirusTest(sieveGraphicalModeWidget));
    list.append(new KSieveUi::SieveConditionIhave(sieveGraphicalModeWidget));
    list.append(new KSieveUi::SieveConditionEnvironment(sieveGraphicalModeWidget));
    list.append(new KSieveUi::SieveConditionHasFlag(sieveGraphicalModeWidget));
    list.append(new KSieveUi::SieveConditionMetaData(sieveGraphicalModeWidget));
    list.append(new KSieveUi::SieveConditionConvert(sieveGraphicalModeWidget));
    list.append(new KSieveUi::SieveConditionMetaDataExists(sieveGraphicalModeWidget));
    list.append(new KSieveUi::SieveConditionServerMetaData(sieveGraphicalModeWidget));
    list.append(new KSieveUi::SieveConditionServerMetaDataExists(sieveGraphicalModeWidget));
    return list;
}
