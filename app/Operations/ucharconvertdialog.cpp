#include "ucharconvertdialog.h"
#include "ui_ucharconvertdialog.h"

UCharConvertDialog::UCharConvertDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UCharConvertDialog)
{
    ui->setupUi(this);
}

UCharConvertDialog::~UCharConvertDialog()
{
    delete ui;
}
