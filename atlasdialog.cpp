#include "atlasdialog.h"
#include "ui_atlasdialog.h"

AtlasDialog::AtlasDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AtlasDialog)
{
    ui->setupUi(this);
}

AtlasDialog::~AtlasDialog()
{
    delete ui;
}
