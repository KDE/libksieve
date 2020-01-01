/*
   Copyright (C) 2013-2020 Laurent Montel <montel@kde.org>

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
#include "sieveconditionlist.h"
#include "sievecondition.h"
#include "sieveconditionheader.h"
#include "sieveconditionaddress.h"
#include "sieveconditionsize.h"
#include "sieveconditionenvelope.h"
#include "sieveconditionexists.h"
#include "sieveconditiontrue.h"
#include "sieveconditionfalse.h"
//RFC5173 (extension)
#include "sieveconditionbody.h"
//rfc5260
#include "sieveconditiondate.h"
#include "sieveconditioncurrentdate.h"

#include "sieveconditionmailboxexists.h"

#include "sieveconditionspamtest.h"
#include "sieveconditionvirustest.h"
#include "sieveconditionihave.h"
#include "sieveconditionenvironment.h"

#include "sieveconditionhasflag.h"
#include "sieveconditionmetadata.h"
#include "sieveconditionconvert.h"
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
