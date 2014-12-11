/*
 * Copyright 2011-2012 INSA Rennes
 *
 * This file is part of ImageINSA.
 *
 * ImageINSA is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ImageINSA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ImageINSA.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ColorDialog.h"
#include "ui_ColorDialog.h"

ColorDialog::ColorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColorDialog)
{
    ui->setupUi(this);
    ui->hsvWidget->setVisible(false);
    this->adjustSize();
}

ColorDialog::~ColorDialog()
{
    delete ui;
}

QColor ColorDialog::getColor() const {

    if(ui->rgbButton->isChecked()) {
        int r = ui->redBox->value();
        int g = ui->greenBox->value();
        int b = ui->blueBox->value();
        return QColor::fromRgb(r, g, b);
    }
    else {
        int h = ui->hueBox->value();
        int s = ui->satBox->value();
        int v = ui->valBox->value();
        return QColor::fromHsv(h, s, v);
    }
}

unsigned int ColorDialog::getWidth() const {
    return ui->widthBox->value();
}

unsigned int ColorDialog::getHeight() const {
    return ui->heightBox->value();
}
