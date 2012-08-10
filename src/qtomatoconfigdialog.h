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

#ifndef QTOMATOCONFIGDIALOG_H
#define QTOMATOCONFIGDIALOG_H

#include <QDialog>

class QTomatoConfig;

namespace Ui {
class QTomatoConfigDialog;
}

class QTomatoConfigDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit QTomatoConfigDialog(QWidget *parent = 0);
    ~QTomatoConfigDialog();

    void getConfig() const;
    void setConfig();
    
private:
    Ui::QTomatoConfigDialog *ui;
};

#endif // QTOMATOCONFIGDIALOG_H
