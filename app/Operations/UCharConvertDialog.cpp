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
    this->setMinimumWidth(180);

    QFormLayout* layout = new QFormLayout(this);

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

    layout->addRow(label1, _comboBox);
    layout->addRow(_label2, _spinBox);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal, this);
    layout->insertRow(3, buttonBox);
    layout->setSizeConstraint(QLayout::SetFixedSize);


    QObject::connect(_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(enableOffset(int)));
    QObject::connect(_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeDescription(int)));
    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    changeDescription(0);
}

void UCharConvertDialog::changeDescription(int a){
    _label2->setText("ton text");
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
