#ifndef QTOMATOTIMER_H
#define QTOMATOTIMER_H

#include <QObject>
#include <QTimer>

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
    QTomatoState mState;
    int mSecondsLeft;
    int mCompleted;

    QTimer mTimer;
};

#endif // QTOMATOTIMER_H
