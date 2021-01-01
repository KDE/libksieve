/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SIEVEDEFAULTTEMPLATE_H
#define SIEVEDEFAULTTEMPLATE_H

#include "sievetemplatewidget.h"

namespace KSieveUi {
namespace SieveDefaultTemplate {
Q_REQUIRED_RESULT QVector<PimCommon::defaultTemplate> defaultTemplates();
}
}

#endif // SIEVEDEFAULTTEMPLATE_H
