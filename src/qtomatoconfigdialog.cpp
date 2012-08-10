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

#include "qtomatoconfigdialog.h"
#include "ui_qtomatoconfigdialog.h"

#ifndef QT_NO_DEBUG
static const int sDebugFactor = 1;
#else
static const int sDebugFactor = 60;
#endif

QTomatoConfigDialog::QTomatoConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QTomatoConfigDialog)
{
    ui->setupUi(this);
}

QTomatoConfigDialog::~QTomatoConfigDialog()
{
    delete ui;
}

void QTomatoConfigDialog::getConfig() const
{
  QTomatoConfig *config = QTomatoConfig::instance();

  config->mPomodoroLength = ui->pomodoroLengthInput->value() * sDebugFactor;
  config->mShortBreakLength = ui->shortBreakLengthInput->value() * sDebugFactor;
  config->mLongBreakLength = ui->longBreakLengthInput->value() * sDebugFactor;
  config->mLongBreakInterval = ui->longBreakIntervalInput->value();
  config->mPenaltyFactor = qRound( ui->penaltyFactorInput->value() * 100 ) ;
  config->mRewardFactor = qRound( ui->rewardFactorInput->value() * 100 );
}

void QTomatoConfigDialog::setConfig()
{
  QTomatoConfig *config = QTomatoConfig::instance();

  ui->pomodoroLengthInput->setValue( config->mPomodoroLength / sDebugFactor );
  ui->shortBreakLengthInput->setValue( config->mShortBreakLength / sDebugFactor );
  ui->longBreakLengthInput->setValue( config->mLongBreakLength / sDebugFactor );
  ui->longBreakIntervalInput->setValue( config->mLongBreakInterval );
  ui->penaltyFactorInput->setValue( qreal( config->mPenaltyFactor ) / 100 );
  ui->rewardFactorInput->setValue( qreal( config->mRewardFactor ) / 100 );
}
