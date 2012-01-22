#include <QCoreApplication>
#include <QMenu>
#include <QMessageBox>

#include "qtomatoconfigdialog.h"
#include "qtomatotimer.h"

#include "qtomatotray.h"

QTomatoTray::QTomatoTray(QObject *parent) :
    QSystemTrayIcon( QIcon( ":/icons/res/tomato.svg" ), parent)
  , mMenu( 0 )
{
    mTimer = new QTomatoTimer( this );
    connect( mTimer, SIGNAL(tick( int)), SLOT( slotTick( int )));
    connect( mTimer, SIGNAL(pomodoroCompleted()), SLOT( slotPomodoroCompleted() ) );
    connect( mTimer, SIGNAL(shortBreakCompleted()), SLOT( slotShortBreakCompleted()) );
    connect( mTimer, SIGNAL(longBreakCompleted()), SLOT( slotLongBreakCompleted()) );
    connect( mTimer, SIGNAL(requestConfirmation()), SLOT( slotRequestConfirmation() ) );

    connect( this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), SLOT(slotActivated(QSystemTrayIcon::ActivationReason)));

    buildMenu();
    updateTooltip();
    show();
}

void QTomatoTray::slotActivated( QSystemTrayIcon::ActivationReason pActivationReason )
{
  if ( pActivationReason == QSystemTrayIcon::Trigger ) {
    mTimer->step();
  }
}

void QTomatoTray::slotTick( int pSecondsLeft )
{
  updateTooltip( pSecondsLeft );
  updateIcon( pSecondsLeft );
}

void QTomatoTray::updateIcon( int pSecondsLeft )
{
    // TODO
}

void QTomatoTray::slotPomodoroCompleted()
{
  QString message = mTimer->nextBreakIsLong() ? tr( "Time's up, click the icon to start a long break." )
                                              : tr( "Time's up, click the icon to start a short break." );
  showMessage( tr( "QTomato" ), message );
}

void QTomatoTray::buildMenu()
{
	if ( !mMenu ) {
		mMenu = new QMenu();
	}

  QAction *a;
	a = mMenu->addAction( tr( "Reset" ) );
	connect( a, SIGNAL( triggered()), mTimer, SLOT( slotReset()) );
  mMenu->addSeparator();
  a = mMenu->addAction( tr( "Settings..." ) );
  connect( a, SIGNAL( triggered()), SLOT( slotShowConfiguration() ) );
  mMenu->addSeparator();
  a = mMenu->addAction( tr( "Quit" ) );
  connect( a, SIGNAL( triggered()), SLOT( slotQuit() ) );

	setContextMenu( mMenu );
}

void QTomatoTray::updateTooltip( int pSecondsLeft )
{
  QString tooltip;
  QTomatoTimer::QTomatoState state = mTimer->getState();

  if ( state == QTomatoTimer::POMODORO || state == QTomatoTimer::SHORTBREAK || state == QTomatoTimer::LONGBREAK ) {
    if ( pSecondsLeft >= 0 ) {
      int seconds = pSecondsLeft % 60;
      int minutes = ( pSecondsLeft - seconds ) / 60;

      if ( minutes > 0 ) {
        tooltip += tr( "QTomato - %1, %2 minutes and %3 seconds left" ).arg( mTimer->getStateString() ).arg( minutes ).arg( seconds );
      } else {
        tooltip += tr( "QTomato - %1, %2 seconds left" ).arg( mTimer->getStateString() ).arg( seconds );
      }
    }
  } else if ( state == QTomatoTimer::IDLE ) {
    tooltip = tr( "QTomato - Idle, click the icon to start a new pomodoro." );
  } else if ( state == QTomatoTimer::AWAITBREAK ) {
    tooltip = tr( "Pomodoro finished, click to start the break." );
  } else {
    Q_ASSERT( "Unknown state." );
  }

  setToolTip( tooltip );
}

void QTomatoTray::slotShortBreakCompleted()
{
  showMessage( tr( "QTomato" ), "Short break is over, click the icon to start." );
}

void QTomatoTray::slotLongBreakCompleted()
{
  showMessage( tr( "QTomato" ), "Long break is over, click the icon to start." );
}

void QTomatoTray::slotQuit()
{
  if ( QMessageBox::question( 0, tr( "QTomato" ), tr( "Do you want to quit QTomato?" ), QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes ) {
    QCoreApplication::exit( 0 );
  }
}

void QTomatoTray::slotRequestConfirmation()
{
  QString question;

  switch( mTimer->getState() ) {
    case QTomatoTimer::POMODORO: {
      question = tr( "Do you want to abort this pomodoro?" );
      break;
    }
    case QTomatoTimer::SHORTBREAK: {
      question = tr( "Do you want to abort your short break and start a new pomodoro?" );
      break;
    }
    case QTomatoTimer::LONGBREAK: {
      question = tr( "Do you want to abort your long break and start a new pomodoro?" );
      break;
    }
    default: Q_ASSERT( "There's no question to ask in this state." );
  }

  if ( QMessageBox::question( 0, tr( "QTomato" ), question, QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes ) {
    mTimer->confirm();
  }

}

void QTomatoTray::slotShowConfiguration()
{
  QTomatoConfigDialog *qcd = new QTomatoConfigDialog();

  if ( qcd->exec() == QDialog::Accepted ) {
    mTimer->setConfig( qcd->getConfig() );
  }

  delete qcd;
}
