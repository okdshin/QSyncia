#include "advanceddialog.h"
#include "ui_advanceddialog.h"

AdvancedDialog::AdvancedDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdvancedDialog)
{
    ui->setupUi(this);
}

AdvancedDialog::~AdvancedDialog()
{
    delete ui;
}
