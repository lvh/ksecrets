/* This file is part of the KDE project
 *
 * Copyright (C) 2011 Valentin Rusu <kde@rusu.info>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef KSECRETSITEMJOBS_P_H
#define KSECRETSITEMJOBS_P_H

#include "ksecretsserviceitemjobs.h"
#include "ksecretsservicedbustypes.h"
#include "ksecretsitem_p.h"
#include "ksecretsvalue_p.h"

#include <qdbusconnection.h>
#include <qwindowdefs.h>

class QDBusPendingCallWatcher;

namespace KSecrets {

class GetSecretItemSecretJob;
class SecretItemPrivate;

class SecretItemJobPrivate {
    Q_DISABLE_COPY(SecretItemJobPrivate)
public:
    SecretItemJobPrivate();

    SecretItem  *_item;
    QVariant    _customData;
};

class GetSecretItemSecretJobPrivate  : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(GetSecretItemSecretJobPrivate)
public:
    GetSecretItemSecretJobPrivate( GetSecretItemSecretJob * );

    void start();

private Q_SLOTS:
    void getSecretReply( QDBusPendingCallWatcher* watcher );

Q_SIGNALS:
    void getSecretFinished( KSecrets::SecretItemJob::ItemJobError, const QString& );

public:
    GetSecretItemSecretJob *job;
    DBusSecretStruct secret;
    const QSharedDataPointer< SecretItemPrivate > secretItemPrivate;
};

class SetSecretItemSecretJobPrivate : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(SetSecretItemSecretJobPrivate)
public:
    SetSecretItemSecretJobPrivate( SetSecretItemSecretJob*, const Secret & );

    void start();

private Q_SLOTS:
    void setSecretReply( QDBusPendingCallWatcher* watcher );

public:
    SetSecretItemSecretJob *job;
    Secret secret;
    QSharedDataPointer< SecretItemPrivate > secretItemPrivate;
    QSharedDataPointer< SecretPrivate> secretPrivate;
};

class SecretItemDeleteJobPrivate : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(SecretItemDeleteJobPrivate)
public:
    SecretItemDeleteJobPrivate( SecretItemDeleteJob* );

    void startDelete();

private Q_SLOTS:
    void deleteItemReply( QDBusPendingCallWatcher *watcher );
    void deletePromptFinished(KJob*);

public:
    SecretItemDeleteJob *job;
    QSharedDataPointer< SecretItemPrivate > secretItemPrivate;
    WId                 promptWinId;
};

class ReadItemPropertyJobPrivate : public QSharedData {
public:
    explicit ReadItemPropertyJobPrivate( QSharedDataPointer< SecretItemPrivate> , ReadItemPropertyJob* );

    void startReadingProperty();

    QSharedDataPointer< SecretItemPrivate > itemPrivate;
    ReadItemPropertyJob *readPropertyJob;
    const char *propertyName;
    QVariant value;
};

class WriteItemPropertyJobPrivate : public QSharedData {
public:
    explicit WriteItemPropertyJobPrivate( QSharedDataPointer< SecretItemPrivate>, WriteItemPropertyJob* );

    void startWritingProperty();

    QSharedDataPointer< SecretItemPrivate > itemPrivate;
    WriteItemPropertyJob *writePropertyJob;
    const char *propertyName;
    QVariant value;
};

} // namespace

#endif // KSECRETSITEMJOBS_P_H
