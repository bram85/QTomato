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

#ifndef QTOMATOTIMER_H
#define QTOMATOTIMER_H

#include <QObject>
#include <QTimer>

#include "qtomatoconfig.h"

class QTomatoTimer : public QObject
{
    Q_OBJECT
public:
    explicit QTomatoTimer(QObject *parent = 0);
    
    enum QTomatoState {
        IDLE = 0,
        POMODORO = 1,
        AWAITBREAK = 2,
        SHORTBREAK = 3,
        LONGBREAK = 4
    };

    /** Proceed when the user clicks the icon. */
    void step();

    /** Proceed when the user confirms an action. */
    void confirm();

    QTomatoState getState() const;
    QString getStateString() const;
    static QString stateToString( QTomatoState pState );

    bool nextBreakIsLong() const;

    QTomatoConfig getConfig() const;
    void setConfig( QTomatoConfig pConfig );

    int getCompleted() const;

signals:
    /**
      * Emit a tick, passing along the number of seconds left.
      */
    void tick( int );
    void pomodoroCompleted();
    void shortBreakCompleted();
    void longBreakCompleted();
    void requestConfirmation();

public slots:
    /** Called every second when mTimer times out. */
    void slotTick();

    /** Reset the timer. */
    void slotReset();

protected:
    void startTimer( int pSeconds );
    void startPomodoro();
    void startShortBreak();
    void startLongBreak();
    void goIdle();

private:
    QTomatoConfig mConfig;
    QTomatoState mState;
    int mSecondsLeft;
    int mCompleted;

    QTimer mTimer;
};

#endif // QTOMATOTIMER_H
