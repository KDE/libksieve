/*
   SPDX-FileCopyrightText: 2013-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "ksieveui_private_export.h"
#include <KMessageWidget>
namespace KSieveUi
{
class KSIEVEUI_TESTS_EXPORT SieveEditorParsingMissingFeatureWarning : public KMessageWidget
{
    Q_OBJECT
public:
    enum TextEditorType {
        TextEditor,
        GraphicEditor
    };

    explicit SieveEditorParsingMissingFeatureWarning(SieveEditorParsingMissingFeatureWarning::TextEditorType type, QWidget *parent = nullptr);
    ~SieveEditorParsingMissingFeatureWarning() override;

    void setErrors(const QString &initialScript, const QString &errors);
    [[nodiscard]] QString initialScript() const;

Q_SIGNALS:
    void switchToGraphicalMode();
    void switchToTextMode();

private:
    void slotSwitchInGraphicalMode();
    void slotSwitchInTextMode();
    void slotInActualMode();
    void slotShowDetails(const QString &content);
    QString mirrors;
    QString mScript;
};
}
