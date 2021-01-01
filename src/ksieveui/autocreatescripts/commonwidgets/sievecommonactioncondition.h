/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SIEVECOMMONACTIONCONDITION_H
#define SIEVECOMMONACTIONCONDITION_H

#include <QObject>
namespace KSieveUi {
class SieveImapAccountSettings;
class SieveEditorGraphicalModeWidget;
class SieveCommonActionCondition : public QObject
{
    Q_OBJECT
public:
    explicit SieveCommonActionCondition(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, const QString &name, const QString &label, QObject *parent = nullptr);
    ~SieveCommonActionCondition() override;

    Q_REQUIRED_RESULT QString name() const;
    Q_REQUIRED_RESULT QString label() const;

    Q_REQUIRED_RESULT virtual QString code(QWidget *parent) const;

    Q_REQUIRED_RESULT virtual QStringList needRequires(QWidget *parent) const;

    Q_REQUIRED_RESULT virtual bool needCheckIfServerHasCapability() const;

    Q_REQUIRED_RESULT virtual QString serverNeedsCapability() const;

    Q_REQUIRED_RESULT virtual QString help() const;
    Q_REQUIRED_RESULT virtual QUrl href() const;

    virtual void unknownTag(const QStringRef &tag, QString &error);
    virtual void unknownTagValue(const QString &tagValue, QString &error);
    virtual void tooManyArguments(const QStringRef &tagName, int index, int maxValue, QString &error);
    virtual void serverDoesNotSupportFeatures(const QString &feature, QString &error);

    Q_REQUIRED_RESULT QString comment() const;

    void setComment(const QString &comment);

    Q_REQUIRED_RESULT QString generateComment() const;

    virtual QWidget *createParamWidget(QWidget *parent) const;
protected:
    Q_REQUIRED_RESULT KSieveUi::SieveImapAccountSettings sieveImapAccountSettings() const;
    Q_REQUIRED_RESULT QStringList sieveCapabilities() const;
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

#endif // SIEVECOMMONACTIONCONDITION_H
