/*
   Copyright (C) 2013-2019 Laurent Montel <montel@kde.org>

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
#ifndef SIEVEACTION_H
#define SIEVEACTION_H

#include <QObject>
#include <QUrl>
class QXmlStreamReader;
namespace KSieveUi {
class SieveImapAccountSettings;
class SieveEditorGraphicalModeWidget;
class SieveAction : public QObject
{
    Q_OBJECT
public:
    SieveAction(SieveEditorGraphicalModeWidget *sieveGraphicalModeWidget, const QString &name, const QString &label, QObject *parent = nullptr);
    ~SieveAction() override;

    Q_REQUIRED_RESULT QString name() const;
    Q_REQUIRED_RESULT QString label() const;

    Q_REQUIRED_RESULT virtual QWidget *createParamWidget(QWidget *parent) const;

    Q_REQUIRED_RESULT virtual bool setParamWidgetValue(QXmlStreamReader &element, QWidget *parent, QString &error);

    Q_REQUIRED_RESULT virtual QString code(QWidget *) const;

    Q_REQUIRED_RESULT virtual QStringList needRequires(QWidget *parent) const;

    Q_REQUIRED_RESULT virtual bool needCheckIfServerHasCapability() const;

    Q_REQUIRED_RESULT virtual QString serverNeedsCapability() const;

    Q_REQUIRED_RESULT virtual QString help() const;
    Q_REQUIRED_RESULT virtual QUrl href() const;

    Q_REQUIRED_RESULT QString comment() const;

    void setComment(const QString &comment);

    void unknownTag(const QStringRef &tag, QString &error);
    void unknowTagValue(const QString &tagValue, QString &error);
    void tooManyArgument(const QStringRef &tagName, int index, int maxValue, QString &error);
    void serverDoesNotSupportFeatures(const QString &feature, QString &error);

protected:
    Q_REQUIRED_RESULT SieveImapAccountSettings sieveImapAccountSettings() const;
    Q_REQUIRED_RESULT QStringList listOfIncludeFile() const;
    Q_REQUIRED_RESULT QStringList sieveCapabilities() const;
    SieveEditorGraphicalModeWidget *mSieveGraphicalModeWidget = nullptr;

Q_SIGNALS:
    void valueChanged();

private:
    Q_DISABLE_COPY(SieveAction)
    QString mName;
    QString mLabel;
    QString mComment;
};
}

#endif // SIEVEACTION_H
