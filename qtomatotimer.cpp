#include <QDebug>

#include "qtomatotimer.h"

QTomatoTimer::QTomatoTimer(QObject *parent) :
    QObject(parent)
  , mState( QTomatoTimer::IDLE )
  , mSecondsLeft( 0 )
  , mCompleted( 0 )
{
  connect( &mTimer, SIGNAL(timeout()), SLOT(slotTick()));
}

void QTomatoTimer::startPomodoro()
{
  qDebug() << "Starting new pomodoro.";

  startTimer( mConfig.mPomodoroLength );
  mState = QTomatoTimer::POMODORO;
}

void QTomatoTimer::startShortBreak()
{
  qDebug() << "Starting short break.";

  startTimer( mConfig.mShortBreakLength );
  mState = QTomatoTimer::SHORTBREAK;
}

void QTomatoTimer::startLongBreak()
{
  qDebug() << "Starting long break.";

  startTimer( mConfig.mLongBreakLength );
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

void QTomatoTimer::slotReset()
{
    goIdle();
    mCompleted = 0;
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
        mState = AWAITBREAK;
        emit pomodoroCompleted();
        break;
      }
      case QTomatoTimer::SHORTBREAK: {
        goIdle();
        emit shortBreakCompleted();
        break;
      }
      case QTomatoTimer::LONGBREAK: {
        goIdle();
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
    case IDLE: {
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
  slotReset();
}

QTomatoConfig QTomatoTimer::getConfig() const
{
  return mConfig;
}
