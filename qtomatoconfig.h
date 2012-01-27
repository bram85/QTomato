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

#ifndef QTOMATOCONFIG_H
#define QTOMATOCONFIG_H

class QTomatoConfig
{
public:
  QTomatoConfig();
  void operator=( const QTomatoConfig &pConfig );

  int mPomodoroLength;
  int mShortBreakLength;
  int mLongBreakLength;
  int mLongBreakInterval;

  int mPenaltyFactor;
  int mRewardFactor;
};

#endif // QTOMATOCONFIG_H
