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

protected:
    void buildMenu();
    void updateIcon(int pSecondsLeft);
    void updateTooltip( int pSecondsLeft = -1 );

private:
    QMenu *mMenu;
    QTomatoTimer *mTimer;
};

#endif // QTOMATOTRAY_H
