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

#include "ksecretsservicecollection.h"
#include "ksecretsservicecollection_p.h"
#include "dbusbackend.h"
#include "ksecretscollectionjobs.h"

#include <QDateTime>
#include <QtDBus/QDBusPendingReply>
#include <kcompositejob.h>
#include <QTimer>
#include <kjob.h>

namespace KSecretsService {

Collection::Collection(): QObject()
{
    // nothing to do
}

Collection * KSecretsService::Collection::findCollection(const QString& collectionName, 
                                                         KSecretsService::Collection::FindCollectionOptions options)
{
    // this will simply return the C++ collection objet, without trying to connect to the daemon
    // this will be handled later on, when first calls to other methods will happen
    Collection *collection = new Collection();
    collection->d->setPendingFindCollection( collectionName, options );
    return collection;
}

KJob* Collection::deleteCollection()
{
    return new DeleteCollectionJob( this );
}

KJob* Collection::renameCollection(const QString& newName)
{
    // TODO: implement this
    return NULL;
}

Collection::SearchItemsJob* Collection::searchItems(const QStringStringMap& attributes)
{
    // TODO: implement this
    return NULL;
}

Collection::SearchSecretsJob* Collection::searchSecrets(const QStringStringMap& attributes)
{
    // TODO: implement this
    return NULL;
}

Collection::CreateItemJob* KSecretsService::Collection::createItem(const QMap< QString, QString >& attributes, const Secret& secret)
{
    // TODO: call the dbus method
    // if it returns item with "/" path, then it must have returned a prompt object
    //      use the prompt 
    //      get the create item (from the prompt ?)
    // if not, return the corresponding item+
    return NULL;

}

Collection::ReadItemsJob* Collection::items() const
{
    // TODO: implement this
    return NULL;
}

bool Collection::isLocked() const
{
    // TODO: implement this
    return false;
}

QString Collection::label() const
{
    // TODO: implement this
    return "";
}

QDateTime Collection::createdTime() const
{
    // TODO: implement this
    return QDateTime();
}

QDateTime Collection::modifiedTime() const
{
    // TODO: implement this
    return QDateTime();
}

void Collection::setLabel(const QString& label)
{
    // TODO: implement this
}



Collection::SearchItemsJob::SearchItemsJob( Collection *collection,
                                            QObject *parent ) :
    CollectionJob( collection, parent ) 
{
}


Collection::SearchSecretsJob::SearchSecretsJob( Collection* collection, QObject* parent ) : 
    CollectionJob( collection, parent )
{
}

Collection::CreateItemJob::CreateItemJob( Collection *collection,
                                          QObject *parent ) :
    CollectionJob( collection, parent )
{
}

Collection::ReadItemsJob::ReadItemsJob( Collection *collection,
                                        QObject *parent ) :
    CollectionJob( collection, parent )
{
}
    
QList< Secret > Collection::SearchSecretsJob::secrets() const
{
    // TODO: implement this
    return QList< Secret >();
}





CollectionPrivate::CollectionPrivate() :
        findOptions( Collection::OpenOnly ),
        findStatus( Invalid ) 
{
}

void CollectionPrivate::setPendingFindCollection( const QString &collName, 
                                               Collection::FindCollectionOptions options ) 
{
    // TODO: implemement this
}

bool CollectionPrivate::isValid() const 
{
    // TODO: implemement this
    return false;
}

#include "ksecretsservicecollection.moc"

    
};

