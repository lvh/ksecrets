/*
 * Copyright 2010, Dario Freddi <dario.freddi@collabora.co.uk>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef KSECRETUITEST_H
#define KSECRETUITEST_H

#include <QtTest/QtTest>

class BackendCollectionManager;
class BackendCollection;

class KSecretUiTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();

    void testCreateCollectionAsync();
    void testLockCollectionAsync();
    void testUnlockCollectionAsync();

    void cleanupTestCase();

private:
    BackendCollectionManager *m_manager;
    BackendCollection *m_collection;

    QDateTime m_collCreated;
    QDateTime m_collModified;
};

#endif // KSECRETUITEST_H
