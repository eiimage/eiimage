/*
 * Copyright 2011-2012 INSA Rennes
 *
 * This file is part of INSAimage.
 *
 * INSAimage is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * INSAimage is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with INSAimage.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "DCTDialog.h"
#include "ui_DCTDialog.h"

DCTDialog::DCTDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DCTDialog)
{
    ui->setupUi(this);
}

DCTDialog::~DCTDialog()
{
    delete ui;
}

bool DCTDialog::isTruncMode() const {
    return ui->truncButton->isChecked();
}

int DCTDialog::getTruncLimit() const {
    return ui->truncLimitBox->value();
}

int DCTDialog::getNbBitInit() const {
    return ui->initBitBox->value();
}

double DCTDialog::getSlope() const {
    return ui->slopeBox->value();
}
