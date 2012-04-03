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
#include <QMessageBox>
#include <QPainter>

#include "qtomatoconfigdialog.h"
#include "qtomatotimer.h"

#include "qtomatotray.h"

QTomatoTray::QTomatoTray(QObject *parent) :
    QSystemTrayIcon( QIcon( ":/icons/res/tomato.svg" ), parent)
  , mMenu( 0 )
  , mStepLock( 0 )
{
    mTimer = new QTomatoTimer( this );
    connect( mTimer, SIGNAL(tick( int, int ) ), SLOT( slotTick( int, int )));
    connect( mTimer, SIGNAL(pomodoroCompleted()), SLOT( slotPomodoroCompleted() ) );
    connect( mTimer, SIGNAL(shortBreakCompleted()), SLOT( slotShortBreakCompleted()) );
    connect( mTimer, SIGNAL(longBreakCompleted()), SLOT( slotLongBreakCompleted()) );
    connect( mTimer, SIGNAL(requestConfirmation()), SLOT( slotRequestConfirmation() ) );

    connect( this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), SLOT(slotActivated(QSystemTrayIcon::ActivationReason)));
    connect( this, SIGNAL(messageClicked()), SLOT(slotStep()));

    buildMenu();
    updateTooltip();
    updateIcon();
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
  if ( mStepLock == 0 ) {
    ++mStepLock;
    QTimer::singleShot( 100, this, SLOT( slotReleaseLock() ) );
    mTimer->step();
  }
}

void QTomatoTray::slotTick( int pSecondsLeft, int pTotal )
{
  updateTooltip( pSecondsLeft );
  updateIcon( pSecondsLeft, pTotal );
}

void QTomatoTray::updateIcon( int pSecondsLeft, int pTotal )
{
  static QIcon icon( ":/icons/res/tomato.svg" );
  static QPixmap base = icon.pixmap( 64, 64 );

  QPixmap pm = base;

  if ( pSecondsLeft != -1 ) {
    // turn to gray
    QPainter p( &pm );

    // no border
    QPen pen = p.pen();
    pen.setWidth( 0 );
    p.setPen( pen );

    p.setCompositionMode( QPainter::CompositionMode_SourceAtop );

    int width = qRound( 64 * pSecondsLeft / pTotal );
    p.fillRect( 64 - width, 0, width, 64, QColor( 0, 0, 0, 128 ) );
  }

  setIcon( QIcon( pm ) );
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
  a = mMenu->addAction( tr( "&Idle" ) );
  connect( a, SIGNAL( triggered()), SLOT( slotIdle() ) );
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
  } else if ( state == QTomatoTimer::IDLE || state == QTomatoTimer::AWAITPOMODORO ) {
    tooltip = tr( "QTomato - %1, click the icon to start a new pomodoro." ).arg( mTimer->getStateString() );
  } else if ( state == QTomatoTimer::AWAITBREAK ) {
    tooltip = tr( "QTomato - Pomodoro finished, click the icon to start a %1 break.", "short or long break" ).arg(
        mTimer->nextBreakIsLong() ? tr( "long" ) : tr( "short" ) );
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
  showMessage( tr( "QTomato" ), tr( "Short break is over, click here to start a new pomodoro." ) );
}

void QTomatoTray::slotLongBreakCompleted()
{
  showMessage( tr( "QTomato" ), tr( "Long break is over, click here to start a new pomodoro." ) );
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
  ++mStepLock;

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

  --mStepLock;
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
  --mStepLock;
}

void QTomatoTray::slotAbout()
{
  QString text = tr( "QTomato %1" ).arg( QCoreApplication::applicationVersion() );

  text += "\n\n";

  text += "Bram Schoenmakers <me@bramschoenmakers.nl>";

  QMessageBox::about( 0, tr( "About QTomato" ), text );
}

void QTomatoTray::slotIdle()
{
  QString question;

  switch( mTimer->getState() ) {
    case QTomatoTimer::POMODORO: {
      question = tr( "Do you want to abort this pomodoro?" );
      break;
    }
    case QTomatoTimer::SHORTBREAK: {
      question = tr( "Do you want to abort your short break?" );
      break;
    }
    case QTomatoTimer::LONGBREAK: {
      question = tr( "Do you want to abort your long break?" );
      break;
    }
    case QTomatoTimer::AWAITBREAK:
    case QTomatoTimer::AWAITPOMODORO: {
      question = tr( "Do you want QTomato to go idle?" );
      break;
    }
    case QTomatoTimer::IDLE: {
      // do nothing
      return;
    }
    default: Q_ASSERT( "There's no question to ask in this state." );
  }

  if ( QMessageBox::question( 0, tr( "QTomato" ), question, QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes ) {
    mTimer->goIdle();
  }
}
