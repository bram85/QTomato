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

#include <qmath.h>
#include <QDebug>

#include "qtomatotimer.h"

QTomatoTimer::QTomatoTimer(QObject *parent) :
    QObject(parent)
  , mState( QTomatoTimer::IDLE )
  , mSecondsLeft( 0 )
  , mCompleted( 0 )
  , mTotalCompleted( 0 )
{
  mConfig = QTomatoConfig::load();
  connect( &mTimer, SIGNAL(timeout()), SLOT(slotTick()));
}

void QTomatoTimer::startPomodoro()
{
  qDebug() << mSecondsLeft;
  int penalty = 0;
  if ( mSecondsLeft < -10 ) {
    penalty = qCeil( -1 * mSecondsLeft * ( qreal( mConfig.mPenaltyFactor ) / 100 ) );
  }

  qDebug() << "Starting new pomodoro with length"
           << mConfig.mPomodoroLength + penalty
           << "("
           << mConfig.mPomodoroLength
           << "+"
           << penalty
           << ")";

  startTimer( mConfig.mPomodoroLength + penalty );
  mState = QTomatoTimer::POMODORO;
}

void QTomatoTimer::startShortBreak()
{
  qDebug() << mSecondsLeft;
  // reward overtime
  int reward = 0;
  if ( mSecondsLeft <= -10 ) {
    reward = qFloor( -1 * mSecondsLeft * ( qreal( mConfig.mRewardFactor ) / 100 ) );
  }

  qDebug() << "Starting short break with length"
           << mConfig.mShortBreakLength + reward
           << "("
           << mConfig.mShortBreakLength
           << "+"
           << reward
           << ")";

  startTimer( mConfig.mShortBreakLength + reward );
  mState = QTomatoTimer::SHORTBREAK;
}

void QTomatoTimer::startLongBreak()
{
  qDebug() << mSecondsLeft;
  // reward overtime
  int reward = 0;
  if ( mSecondsLeft <= -10 ) {
    reward = qFloor( -1 * mSecondsLeft * ( qreal( mConfig.mRewardFactor ) / 100 ) );
  }

  qDebug() << "Starting long break with length"
           << mConfig.mLongBreakLength + reward
           << "("
           << mConfig.mLongBreakLength
           << "+"
           << reward
           << ")";

  startTimer( mConfig.mLongBreakLength + reward );
  mState = QTomatoTimer::LONGBREAK;
}

void QTomatoTimer::goIdle()
{
  qDebug() << "Idling.";

  mTimer.stop();
  mState = IDLE;
  mSecondsLeft = 0;

  emit tick( -1 );
}

void QTomatoTimer::reset()
{
    goIdle();
    mCompleted = 0;
    mTotalCompleted = 0;
}

void QTomatoTimer::startTimer( int pSeconds )
{
  mTimer.start( 1000 );
  mSecondsLeft = pSeconds;
}

void QTomatoTimer::slotTick()
{
  --mSecondsLeft;
  emit tick( mSecondsLeft );

  if ( mSecondsLeft == 0 ) {
    switch ( mState ) {
      case QTomatoTimer::POMODORO: {
        ++mCompleted;
        ++mTotalCompleted;
        mState = AWAITBREAK;
        emit pomodoroCompleted();
        break;
      }
      case QTomatoTimer::SHORTBREAK: {
        mState = AWAITPOMODORO;
        emit shortBreakCompleted();
        break;
      }
      case QTomatoTimer::LONGBREAK: {
        mState = AWAITPOMODORO;
        mCompleted = 0;
        emit longBreakCompleted();
        break;
      }
      default: {
        goIdle();
        break;
      }
    }
  }
}

QTomatoTimer::QTomatoState QTomatoTimer::getState() const
{
    return mState;
}

void QTomatoTimer::step()
{
  switch ( mState ) {
    case IDLE:
    case AWAITPOMODORO: {
      startPomodoro();
      break;
    }
    case POMODORO:
    case SHORTBREAK:
    case LONGBREAK: {
      emit requestConfirmation();
      break;
    }
    case AWAITBREAK: {
      if ( mCompleted < mConfig.mLongBreakInterval ) {
        startShortBreak();
      } else {
        startLongBreak();
      }
      break;
    }
    default: {
      goIdle();
      break;
    }
  }
}

void QTomatoTimer::confirm()
{
  switch ( mState ) {
  case POMODORO: {
    goIdle();
    break;
  }
  case LONGBREAK: {
    mCompleted = 0;
    // fall through
  }
  case SHORTBREAK: {
    startPomodoro();
    break;
  }
  default: Q_ASSERT( "confirm not possible in this state." );
  }
}

QString QTomatoTimer::getStateString() const
{
  return stateToString( mState );
}

QString QTomatoTimer::stateToString( QTomatoState pState )
{
  switch ( pState ) {
    case IDLE: return tr( "Idle" );
    case POMODORO: return tr( "Pomodoro" );
    case SHORTBREAK: return tr( "Short break" );
    case LONGBREAK: return tr( "Long break" );
    case AWAITBREAK: return tr( "Awaiting break" );
    case AWAITPOMODORO: return tr( "Awaiting new pomodoro" );
    default: Q_ASSERT( "Unknown state." );
  }

  return "Error";
}

bool QTomatoTimer::nextBreakIsLong() const
{
  return mCompleted == mConfig.mLongBreakInterval;
}

void QTomatoTimer::setConfig( QTomatoConfig pConfig )
{
  mConfig = pConfig;
  reset();
}

QTomatoConfig QTomatoTimer::getConfig() const
{
  return mConfig;
}

int QTomatoTimer::getCompleted() const
{
  // mCompleted gets reset after long break, not useful to expose.
  return mTotalCompleted;
}
