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

#include "PyramidDialog.h"
#include "ui_PyramidDialog.h"

PyramidDialog::PyramidDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PyramidDialog)
{
    ui->setupUi(this);
}

PyramidDialog::~PyramidDialog()
{
    delete ui;
}

Pyramid::Filtre PyramidDialog::getFilter(std::string &to_print) const {
    Pyramid::Filters filters;
    Pyramid::Filtre filter;
    bool notFoundError = false;
    switch(ui->filterBox->currentIndex()) {
        case 1: notFoundError = !filters.getFromName("gaussien", filter);
                to_print = QString(qApp->translate("PyramidDialog","Decomposition pyramidale avec filtre gaussien :\n")).toStdString();
                break;
        case 2: notFoundError = !filters.getFromName("trimodal", filter);
                to_print = QString(qApp->translate("PyramidDialog","Decomposition pyramidale avec filtre trimodal :\n")).toStdString();
                break;
        case 3: notFoundError = !filters.getFromName("rectangulaire", filter);
                to_print = QString(qApp->translate("PyramidDialog","Decomposition pyramidale avec filtre rectangulaire :\n")).toStdString();
                break;
        case 4: notFoundError = !filters.getFromName("qmf", filter);
                to_print = QString(qApp->translate("PyramidDialog","Decomposition pyramidale avec filtre qmf :\n")).toStdString();
                break;
        default: notFoundError = !filters.getFromName("triangulaire", filter);
                to_print = QString(qApp->translate("PyramidDialog","Decomposition pyramidale avec filtre triangulaire :\n")).toStdString();
                break;
    }
    if(notFoundError){
        to_print = QString(qApp->translate("PyramidDialog","Filtre non trouvé. Décomposition avec filtre gaussien par défaut. \n")).toStdString();
    }
    return filter;
}

int PyramidDialog::getNbStep() {
    return ui->stepBox->value();
}

bool PyramidDialog::isGaussian() const {
    return ui->gaussianButton->isChecked();
}

bool PyramidDialog::onlyOneStep() const {
    return ui->oneStepBox->isChecked();
}

int PyramidDialog::onlyStep() const {
    return ui->onlyStepBox->value();
}

void PyramidDialog::setFilter(int a){
    ui->filterBox->setCurrentIndex(a);
}

void PyramidDialog::setOneStepChecked(bool a){
    ui->oneStepBox->setChecked(a);
}

void PyramidDialog::setOneStep(int a){
    ui->onlyStepBox->setValue(a);
}

/*Update the maximum decomposition level according to the size of the input image*/
void PyramidDialog::updateMaxLevel(int max)
{
    ui->stepBox->setMaximum(max);
}
