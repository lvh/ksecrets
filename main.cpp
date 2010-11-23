/*
 * Copyright 2010, Valentin Rusu <kde@rusu.info>
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


#include <kuniqueapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <kdebug.h>
#include <QtDBus/QDBusConnection>
#include <QtCrypto/QtCrypto>
#include <klocalizedstring.h>
#include <kstandarddirs.h>
#include <QtCrypto/QtCrypto>

#include "ksecretsync.h"

int main(int argc, char **argv)
{
    KAboutData aboutdata("ksecretsync", 0, ki18n("KDE Secret Service Synchronization Daemon"),
                         "0.0", ki18n("KDE Secret Service"),
                         KAboutData::License_GPL, ki18n("(C) 2010 Valentin Rusu"));
    aboutdata.addAuthor(ki18n("Valentin Rusu"), ki18n("Maintainer"), "kde@rusu.info");
    aboutdata.setProgramIconName("ksecretsyncd");

    KCmdLineArgs::init(argc, argv, &aboutdata);
    KUniqueApplication::addCmdLineOptions();
    KUniqueApplication app;

    app.setQuitOnLastWindowClosed(false);

    if(!KUniqueApplication::start()) {
        kDebug() << "ksecretsyncd is already running!";
        return 0;
    }

    // initialize QCA
    QCA::Initializer qcaInit;

    KSecretSync mainWindow;
    mainWindow.setCaption(i18n("KSecretSync"));
    
    KGlobal::dirs()->addResourceType("kwallet", QLatin1String( "share/apps/kwallet" ));

    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
    if ( args ) {
        if (args->isSet("show")) {
            mainWindow.show();
        }
        if (args->isSet("poll")) {
        }
  
    }

    return app.exec();
}
