/*
 * This file is part of QTomato.
 *
 * Bram Schoenmakers <me@bramschoenmakers.nl>
 *
 * QTomato is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * QTomato is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with QTomato.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QtGui/QApplication>
#include "qtomatotray.h"

static const QString sAppname = "QTomato";
static const QString sVersion = "0.3.2";
static const QString sOrganization = "bramschoenmakers.nl";
static const QString sDomain = "bramschoenmakers.nl";

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed( false );

    a.setApplicationName( sAppname );
    a.setApplicationVersion( sVersion );
    a.setOrganizationName( sOrganization );
    a.setOrganizationDomain( sDomain );

    QTomatoTray qtt;

    return a.exec();
}
