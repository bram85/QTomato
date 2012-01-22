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

#include "qtomatoconfigdialog.h"
#include "ui_qtomatoconfigdialog.h"

QTomatoConfigDialog::QTomatoConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QTomatoConfigDialog)
{
    // TODO: set values
    ui->setupUi(this);
}

QTomatoConfigDialog::~QTomatoConfigDialog()
{
    delete ui;
}

QTomatoConfig QTomatoConfigDialog::getConfig() const
{
  QTomatoConfig config;

  config.mPomodoroLength = ui->pomodoroLengthInput->value() * 60;
  config.mShortBreakLength = ui->shortBreakLengthInput->value() * 60;
  config.mLongBreakLength = ui->longBreakLengthInput->value() * 60;
  config.mLongBreakInterval = ui->longBreakIntervalInput->value() * 60;

  return config;
}
