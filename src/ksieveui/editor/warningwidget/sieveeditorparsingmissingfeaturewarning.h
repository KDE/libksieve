/*
   Copyright (C) 2013-2018 Laurent Montel <montel@kde.org>

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
#ifndef SIEVEEDITORPARSINGMISSINGFEATUREWARNING_H
#define SIEVEEDITORPARSINGMISSINGFEATUREWARNING_H

#include <KMessageWidget>
#include "ksieveui_private_export.h"
namespace KSieveUi {
class KSIEVEUI_TESTS_EXPORT SieveEditorParsingMissingFeatureWarning : public KMessageWidget
{
    Q_OBJECT
public:
    enum TextEditorType {
        TextEditor,
        GraphicEditor
    };

    explicit SieveEditorParsingMissingFeatureWarning(SieveEditorParsingMissingFeatureWarning::TextEditorType type, QWidget *parent = nullptr);
    ~SieveEditorParsingMissingFeatureWarning();

    void setErrors(const QString &initialScript, const QString &errors);
    Q_REQUIRED_RESULT QString initialScript() const;

Q_SIGNALS:
    void switchToGraphicalMode();
    void switchToTextMode();

private:
    void slotSwitchInGraphicalMode();
    void slotSwitchInTextMode();
    void slotInActualMode();
    void slotShowDetails(const QString &content);
    QString mErrors;
    QString mScript;
};
}

#endif // SIEVEEDITORPARSINGMISSINGFEATUREWARNING_H
