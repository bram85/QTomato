#include "qtomatoconfig.h"

QTomatoConfig::QTomatoConfig()
{
  mPomodoroLength = 25 * 60;
  mShortBreakLength = 5 * 60;
  mLongBreakLength = 15 * 60;

  mLongBreakInterval = 4;
}

void QTomatoConfig::operator=( const QTomatoConfig &pConfig )
{
  mPomodoroLength = pConfig.mPomodoroLength;
  mShortBreakLength = pConfig.mShortBreakLength;
  mLongBreakLength = pConfig.mLongBreakLength;

  mLongBreakInterval = pConfig.mLongBreakInterval;
}
