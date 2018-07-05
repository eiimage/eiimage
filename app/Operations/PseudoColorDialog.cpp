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

#include "PseudoColorDialog.h"
#include <Converter.h>
#include <QDialog>
#include <QDialogButtonBox>
#include <QString>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QLabel>
#include <QLayout>
#include "../Tools.h"


PseudoColorDialog::PseudoColorDialog(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowTitle(qApp->translate("Operations","Pseudocolor"));
    this->setMinimumWidth(180);

    QFormLayout* layout = new QFormLayout(this);

    QHBoxLayout* hLayout = new QHBoxLayout();
    QLabel* label = new QLabel(qApp->translate("Operations","Number of colors"));
    label->setLayout(hLayout);
    _spinBox = new QSpinBox();
    _spinBox->setMinimum(0);
    _spinBox->setMaximum(300);
    _spinBox->setValue(255);
    _spinBox->setLayout(hLayout);


    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal, this);

    layout->addRow(label, _spinBox);
    layout->insertRow(3, buttonBox);
    layout->setSizeConstraint(QLayout::SetFixedSize);


    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

int PseudoColorDialog::getHue(){
    return _spinBox->value();
}



