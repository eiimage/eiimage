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

#include "QuantificationDialog.h"
#include <QDialog>
#include <QFormLayout>
#include <QComboBox>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <GrayscaleImage.h>
#include <QGroupBox>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>

using namespace imagein;
QuantificationDialog::QuantificationDialog(QWidget *parent, QString imgName) :
    QDialog(parent), _editorOnly(imgName.isEmpty())
{
    if(_editorOnly) {
       this->setWindowTitle(tr("Quantification file editor"));
    }
    else {
       this->setWindowTitle(tr("Quantification of %1").arg(imgName));
    }
    setMinimumWidth(180);
    QFormLayout* layout = new QFormLayout(this);
    setLayout(layout);
    _sizeBox = new QSpinBox();
    _sizeBox->setRange(2, 256);
    _sizeBox->setValue(2);

    _quantBox = new QComboBox();
    _quantBox->addItem(tr("Linear with centered value"));
    if(!_editorOnly) {
        _quantBox->addItem(tr("Non linear with centered value"));
        _quantBox->addItem(tr("Non linear with mean value"));
    }
    _quantBox->addItem(tr("LloydMax"));
    _quantBox->addItem(tr("Custom"));

    layout->insertRow(0, tr("Quantification : "), _quantBox);
    layout->insertRow(1, tr("Number of values : "), _sizeBox);

    _editorWidget = new QWidget(this);
    QHBoxLayout* editorLayout = new QHBoxLayout(_editorWidget);
    _quantWidget = new QuantificationWidget(this);
    editorLayout->addStretch();
    editorLayout->addWidget(_quantWidget);
    editorLayout->addStretch();

    layout->insertRow(2, _editorWidget);
    _editorWidget->setVisible(false);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel|QDialogButtonBox::Open|QDialogButtonBox::Save, Qt::Horizontal, this);
    layout->insertRow(3, buttonBox);
//    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    QObject::connect(buttonBox->button(QDialogButtonBox::Ok), SIGNAL(pressed()), this, SLOT(accept()));
    QObject::connect(buttonBox->button(QDialogButtonBox::Open), SIGNAL(pressed()), this, SLOT(open()));
    QObject::connect(buttonBox->button(QDialogButtonBox::Save), SIGNAL(pressed()), this, SLOT(save()));
    _saveButton = buttonBox->button(QDialogButtonBox::Save);
//    _saveButton->setEnabled(false);
    QObject::connect(_sizeBox, SIGNAL(valueChanged(int)), _quantWidget, SLOT(setNbThreshold(int)));
    QObject::connect(_quantBox, SIGNAL(currentIndexChanged(int)), this, SLOT(methodChanged(int)));

    if(_editorOnly) {
        buttonBox->button(QDialogButtonBox::Cancel)->setVisible(false);
        buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Exit"));
    }
    else {
        buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Apply"));
    }
}

void QuantificationDialog::methodChanged(int method) {
    _editorWidget->setVisible((_editorOnly && method == 2) || (!_editorOnly && method == 4));
    _saveButton->setEnabled(_quantBox->currentText()==tr("Linear with centered value") || _quantBox->currentText()==tr("Custom"));
    this->adjustSize();
}

Quantification QuantificationDialog::getQuantif(const Image* image, unsigned int c, std::string &to_print) {

    int size = _sizeBox->value();
    if(_editorOnly) return Quantification::linearQuant(size);
    switch(_quantBox->currentIndex()) {
        case 1:
                to_print = QString(tr("Quantification non lineaire a valeurs centrees :")).toStdString();
                return Quantification::nonLinearQuant(size, image, c);
                break;
        case 2:
                to_print = QString(tr("Quantification non lineaire a valeurs moyennes :")).toStdString();
                return Quantification::nonLinearQuantOptimized(size, image, c);
                break;
        case 3:
                to_print = QString(tr("Quantification LloydMax :")).toStdString();
                return Quantification::lloydMaxQuant(size, image, c);
                break;
        case 4:
                to_print = QString(tr("Quantification personnalisee :")).toStdString();
                return _quantWidget->getQuantif();
                break;

        default:
                to_print = QString(tr("Quantification lineaire a valeurs centrees :")).toStdString();
                return Quantification::linearQuant(size);
                break;
    }
}

Quantification QuantificationDialog::getQuantif() {

    int size = _sizeBox->value();
    if(_editorOnly) return Quantification::linearQuant(size);
    switch(_quantBox->currentIndex()) {
        case 4:
                return _quantWidget->getQuantif();
                break;
        default:
                return Quantification::linearQuant(size); break;
    }
}

void QuantificationDialog::open() {

    QString filename = QFileDialog::getOpenFileName(this, tr("Open a file"), "", tr("Loi de quantification (*.loi)"));
    if(filename.isEmpty()) return;
    Quantification q(filename.toStdString());
    _quantWidget->setQuantif(q);
    _sizeBox->setValue(q.nbValues());
    _quantBox->setCurrentText(tr("Custom"));
}

void QuantificationDialog::save() {

    QString filename = QFileDialog::getSaveFileName(this, tr("Save to file"), "", tr("Loi de quantification (*.loi)"));
    if(filename.isEmpty()) return;
    if(_quantBox->currentIndex() == 0) {
        Quantification::linearQuant(this->_sizeBox->value()).saveAs(filename.toStdString());
    }
    else if((_editorOnly && _quantBox->currentIndex() == 2) || (!_editorWidget && _quantBox->currentIndex() == 4)) {
        _quantWidget->getQuantif().saveAs(filename.toStdString());
    }
}

void QuantificationDialog::setValues(int a){
  _sizeBox->setValue(a);
}

void QuantificationDialog::setQuantif(int a){
  _quantBox->setCurrentIndex(a);
}
