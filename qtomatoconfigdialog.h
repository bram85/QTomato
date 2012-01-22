#ifndef QTOMATOCONFIGDIALOG_H
#define QTOMATOCONFIGDIALOG_H

#include <QDialog>

#include "qtomatoconfig.h"

namespace Ui {
class QTomatoConfigDialog;
}

class QTomatoConfigDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit QTomatoConfigDialog(QWidget *parent = 0);
    ~QTomatoConfigDialog();

    QTomatoConfig getConfig() const;
    
private:
    Ui::QTomatoConfigDialog *ui;
};

#endif // QTOMATOCONFIGDIALOG_H
