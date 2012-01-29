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

#include <QCoreApplication>
#include <QMenu>
#include <QMessageBox>

#include "qtomatoconfigdialog.h"
#include "qtomatotimer.h"

#include "qtomatotray.h"

QTomatoTray::QTomatoTray(QObject *parent) :
    QSystemTrayIcon( QIcon( ":/icons/res/tomato.svg" ), parent)
  , mMenu( 0 )
  , mStepLock( 1 )
{
    mTimer = new QTomatoTimer( this );
    connect( mTimer, SIGNAL(tick( int)), SLOT( slotTick( int )));
    connect( mTimer, SIGNAL(pomodoroCompleted()), SLOT( slotPomodoroCompleted() ) );
    connect( mTimer, SIGNAL(shortBreakCompleted()), SLOT( slotShortBreakCompleted()) );
    connect( mTimer, SIGNAL(longBreakCompleted()), SLOT( slotLongBreakCompleted()) );
    connect( mTimer, SIGNAL(requestConfirmation()), SLOT( slotRequestConfirmation() ) );

    connect( this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), SLOT(slotActivated(QSystemTrayIcon::ActivationReason)));
    connect( this, SIGNAL(messageClicked()), SLOT(slotStep()));

    buildMenu();
    updateTooltip();
    show();
}

void QTomatoTray::slotActivated( QSystemTrayIcon::ActivationReason pActivationReason )
{
  if ( pActivationReason == QSystemTrayIcon::Trigger ) {
    slotStep();
  }
}

void QTomatoTray::slotStep()
{
  // make sure to call mTimer->step() only once for one event loop pass.
  if ( mStepLock ) {
    --mStepLock;
    QTimer::singleShot( 100, this, SLOT( slotReleaseLock() ) );
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
  QString message = mTimer->nextBreakIsLong() ? tr( "Time's up, click to start a long break." )
                                              : tr( "Time's up, click to start a short break." );
  showMessage( tr( "QTomato" ), message );
}

void QTomatoTray::buildMenu()
{
	if ( !mMenu ) {
		mMenu = new QMenu();
	}

  QAction *a;
    a = mMenu->addAction( tr( "&Reset" ) );
	connect( a, SIGNAL( triggered()), SLOT( slotReset()) );
  mMenu->addSeparator();
  a = mMenu->addAction( tr( "&Settings..." ) );
  connect( a, SIGNAL( triggered()), SLOT( slotShowConfiguration() ) );
  mMenu->addSeparator();
  a = mMenu->addAction( tr( "&About QTomato..." ) );
  connect( a, SIGNAL( triggered()), SLOT( slotAbout() ) );
  a = mMenu->addAction( tr( "&Quit" ) );
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
        tooltip += tr( "QTomato - %1, %2 minutes and %3 seconds left." ).arg( mTimer->getStateString() ).arg( minutes ).arg( seconds );
      } else {
        tooltip += tr( "QTomato - %1, %2 seconds left." ).arg( mTimer->getStateString() ).arg( seconds );
      }
    }
  } else if ( state == QTomatoTimer::IDLE ) {
    tooltip = tr( "QTomato - Idle, click the icon to start a new pomodoro." );
  } else if ( state == QTomatoTimer::AWAITBREAK ) {
    tooltip = tr( "QTomato - Pomodoro finished, click the icon to start the break." );
  } else {
    Q_ASSERT( "Unknown state." );
  }

  int completed = mTimer->getCompleted();
  if ( completed > 0 ) {
    // FIXME: not RTL friendly
    tooltip += " " + tr( "%n pomodoro(s) completed.", "", completed );
  }

  setToolTip( tooltip );
}

void QTomatoTray::slotShortBreakCompleted()
{
  showMessage( tr( "QTomato" ), "Short break is over, click here to start." );
}

void QTomatoTray::slotLongBreakCompleted()
{
  showMessage( tr( "QTomato" ), "Long break is over, click here to start." );
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

  qcd->setConfig( mTimer->getConfig() );
  if ( qcd->exec() == QDialog::Accepted ) {
    QTomatoConfig config = qcd->getConfig();
    mTimer->setConfig( config );
    QTomatoConfig::save( config );
  }

  delete qcd;
}

void QTomatoTray::slotReset()
{
  if ( QMessageBox::question( 0, tr( "QTomato" ), tr( "Do you want to reset QTomato? All completed pomodoros will be lost." ), QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes ) {
    mTimer->reset();
  }
}

void QTomatoTray::slotReleaseLock()
{
  mStepLock = 1;
}

void QTomatoTray::slotAbout()
{
  QString text = tr( "QTomato %1" ).arg( QCoreApplication::applicationVersion() );

  text += "\n\n";

  text += "Bram Schoenmakers <me@bramschoenmakers.nl>";

  QMessageBox::about( 0, tr( "About QTomato" ), text );
}
