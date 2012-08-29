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
#include <QDialog>
#include <QFormLayout>
#include <QRadioButton>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include "RandomImgOp.h"
#include "../Tools.h"
#include <RandomLib/Random.hpp>

using namespace std;
using namespace imagein;

RandomImgOp::RandomImgOp() : Operation(Tools::tr("Generate random image").toStdString())
{
}

void RandomImgOp::operator()(const imagein::Image*, const std::map<const imagein::Image*, std::string>&) {
    QDialog* dialog = new QDialog();
    dialog->setWindowTitle(dialog->tr("Parameters"));
    dialog->setMinimumWidth(180);
    QFormLayout* layout = new QFormLayout(dialog);

    QGroupBox* radioGroup = new QGroupBox("Image type", dialog);
    QRadioButton* intButton = new QRadioButton(dialog->tr("8-bit integer"));
    QRadioButton* floatButton = new QRadioButton(dialog->tr("Floating point"));
    QHBoxLayout* radioLayout = new QHBoxLayout(radioGroup);
    radioLayout->addWidget(intButton);
    radioLayout->addWidget(floatButton);
    intButton->setChecked(true);
    layout->insertRow(0, radioGroup);

    QSpinBox* widthBox = new QSpinBox(dialog);
    widthBox->setRange(0, 65536);
    widthBox->setValue(512);
    layout->insertRow(1, "Width : ", widthBox);

    QSpinBox* heightBox = new QSpinBox(dialog);
    heightBox->setRange(0, 65536);
    heightBox->setValue(512);
    layout->insertRow(2, "Height : ", heightBox);

    QSpinBox* channelBox = new QSpinBox(dialog);
    channelBox->setRange(1, 4);
    channelBox->setValue(3);
    layout->insertRow(3, "Number of channels : ", channelBox);

    QWidget* intRangeWidget = new QWidget(dialog);
    QHBoxLayout* intRangeLayout = new QHBoxLayout(intRangeWidget);
    QSpinBox* intMinBox = new QSpinBox(dialog);
    QSpinBox* intMaxBox = new QSpinBox(dialog);
    intMinBox->setRange(0, 255);
    intMaxBox->setRange(0, 255);
    intMinBox->setValue(0);
    intMaxBox->setValue(255);
    intRangeLayout->addWidget(new QLabel("Range : "));
    intRangeLayout->addWidget(intMinBox);
    intRangeLayout->addWidget(new QLabel(" to "));
    intRangeLayout->addWidget(intMaxBox);
    layout->insertRow(4, intRangeWidget);

    QWidget* floatRangeWidget = new QWidget(dialog);
    QHBoxLayout* floatRangeLayout = new QHBoxLayout(floatRangeWidget);
    QDoubleSpinBox* floatMinBox = new QDoubleSpinBox(dialog);
    QDoubleSpinBox* floatMaxBox = new QDoubleSpinBox(dialog);
    floatMinBox->setValue(0.0);
    floatMaxBox->setValue(1.0);
    floatMinBox->setRange(-65536, 65536);
    floatMaxBox->setRange(-65536, 65536);
    floatRangeLayout->addWidget(new QLabel("Range : "));
    floatRangeLayout->addWidget(floatMinBox);
    floatRangeLayout->addWidget(new QLabel(" to "));
    floatRangeLayout->addWidget(floatMaxBox);
    layout->insertRow(5, floatRangeWidget);
    floatRangeWidget->hide();

    layout->setSizeConstraint(QLayout::SetFixedSize);

    QObject::connect(intButton, SIGNAL(toggled(bool)), intRangeWidget, SLOT(setVisible(bool)));
    QObject::connect(floatButton, SIGNAL(toggled(bool)), floatRangeWidget, SLOT(setVisible(bool)));

    QPushButton *okButton = new QPushButton(dialog->tr("Validate"), dialog);
    okButton->setDefault(true);
    layout->addWidget(okButton);
    QObject::connect(okButton, SIGNAL(clicked()), dialog, SLOT(accept()));

    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());

    if(code!=QDialog::Accepted) {
        return;
    }

    if(intButton->isChecked()) {

        Image* resImg = new Image(widthBox->value(), heightBox->value(), channelBox->value());
        RandomLib::Random random;
        for(unsigned int c = 0; c < resImg->getNbChannels(); ++c) {
            for(unsigned int j = 0; j < resImg->getHeight(); ++j) {
                for(unsigned int i = 0; i < resImg->getWidth(); ++i) {
                    Image::depth_t value = random.IntegerC<Image::depth_t>(intMinBox->value(), intMaxBox->value());
                    resImg->setPixel(i, j, c, value);
                }
            }
        }
        this->outImage(resImg, Tools::tr("Random image").toStdString());

    }
    else if(floatButton->isChecked()) {

        Image_t<double>* resImg = new Image_t<double>(widthBox->value(), heightBox->value(), channelBox->value());
        RandomLib::Random random;
        for(unsigned int c = 0; c < resImg->getNbChannels(); ++c) {
            for(unsigned int j = 0; j < resImg->getHeight(); ++j) {
                for(unsigned int i = 0; i < resImg->getWidth(); ++i) {
                    double min = floatMinBox->value();
                    double max = floatMaxBox->value();
                    double value = random.FixedN<double>();
                    value = value*(max-min) + min;
                    resImg->setPixel(i, j, c, value);
                }
            }
        }
        this->outDoubleImage(resImg, Tools::tr("Random image").toStdString(), true);

    }
}

bool RandomImgOp::needCurrentImg() const {
    return false;
}
