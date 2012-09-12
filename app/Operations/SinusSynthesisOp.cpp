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

#include "SinusSynthesisOp.h"
#include "../Tools.h"
#include <QDialog>
#include <QFormLayout>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QApplication>
#include <GrayscaleImage.h>
#include <QGroupBox>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QLabel>

using namespace std;
using namespace imagein;

SinusSynthesisOp::SinusSynthesisOp() : Operation(Tools::tr("Sinus synthesis").toStdString())
{
}

bool SinusSynthesisOp::needCurrentImg() const {
    return false;
}

void SinusSynthesisOp::operator()(const imagein::Image*, const std::map<const imagein::Image*, std::string>&) {

    QDialog* dialog = new QDialog(QApplication::activeWindow());
    dialog->setWindowTitle(QString(dialog->tr("Sinus synthesis")));
    dialog->setMinimumWidth(180);
    QFormLayout* layout = new QFormLayout();
    dialog->setLayout(layout);

    QGroupBox* radioGroup = new QGroupBox("Direction", dialog);
    QRadioButton* linearButton = new QRadioButton(dialog->tr("Linear"));
    QRadioButton* circularButton = new QRadioButton(dialog->tr("Circular"));
    QHBoxLayout* radioLayout = new QHBoxLayout(radioGroup);
    radioLayout->addWidget(linearButton);
    radioLayout->addWidget(circularButton);
    linearButton->setChecked(true);

    QSpinBox* sizeBox = new QSpinBox();
    sizeBox->setRange(0, 65536);
    QSpinBox* periodBox = new QSpinBox();
    periodBox->setRange(0, 65536);
    QDoubleSpinBox* angleBox = new QDoubleSpinBox();
    angleBox->setRange(0, 359);

    QComboBox* colorBox = new QComboBox();
    colorBox->addItem(dialog->tr("256"));
    colorBox->addItem(dialog->tr("2 (Black and white)"));
    layout->insertRow(0, radioGroup);
    layout->insertRow(1, dialog->tr("Image size (width=height) : "), sizeBox);
    layout->insertRow(2, dialog->tr("Signal period (pixel) : "), periodBox);
    QLabel* orientationLabel = new QLabel(dialog->tr("Orientation (°): "));
    layout->insertRow(3, orientationLabel, angleBox);
    layout->insertRow(4, dialog->tr("Niveaux de gris : "), colorBox);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal, dialog);
    layout->insertRow(5, buttonBox);
    QObject::connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), dialog, SLOT(reject()));


    QObject::connect(circularButton, SIGNAL(toggled(bool)), orientationLabel, SLOT(setHidden(bool)));
    QObject::connect(circularButton, SIGNAL(toggled(bool)), angleBox, SLOT(setHidden(bool)));
    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());

    if(code!=QDialog::Accepted) return;

    GrayscaleImage* resImg = new GrayscaleImage(sizeBox->value(), sizeBox->value());

    double angle = angleBox->value();
    double period = periodBox->value();
    double pi=3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117;
    if(linearButton->isChecked()) {
        if(colorBox->currentIndex() == 0) {
            for(unsigned int j = 0; j < resImg->getHeight(); ++j) {
                for(unsigned int i = 0; i < resImg->getWidth(); ++i) {
                    double arg = i * sin(angle * pi / 180.) + j * cos(angle * pi / 180.);
                    double value = cos(2 * pi * arg / period);
                    value = (value + 1.) * 128;
                    if(value > 255.) value = 255.;
                    resImg->setPixel(i, j, value + 0.5);
                }
            }
        }
        else{
            for(unsigned int j = 0; j < resImg->getHeight(); ++j) {
                for(unsigned int i = 0; i < resImg->getWidth(); ++i) {
                    double arg = i * sin(angle / 180. * pi) + j * cos(angle / 180. * pi);
                    double value = cos(2 * pi * arg / period);
                    resImg->setPixel(i, j, value < 0. ? 0 : 255);
                }
            }
        }
    }
    else {
        if(colorBox->currentIndex() == 0) {
            for(unsigned int j = 0; j < resImg->getHeight(); ++j) {
                for(unsigned int i = 0; i < resImg->getWidth(); ++i) {
                    double dx = i - resImg->getWidth()/2.;
                    double dy = j - resImg->getHeight()/2.;
                    double arg = std::sqrt(dx*dx + dy*dy);
                    double value = cos(2 * pi * arg / period);
                    value = (value + 1.) * 128;
                    if(value > 255.) value = 255.;
                    resImg->setPixel(i, j, value + 0.5);
                }
            }
        }
        else{
            for(unsigned int j = 0; j < resImg->getHeight(); ++j) {
                for(unsigned int i = 0; i < resImg->getWidth(); ++i) {
                    double dx = i - resImg->getWidth()/2.;
                    double dy = j - resImg->getHeight()/2.;
                    double arg = std::sqrt(dx*dx + dy*dy);
                    double value = cos(2 * pi * arg / period);
                    resImg->setPixel(i, j, value < 0. ? 0 : 255);
                }
            }
        }
    }
    outImage(resImg, "Sinus synthesis");


}
