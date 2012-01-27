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

#include "qtomatoconfig.h"

QTomatoConfig::QTomatoConfig()
{
#ifdef QT_NO_DEBUG
  mPomodoroLength = 25 * 60;
  mShortBreakLength = 5 * 60;
  mLongBreakLength = 15 * 60;
#else
  mPomodoroLength = 25;
  mShortBreakLength = 5;
  mLongBreakLength = 15;
#endif

  mLongBreakInterval = 4;

  mPenaltyFactor = 0;
  mRewardFactor = 0;
}

void QTomatoConfig::operator=( const QTomatoConfig &pConfig )
{
  mPomodoroLength = pConfig.mPomodoroLength;
  mShortBreakLength = pConfig.mShortBreakLength;
  mLongBreakLength = pConfig.mLongBreakLength;

  mLongBreakInterval = pConfig.mLongBreakInterval;

  mPenaltyFactor = pConfig.mPenaltyFactor;
  mRewardFactor = pConfig.mRewardFactor;
}
