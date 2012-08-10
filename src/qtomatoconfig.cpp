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

#include <QSettings>

#include "qtomatoconfig.h"

#ifdef QT_NO_DEBUG
  static const QString gSuffix;
#else
  static const QString gSuffix = "-dbg";
#endif

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

void QTomatoConfig::save( const QTomatoConfig &pConfig )
{
  QSettings s;

  s.setValue( "pomodorolength" + gSuffix, pConfig.mPomodoroLength );
  s.setValue( "shortbreaklength" + gSuffix, pConfig.mShortBreakLength );
  s.setValue( "longbreaklength" + gSuffix, pConfig.mLongBreakLength );
  s.setValue( "longbreakinterval" + gSuffix, pConfig.mLongBreakInterval );
  s.setValue( "penaltyfactor" + gSuffix, pConfig.mPenaltyFactor );
  s.setValue( "rewardfactor" + gSuffix, pConfig.mRewardFactor );
}

QTomatoConfig QTomatoConfig::load()
{
  QSettings s;
  QTomatoConfig config;

  config.mPomodoroLength = s.value( "pomodorolength" + gSuffix, config.mPomodoroLength ).toInt();
  config.mShortBreakLength = s.value( "shortbreaklength" + gSuffix, config.mShortBreakLength ).toInt();
  config.mLongBreakLength = s.value( "longbreaklength" + gSuffix, config.mLongBreakLength ).toInt();
  config.mLongBreakInterval = s.value( "longbreakinterval" + gSuffix, config.mLongBreakInterval ).toInt();
  config.mPenaltyFactor = s.value( "penaltyfactor" + gSuffix, config.mPenaltyFactor ).toInt();
  config.mRewardFactor = s.value( "rewardfactor" + gSuffix, config.mRewardFactor ).toInt();

  return config;
}
