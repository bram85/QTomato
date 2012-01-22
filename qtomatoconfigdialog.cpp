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
