#include "atlasdialog.h"
#include "ui_kerndialog.h"

KernDialog::KernDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KernDialog)
{
    ui->setupUi(this);
}

KernDialog::~KernDialog()
{
    delete ui;
}
