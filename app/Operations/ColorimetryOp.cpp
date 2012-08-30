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

#include "ColorimetryOp.h"
#include "../Tools.h"
#include <QDialog>
#include <QFormLayout>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QApplication>
#include <QSlider>
#include <QLabel>
#include <QColorDialog>

ColorimetryOp::ColorimetryOp() : Operation(Tools::tr("Colorimetry").toStdString())
{
}

bool ColorimetryOp::needCurrentImg() const {
    return false;
}

void ColorimetryOp::operator()(const imagein::Image*, const std::map<const imagein::Image*, std::string>&) {

    QDialog* dialog = new QDialog(QApplication::activeWindow());
    dialog->setModal(false);
    dialog->setWindowTitle(QString(dialog->tr("Zero crossing")));
    dialog->setMinimumWidth(180);
    QGridLayout* layout = new QGridLayout();
    dialog->setLayout(layout);

    QSpinBox* redBox = new QSpinBox();
    QSpinBox* greenBox = new QSpinBox();
    QSpinBox* blueBox = new QSpinBox();
    QSpinBox* hueBox = new QSpinBox();
    QSpinBox* saturationBox = new QSpinBox();
    QSpinBox* valueBox = new QSpinBox();
    QSlider* redSlider = new QSlider(Qt::Horizontal);
    QSlider* greenSlider = new QSlider(Qt::Horizontal);
    QSlider* blueSlider = new QSlider(Qt::Horizontal);
    QSlider* hueSlider = new QSlider(Qt::Horizontal);
    QSlider* saturationSlider = new QSlider(Qt::Horizontal);
    QSlider* valueSlider = new QSlider(Qt::Horizontal);

    redBox->setRange(0, 255);
    greenBox->setRange(0, 255);
    blueBox->setRange(0, 255);
    hueBox->setRange(0, 360);
    saturationBox->setRange(0, 100);
    valueBox->setRange(0, 100);
    redSlider->setRange(0, 255);
    greenSlider->setRange(0, 255);
    blueSlider->setRange(0, 255);
    hueSlider->setRange(0, 360);
    saturationSlider->setRange(0, 100);
    valueSlider->setRange(0, 100);
    layout->addWidget(new QLabel(dialog->tr("Red : ")), 0, 0);
    layout->addWidget(new QLabel(dialog->tr("Green : ")), 1, 0);
    layout->addWidget(new QLabel(dialog->tr("Blue : ")), 2, 0);
    layout->addWidget(new QLabel(dialog->tr("Hue : ")), 3, 0);
    layout->addWidget(new QLabel(dialog->tr("Saturation : ")), 4, 0);
    layout->addWidget(new QLabel(dialog->tr("Value : ")), 5, 0);
    layout->addWidget(redSlider, 0, 1);
    layout->addWidget(greenSlider, 1, 1);
    layout->addWidget(blueSlider, 2, 1);
    layout->addWidget(hueSlider, 3, 1);
    layout->addWidget(saturationSlider, 4, 1);
    layout->addWidget(valueSlider, 5, 1);
    layout->addWidget(redBox, 0, 2);
    layout->addWidget(greenBox, 1, 2);
    layout->addWidget(blueBox, 2, 2);
    layout->addWidget(hueBox, 3, 2);
    layout->addWidget(saturationBox, 4, 2);
    layout->addWidget(valueBox, 5, 2);

    QObject::connect(redBox, SIGNAL(valueChanged(int)), redSlider, SLOT(setValue(int)));
    QObject::connect(greenBox, SIGNAL(valueChanged(int)), greenSlider, SLOT(setValue(int)));
    QObject::connect(blueBox, SIGNAL(valueChanged(int)), blueSlider, SLOT(setValue(int)));
    QObject::connect(hueBox, SIGNAL(valueChanged(int)), hueSlider, SLOT(setValue(int)));
    QObject::connect(saturationBox, SIGNAL(valueChanged(int)), saturationSlider, SLOT(setValue(int)));
    QObject::connect(valueBox, SIGNAL(valueChanged(int)), valueSlider, SLOT(setValue(int)));
    QObject::connect(redSlider, SIGNAL(valueChanged(int)), redBox, SLOT(setValue(int)));
    QObject::connect(greenSlider, SIGNAL(valueChanged(int)), greenBox, SLOT(setValue(int)));
    QObject::connect(blueSlider, SIGNAL(valueChanged(int)), blueBox, SLOT(setValue(int)));
    QObject::connect(hueSlider, SIGNAL(valueChanged(int)), hueBox, SLOT(setValue(int)));
    QObject::connect(saturationSlider, SIGNAL(valueChanged(int)), saturationBox, SLOT(setValue(int)));
    QObject::connect(valueSlider, SIGNAL(valueChanged(int)), valueBox, SLOT(setValue(int)));

    QColorDialog::getColor();

//    dialog->show();

//    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal, dialog);
//    layout->insertRow(6, buttonBox);
//    QObject::connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));
//    QObject::connect(buttonBox, SIGNAL(rejected()), dialog, SLOT(reject()));

//    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());

//    if(code!=QDialog::Accepted) return;
}
