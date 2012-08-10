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

static QTomatoConfig *gInstance = 0;

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

QTomatoConfig *QTomatoConfig::instance() {
  if ( !gInstance ) {
    gInstance = new QTomatoConfig();
    gInstance->load();
  }

  return gInstance;
}

void QTomatoConfig::save() const
{
  QSettings s;

  s.setValue( "pomodorolength" + gSuffix, mPomodoroLength );
  s.setValue( "shortbreaklength" + gSuffix, mShortBreakLength );
  s.setValue( "longbreaklength" + gSuffix, mLongBreakLength );
  s.setValue( "longbreakinterval" + gSuffix, mLongBreakInterval );
  s.setValue( "penaltyfactor" + gSuffix, mPenaltyFactor );
  s.setValue( "rewardfactor" + gSuffix, mRewardFactor );
}

void QTomatoConfig::load()
{
  QSettings s;

  mPomodoroLength = s.value( "pomodorolength" + gSuffix, mPomodoroLength ).toInt();
  mShortBreakLength = s.value( "shortbreaklength" + gSuffix, mShortBreakLength ).toInt();
  mLongBreakLength = s.value( "longbreaklength" + gSuffix, mLongBreakLength ).toInt();
  mLongBreakInterval = s.value( "longbreakinterval" + gSuffix, mLongBreakInterval ).toInt();
  mPenaltyFactor = s.value( "penaltyfactor" + gSuffix, mPenaltyFactor ).toInt();
}
