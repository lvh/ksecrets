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

#ifndef KSECRETSITEM_H
#define KSECRETSITEM_H

#include "ksecretsservicesecret.h"
#include "ksecretsserviceitemjobs.h"

#include <kjob.h>

class SecretItemPrivate;

namespace KSecretsService {

typedef QMap< QString, QString > QStringStringMap;

class GetSecretItemSecretJob;
class SetSecretItemSecretJob;
   
/**
 * KSecretsService aims to let application store sensitive pieces of information as SecretItem(s).
 * The central part of a SecretItem is the secret it holds. The secret is actually a structure named @ref SecretStruct["(SecretStruct)"]
 * SecretItems can be qualified using attributes. These attributes are used internally by KSecretsService to uniquely identify them inside the collection.
 * The attributes list always contain at least one item, named "Label". It's content is up to the client application.
 * The "Label" attribute can also be read by calling the @ref attribute method and set by @ref setLabel method.
 */
class SecretItem {
protected:
    SecretItem();
public:
    SecretItem( const QSharedPointer< SecretItemPrivate > & );
    SecretItem( SecretItemPrivate * );
    
    /**
     */
    KJob * deleteItem();

    /**
     * Read the data held by the SecretItem
     */
    GetSecretItemSecretJob * getSecret() const;

    /**
     * Modify the item's stored data
     */
    SetSecretItemSecretJob* setSecret( const Secret &secret );

    /**
     * FIXME: This methods accesses a dbus property. should it be asynchronous ?
     */
    QMap< QString, QString> attributes() const;
    
    /**
     * FIXME: This methods accesses a dbus property. should it be asynchronous ?
     * @param attributes a map containing the new attributes; it must contain at least one attribute, under the name "Label"
     */
    void setAttributes( const QMap< QString, QString > &attributes );

    /**
     * FIXME: This methods accesses a dbus property. should it be asynchronous ?
     */
    bool isLocked() const;
    
    /**
     * FIXME: This methods accesses a dbus property. should it be asynchronous ?
     */
    QString label() const;
    
    /**
     * FIXME: This methods accesses a dbus property. should it be asynchronous ?
     */
    QDateTime createdTime() const;
    
    /**
     * FIXME: This methods accesses a dbus property. should it be asynchronous ?
     */
    QDateTime modifiedTime() const;
    
    /**
     * FIXME: This methods accesses a dbus property. should it be asynchronous ?
     */
    void setLabel( const QString &label );
    
private:
    friend class SecretItemPrivate;
    friend class GetSecretItemSecretJob;
    friend class SetSecretItemSecretJob;
    
    QSharedPointer< SecretItemPrivate > d;
};



};

#endif // KSECRETSITEM_H
