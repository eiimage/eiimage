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

#include "DPCMDialog.h"
#include "ui_DPCMDialog.h"
#include <QFileDialog>
#include "QuantificationDialog.h"


DPCMDialog::DPCMDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DPCMDialog)
{
    ui->setupUi(this);
}

DPCMDialog::~DPCMDialog()
{
    delete ui;
}

void DPCMDialog::on_quantBrowseButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open file"), "", tr("Loi de quantification (*.loi)"));
    ui->quantFileEdit->setText(filename);
}

void DPCMDialog::on_quantEditorButton_clicked()
{

    QuantificationDialog* dialog = new QuantificationDialog(QApplication::activeWindow());
    dialog->exec();
}

void DPCMDialog::on_noQuantization_clicked()
{
    if(ui->noQuantization->isChecked()){
        ui->quantBrowseButton->setEnabled(false);
        ui->quantFileEdit->setEnabled(false);
        ui->quantEditorButton->setEnabled(false);
    }else{
        ui->quantBrowseButton->setEnabled(true);
        ui->quantFileEdit->setEnabled(true);
        ui->quantEditorButton->setEnabled(true);
    }
}

Quantification* DPCMDialog::getQuantification() const {
    if (ui->noQuantization->isChecked()) {
        /*Quantification constructor that enable the boolean variable noQuantif*/
        return new Quantification();
    } else {
        std::string quantFilePath = ui->quantFileEdit->text().toStdString();

        if (quantFilePath.empty()) {
            throw std::runtime_error("Quantification file path is empty.");
        }

        try {
            return new Quantification(quantFilePath);
        } catch (const std::exception& e) {
            // Si une exception est lancée dans le constructeur de Quantification, la propager plus loin
            // Va se produire si le filename est incorrect
            throw;
        }
    }
}

DPCM::Prediction DPCMDialog::getPrediction() const {
    if(ui->predictRadioA->isChecked()) return DPCM::PX_EQ_A;
    else if(ui->predictRadioAC->isChecked()) return DPCM::PX_EQ_APC;
    else if(ui->predictRadioC->isChecked()) return DPCM::PX_EQ_C;
    else if(ui->predictRadioGraham->isChecked()) return DPCM::PX_EQ_Q;
    else return DPCM::PX_EQ_A;
}

double DPCMDialog::getQ() const {
    return ui->qSpinBox->value();
}
