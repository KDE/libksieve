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
    virtual ~SieveAction();

    QString name() const;
    QString label() const;

    virtual QWidget *createParamWidget(QWidget *parent) const;

    virtual bool setParamWidgetValue(QXmlStreamReader &element, QWidget *parent, QString &error);

    virtual QString code(QWidget *) const;

    virtual QStringList needRequires(QWidget *parent) const;

    virtual bool needCheckIfServerHasCapability() const;

    virtual QString serverNeedsCapability() const;

    virtual QString help() const;
    virtual QUrl href() const;

    QString comment() const;

    void setComment(const QString &comment);

    void unknownTag(const QStringRef &tag, QString &error);
    void unknowTagValue(const QString &tagValue, QString &error);
    void tooManyArgument(const QStringRef &tagName, int index, int maxValue, QString &error);
    void serverDoesNotSupportFeatures(const QString &feature, QString &error);

protected:
    SieveImapAccountSettings sieveImapAccountSettings() const;
    QStringList listOfIncludeFile() const;
    QStringList sieveCapabilities() const;
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
