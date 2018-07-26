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

#include "UCharConvertDialog.h"
#include "ui_ucharconvertdialog.h"
#include <Converter.h>
#include <QDialog>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QStringList>
#include <QString>
#include "../Tools.h"


UCharConvertDialog::UCharConvertDialog(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowTitle(qApp->translate("Operations","Convert to UChar"));
    this->setMinimumHeight(180);

    QGridLayout* gridLayout = new QGridLayout(this);

    QFormLayout* layout = new QFormLayout();

    QStringList text = (QStringList() << qApp->translate("Operations","Crop") << qApp->translate("Operations","Normalize") << qApp->translate("Operations","Add Offset") << qApp->translate("Operations","Scale") << qApp->translate("Operations","Add offset and scale"));
    QLabel* label1 = new QLabel(qApp->translate("Operations","Operation"));
    _comboBox = new QComboBox();
    _comboBox->addItems(text);

    _label2 = new QLabel("Offset");
    _spinBox = new QSpinBox();
    _spinBox->setMaximum(255);
    _spinBox->setMinimum(0);
    _spinBox->setValue(127);
    _spinBox->setEnabled(false);
    _label2->setEnabled(false);
    _spinBox->setVisible(false);
    _label2->setVisible(false);

    _label3 = new QLabel("");
    _label3->setWordWrap(true);

    layout->addRow(label1, _comboBox);
    layout->addRow(_label2, _spinBox);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal, this);
    layout->setSizeConstraint(QLayout::SetFixedSize);

    gridLayout->addLayout(layout,1,1);
    gridLayout->addWidget(_label3,2,1);
    gridLayout->addWidget(buttonBox,3,1);


    QObject::connect(_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(enableOffset(int)));
    QObject::connect(_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeDescription(int)));
    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    changeDescription(0);
}

void UCharConvertDialog::changeDescription(int a){
    switch(a)
    {
        case 0:
            _label3->setText(qApp->translate("Operations","<i>Rogner : Ne garde que les valeurs comprises entre 0 et 255</i>"));
            break;
        case 1:
            _label3->setText(qApp->translate("Operations","<i>Normaliser : Mise à l'echelle des valeurs entre 0 et 255 \n(Opération utilisée pour la visualisation des images doubles)</i>"));
            break;
        case 2: 
            _label3->setText(qApp->translate("Operations","<i>Ajouter un décalage : décale tous les pixels de l'image de\n la valeur séléctionnée ci-dessus</i>"));
            break;
        case 3:
            _label3->setText(qApp->translate("Operations","<i>Mettre à l'echelle : Ne garde que les valeurs positives et\n effectue une mise à l'échelle entre 0 et 255</i>"));
            break;
        case 4:
            _label3->setText(qApp->translate("Operations","<i>Décaler et mettre à l'echelle : Ajoute un décalge de 127 et\n effectue une mise à l'echelle</i>"));
            break;
        default:
            _label3->setText(qApp->translate("Operations","<i>Rogner : Ne garde que les valeurs comprises entre 0 et 255</i>"));
            break;
    }
}

void UCharConvertDialog::enableOffset(int n){
    if(n==2){
        _spinBox->setEnabled(true);
        _label2->setEnabled(true);
        _spinBox->setVisible(true);
        _label2->setVisible(true);
    }else if(n==4){
        _spinBox->setEnabled(false);
        _label2->setEnabled(false);
        _spinBox->setValue(127);
        _spinBox->setVisible(true);
        _label2->setVisible(true);
    }else{
        _spinBox->setEnabled(false);
        _label2->setEnabled(false);
        _spinBox->setVisible(false);
        _label2->setVisible(false);
    }
}

int UCharConvertDialog::getCombo(){
    return _comboBox->currentIndex();
}

int UCharConvertDialog::getOffset(){
    return _spinBox->value();
}

QString UCharConvertDialog::getText(){
    return _comboBox->currentText();
}
