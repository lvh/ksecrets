/*
 * Copyright 2011, Valentin Rusu <valir@kde.org>
 * Copyright 2015, Valentin Rusu <valir@kde.org>
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

#ifndef KSECRETS_TEST_H
#define KSECRETS_TEST_H

#include <QtCore/QObject>

class KSecretServiceTest : public QObject {
    Q_OBJECT
public:
    explicit KSecretServiceTest(QObject* parent = 0);

private Q_SLOTS:
    void initTestCase();
    void testCreateAndDelete();
    void testRenameCollection();
    void testItems();
    void cleanupTestCase();

private:
    void setupKeyring();
    void cleanupKeyring();
};
#endif // KSECRETS_TEST_H
