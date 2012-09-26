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

#include "ClassAnalysisDialog.h"
#include "ui_ClassAnalysisDialog.h"
#include <QFileDialog>

using namespace std;
using namespace imagein;

ClassAnalysisDialog::ClassAnalysisDialog(QWidget *parent, const imagein::Image* img) :
    QDialog(parent),
    ui(new Ui::ClassAnalysisDialog)
{

    _imgZoneSelector = new ImageZoneSelector(this, new Image(*img));
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    _label = new QLabel(tr("Please select the image's area to classify :"));
    ui->formLayout->addRow(_label);
    ui->formLayout->addRow(_imgZoneSelector);
    this->adjustSize();
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    connect(_imgZoneSelector, SIGNAL(selectionEmptinessChanged()), this, SLOT(checkData()));
}

ClassAnalysisDialog::~ClassAnalysisDialog()
{
    delete ui;
}

void ClassAnalysisDialog::on_fileButton_clicked()
{
    QString filename;
    if(isLearningStep()) {
        filename = QFileDialog::getSaveFileName(this, "Save classification file", "", "Classification files (*.cff)");
    }
    else {
        filename = QFileDialog::getOpenFileName(this, "Open classification file", "", "Classification files (*.cff)");
    }
    ui->fileEdit->setText(filename);
}

void ClassAnalysisDialog::on_fileEdit_textChanged(QString /*str*/) {
//    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!str.isEmpty());
    this->checkData();
}

bool ClassAnalysisDialog::isLearningStep() const {
    return ui->stepBox->currentIndex() == 0;
}

bool ClassAnalysisDialog::isClassificationStep() const {
    return ui->stepBox->currentIndex() == 1;
}
QString ClassAnalysisDialog::getFileName() const {
    return ui->fileEdit->text();
}

int ClassAnalysisDialog::getWindowSize() const {
    return ui->windowBox->value();
}

void ClassAnalysisDialog::on_stepBox_currentIndexChanged(int i) {
    _imgZoneSelector->setVisible(i == 0);
    _label->setVisible(i == 0);
    ui->formLayout->invalidate();
    ui->windowBox->setEnabled(i != 2);
    ui->windowLabel->setEnabled(i != 2);
    this->adjustSize();
}

vector<Rectangle> ClassAnalysisDialog::getSelections() const {
    return _imgZoneSelector->getSelections();
}

void ClassAnalysisDialog::checkData() {
    bool ok = !ui->fileEdit->text().isEmpty();
    if(this->isLearningStep()) {
        ok &= !_imgZoneSelector->isSelectionEmpty();
    }
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(ok);
}
