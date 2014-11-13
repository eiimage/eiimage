/*
 * Copyright 2011-2012 INSA Rennes
 *
 * This file is part of EIImage.
 *
 * EIImage is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EIImage is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EIImage.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "MICDDialog.h"
#include "ui_MICDDialog.h"
#include <QFileDialog>
#include "QuantificationDialog.h"

MICDDialog::MICDDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MICDDialog)
{
    ui->setupUi(this);
}

MICDDialog::~MICDDialog()
{
    delete ui;
}

void MICDDialog::on_quantBrowseButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open file"), "", tr("Loi de quantification (*.loi)"));
    ui->quantFileEdit->setText(filename);
}

void MICDDialog::on_quantEditorButton_clicked()
{

    QuantificationDialog* dialog = new QuantificationDialog(QApplication::activeWindow());
    dialog->exec();
}

Quantification* MICDDialog::getQuantification() const {
    try {
        Quantification* quantif = new Quantification(ui->quantFileEdit->text().toStdString());
        return quantif;
    }
    catch(std::exception&) {
        return NULL;
    }
}

MICD::Prediction MICDDialog::getPrediction() const {
    if(ui->predictRadioA->isChecked()) return MICD::PX_EQ_A;
    else if(ui->predictRadioAC->isChecked()) return MICD::PX_EQ_B;
    else if(ui->predictRadioC->isChecked()) return MICD::PX_EQ_APC;
    else if(ui->predictRadioGraham->isChecked()) return MICD::PX_EQ_Q;
    else return MICD::PX_EQ_A;
}

double MICDDialog::getQ() const {
    return ui->qSpinBox->value();
}
