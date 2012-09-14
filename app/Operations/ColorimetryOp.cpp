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
#include "ColorDialog.h"
using namespace imagein;

ColorimetryOp::ColorimetryOp() : Operation(qApp->translate("Operations", "Colorimetry").toStdString())
{
}

bool ColorimetryOp::needCurrentImg() const {
    return false;
}

void ColorimetryOp::operator()(const imagein::Image*, const std::map<const imagein::Image*, std::string>&) {

    ColorDialog* dialog = new ColorDialog(QApplication::activeWindow());
    dialog->setWindowTitle(QString(qApp->translate("Operations", "Colorimetry")));

    if(code!=QDialog::Accepted) return;
    Image* resImg = new Image(512, 512, 3);
    for(unsigned int j = 0; j < resImg->getHeight(); ++j) {
        for(unsigned int i = 0; i < resImg->getWidth(); ++i) {
            resImg->setPixelAt(i, j, 0, color.red());
        }
    }
    for(unsigned int j = 0; j < resImg->getHeight(); ++j) {
        for(unsigned int i = 0; i < resImg->getWidth(); ++i) {
            resImg->setPixelAt(i, j, 1, color.green());
        }
    }
    for(unsigned int j = 0; j < resImg->getHeight(); ++j) {
        for(unsigned int i = 0; i < resImg->getWidth(); ++i) {
            resImg->setPixelAt(i, j, 2, color.blue());
        }
    }
    outImage(resImg, "RGB Image");
}
