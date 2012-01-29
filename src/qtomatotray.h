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

#ifndef QTOMATOTRAY_H
#define QTOMATOTRAY_H

#include <QSystemTrayIcon>

class QMenu;

class QTomatoTimer;

class QTomatoTray : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit QTomatoTray(QObject *parent = 0);
    
protected slots:
    // void slotStartPomodo();
    void slotActivated( QSystemTrayIcon::ActivationReason );
    void slotPomodoroCompleted();
    void slotShortBreakCompleted();
    void slotLongBreakCompleted();
    void slotTick( int pSecondsLeft );
    void slotQuit();
    void slotRequestConfirmation();
    void slotShowConfiguration();
    void slotReset();
    void slotStep();
    void slotReleaseLock();
    void slotAbout();

protected:
    void buildMenu();
    void updateIcon(int pSecondsLeft);
    void updateTooltip( int pSecondsLeft = -1 );

private:
    QMenu *mMenu;
    QTomatoTimer *mTimer;
    int mStepLock;
};

#endif // QTOMATOTRAY_H
