/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>
namespace KSieveCore
{
class SieveImapAccountSettings;
}
namespace KSieveUi
{
class SieveEditorGraphicalModeWidget;
class SieveCommonActionCondition : public QObject
{
    Q_OBJECT
public:
    explicit SieveCommonActionCondition(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget,
                                        const QString &name,
                                        const QString &label,
                                        QObject *parent = nullptr);
    ~SieveCommonActionCondition() override;

    [[nodiscard]] QString name() const;
    [[nodiscard]] QString label() const;

    [[nodiscard]] virtual QString code(QWidget *parent) const;

    [[nodiscard]] virtual QStringList needRequires(QWidget *parent) const;

    [[nodiscard]] virtual bool needCheckIfServerHasCapability() const;

    [[nodiscard]] virtual QString serverNeedsCapability() const;

    [[nodiscard]] virtual QString help() const;
    [[nodiscard]] virtual QUrl href() const;

    virtual void unknownTagValue(const QString &tagValue, QString &error);
    virtual void unknownTag(const QStringView &tag, QString &error);
    virtual void tooManyArguments(const QStringView &tagName, int index, int maxValue, QString &error);
    virtual void serverDoesNotSupportFeatures(const QString &feature, QString &error);

    [[nodiscard]] QString comment() const;

    void setComment(const QString &comment);

    virtual QWidget *createParamWidget(QWidget *parent) const;

protected:
    [[nodiscard]] KSieveCore::SieveImapAccountSettings sieveImapAccountSettings() const;
    [[nodiscard]] QStringList sieveCapabilities() const;
    SieveEditorGraphicalModeWidget *const mSieveGraphicalModeWidget;

Q_SIGNALS:
    void valueChanged();

private:
    Q_DISABLE_COPY(SieveCommonActionCondition)
    const QString mName;
    const QString mLabel;
    QString mComment;
};
}
