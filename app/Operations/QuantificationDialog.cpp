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
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>


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
    _sizeBox->setRange(2,999);
    _sizeBox->setValue(2);

    _quantBox = new QComboBox();
    _quantBox->addItem(tr("Linear with centered value"));
    if(!_editorOnly) {
        _quantBox->addItem(tr("Non linear with centered value"));
        _quantBox->addItem(tr("Non linear with mean value"));
        _quantBox->addItem(tr("LloydMax"));
    }
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

/*SpinBox pour demander à l'utilisateur de renseigner l'interval sur lequel sera réalisé la quantification*/
    auto *spinBoxLayout = new QHBoxLayout();
    _limitQuantifBox1 = new QSpinBox();
    _limitQuantifBox2 = new QSpinBox();

// Par défaut, on quantifie entre 0 et 255 et on impose pas de valeur max pour les bornes
    _limitQuantifBox1->setRange(-999, 999);
    _limitQuantifBox2->setRange(-999, 999);
    _limitQuantifBox1->setValue(0);
    _limitQuantifBox2->setValue(255);

// Réduction de la taille des spinBox
    _limitQuantifBox1->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    _limitQuantifBox2->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

    spinBoxLayout->addWidget(_limitQuantifBox1);
    spinBoxLayout->addWidget(_limitQuantifBox2);

    layout->addRow(tr("Quantification limits:"), spinBoxLayout);


    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel|QDialogButtonBox::Open|QDialogButtonBox::Save, Qt::Horizontal, this);
    layout->insertRow(4, buttonBox);


//    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    QObject::connect(buttonBox->button(QDialogButtonBox::Ok), SIGNAL(pressed()), this, SLOT(accept()));
    QObject::connect(buttonBox->button(QDialogButtonBox::Open), SIGNAL(pressed()), this, SLOT(open()));
    QObject::connect(buttonBox->button(QDialogButtonBox::Save), SIGNAL(pressed()), this, SLOT(save()));
    _saveButton = buttonBox->button(QDialogButtonBox::Save);
//    _saveButton->setEnabled(false);
    QObject::connect(_sizeBox, SIGNAL(valueChanged(int)), _quantWidget, SLOT(setNbThreshold(int)));
    QObject::connect(_quantBox, SIGNAL(currentIndexChanged(int)), this, SLOT(methodChanged(int)));
    QObject::connect(_limitQuantifBox1, SIGNAL(valueChanged(int)), _quantWidget, SLOT(setLimitQuantifmin(int)));
    QObject::connect(_limitQuantifBox2, SIGNAL(valueChanged(int)), _quantWidget, SLOT(setLimitQuantifmax(int)));



    if(_editorOnly) {
        buttonBox->button(QDialogButtonBox::Cancel)->setVisible(false);
        buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Exit"));
    }
    else {
        buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Apply"));
    }
}

void QuantificationDialog::methodChanged(int method) {
    _editorWidget->setVisible((_editorOnly && method == 1) || (!_editorOnly && method == 4));
    _saveButton->setEnabled(_quantBox->currentText()==tr("Linear with centered value") || _quantBox->currentText()==tr("Custom"));

    /*Si l'on est en mode personnalisé, on désactive les boxes pour renseigner les bornes de la quantification*/
    if(_quantBox->currentIndex() == 4) {
        _limitQuantifBox1->setEnabled(false);
        _limitQuantifBox2->setEnabled(false);
    }
    this->adjustSize();
}

Quantification QuantificationDialog::getQuantif(const genericinterface::ImageWindow *currentWnd, unsigned int c, std::string &to_print) {

    int size = _sizeBox->value();
    int threshold_a = _limitQuantifBox1->value();
    int threshold_b = _limitQuantifBox2->value();

    if(size>(threshold_b-threshold_a+1)){
        QMessageBox messageBox;
        messageBox.information(nullptr,"Information : ","Be carefull, the range of quantification is higher than the range of color value in the input image");
        messageBox.setFixedSize(500,200);
    }

    if(_editorOnly) return Quantification::linearQuant(size,threshold_a,threshold_b);
    switch(_quantBox->currentIndex()) {
        case 1:
                to_print = QString(tr("Quantification non lineaire a valeurs centrees :")).toStdString();
                return Quantification::nonLinearQuant(size,threshold_a,threshold_b, currentWnd, c);
        case 2:
                to_print = QString(tr("Quantification non lineaire a valeurs moyennes :")).toStdString();
                return Quantification::nonLinearQuantOptimized(size,threshold_a,threshold_b, currentWnd, c);

        case 3:
                to_print = QString(tr("Quantification LloydMax :")).toStdString();
                return Quantification::lloydMaxQuant(size,threshold_a,threshold_b, currentWnd, c);
        case 4:
                to_print = QString(tr("Quantification personnalisee :")).toStdString();
                return _quantWidget->getQuantif();

        default:
                to_print += QString(tr("Quantification lineaire a valeurs centrees :")).toStdString();
                return Quantification::linearQuant(size,threshold_a,threshold_b);
    }
}

Quantification QuantificationDialog::getQuantif() {

    int size = _sizeBox->value();
    int threshold_a = _limitQuantifBox1->value();
    int threshold_b = _limitQuantifBox1->value();
    if(_editorOnly) return Quantification::linearQuant(size,threshold_a,threshold_b);
    switch(_quantBox->currentIndex()) {
        case 4:
                return _quantWidget->getQuantif();
        default:
                return Quantification::linearQuant(size,threshold_a,threshold_b);
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

    /*Sur linux, l'extension ne se met pas toute seule comme sur windows*/
    QString extension = ".loi";
    if (!filename.endsWith(extension, Qt::CaseInsensitive)) {
        filename += extension;
    }

    if(filename.isEmpty()) return;
    if(_quantBox->currentIndex() == 0) {
        Quantification::linearQuant(this->_sizeBox->value(),this->_limitQuantifBox1->value(),this->_limitQuantifBox2->value()).saveAs(filename.toStdString());
    }
    else if((_editorOnly && _quantBox->currentIndex() == 1) || (!_editorOnly && _quantBox->currentIndex() == 4)) {
        _quantWidget->getQuantif().saveAs(filename.toStdString());
    }
}

void QuantificationDialog::setValues(int a){
  _sizeBox->setValue(a);
}

void QuantificationDialog::setQuantif(int a){
  _quantBox->setCurrentIndex(a);
}
