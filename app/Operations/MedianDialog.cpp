#include "MedianDialog.h"
#include "ui_MedianDialog.h"

MedianDialog::MedianDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MedianDialog)
{
    ui->setupUi(this);
}

bool MedianDialog::cross() const {
    return ui->radioButton->isChecked();
}

bool MedianDialog::square() const {
    return ui->radioButton_2->isChecked();
}

unsigned int MedianDialog::getSize() const {
    return ui->spinBox->value();
}

MedianDialog::~MedianDialog()
{
    delete ui;
}
