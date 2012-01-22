#include <QDebug>

#include "qtomatotimer.h"

// TODO: Make configurable.
#ifdef QT_NO_DEBUG
static const int POMODORO_LENGTH = 25 * 60;
static const int SHORTBREAK_LENGTH = 5 * 60;
static const int LONGBREAK_LENGTH = 15 * 60;
#else // turn minutes into seconds
static const int POMODORO_LENGTH = 25;
static const int SHORTBREAK_LENGTH = 5;
static const int LONGBREAK_LENGTH = 15;
#endif

// the 4th break is a long break
static const int LONGBREAK_INTERVAL = 4;

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

  startTimer( POMODORO_LENGTH );
  mState = QTomatoTimer::POMODORO;
}

void QTomatoTimer::startShortBreak()
{
  qDebug() << "Starting short break.";

  startTimer( SHORTBREAK_LENGTH );
  mState = QTomatoTimer::SHORTBREAK;
}

void QTomatoTimer::startLongBreak()
{
  qDebug() << "Starting long break.";

  startTimer( LONGBREAK_LENGTH );
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
      if ( mCompleted < LONGBREAK_INTERVAL ) {
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
  return mCompleted == LONGBREAK_INTERVAL;
}
