/* This file is part of the KDE project
 *
 * Copyright (C) 2010 Dario Freddi <drf@kde.org>
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

#ifndef KSECRETSERVICE_LIB_H
#define KSECRETSERVICE_LIB_H

#include <QtCore/QObject>
#include <QtCore/QVariantMap>

class KJob;
class KSecretServiceCollection;
class OrgFreedesktopSecretSessionInterface;
class OrgFreedesktopSecretServiceInterface;

class KSecretService : public QObject
{
    Q_OBJECT

public:
    static KSecretService *instance();

    virtual ~KSecretService();

    enum SessionEncryptingAlgorithm {
        ALGORITHM_PLAIN
        // TODO: add other algorithm types here
    };
    
    void setAlgorithm( SessionEncryptingAlgorithm sessionAlgorithm );
    SessionEncryptingAlgorithm sessionAlgorithm() const { return _sessionAlgorithm; }
    
    /*
     * @return true if we're connected to the ksecretservice daemon
     */
    bool isConnected() const;

    /*
     * This method will attempt to connect to KSecretService daemon if not aloready connected
     * @return the main service interface
     */
    OrgFreedesktopSecretServiceInterface *service();
    
    /*
     * This method will attempt to connect to KSecretService daemon if not aloready connected
     * @return the KSecretService session interface
     */
    OrgFreedesktopSecretSessionInterface *session();
    
    QStringList collections() const;
    KSecretServiceCollection *openCollection(const QString &label);

    KJob *createCollectionJob(const QString &label, const QVariantMap &properties = QVariantMap());
    KJob *deleteCollection(const QString &label);

private:
    KSecretService(QObject *parent = 0);
    
    /**
     * This method will connect the client process using this class to the ksecretservice daemon
     */
    void connectToService();

    class Private;
    Private * const d;
    SessionEncryptingAlgorithm _sessionAlgorithm;

    friend class CreateCollectionJob;
    friend class DeleteCollectionJob;
    friend class ReadEntryJob;
    friend class WriteEntryJob;
};

#endif // KSECRETSERVICE_H
