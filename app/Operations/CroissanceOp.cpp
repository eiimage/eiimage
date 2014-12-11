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

#include <QDialog>
#include <QFormLayout>
#include <QComboBox>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QApplication>

#include "CroissanceOp.h"
#include "../Tools.h"
#include "../Algorithms/Croissance.h"
#include <GrayscaleImage.h>
#include <Converter.h>

using namespace std;
using namespace imagein;


CroissanceOp::CroissanceOp() : Operation(qApp->translate("Operations", "Croissance").toStdString())
{
}

bool CroissanceOp::needCurrentImg() const {
    return true;
}

void CroissanceOp::operator()(const imagein::Image* img, const std::map<const imagein::Image*, std::string>&) {

    QDialog* dialog = new QDialog(QApplication::activeWindow());
    dialog->setWindowTitle(QString(qApp->translate("Operations", "Croissance")));
    dialog->setMinimumWidth(180);
    QFormLayout* layout = new QFormLayout();
    dialog->setLayout(layout);


//    layout->setSizeConstraint(QLayout::SetFixedSize);

    QSpinBox* thresholdBox = new QSpinBox();
    thresholdBox->setRange(0, 255);
    QComboBox* initBox = new QComboBox();
    initBox->addItem(qApp->translate("CroissanceOp", "At origin"));
    initBox->addItem(qApp->translate("CroissanceOp", "Point of lowest luminance"));
    QComboBox* stopBox = new QComboBox();
    stopBox->addItem(qApp->translate("CroissanceOp", "| current - mean | < threshold"));
    stopBox->addItem(qApp->translate("CroissanceOp", "| current - initial | < threshold"));


    layout->insertRow(0, qApp->translate("CroissanceOp", "Threshold : "), thresholdBox);
    layout->insertRow(1, qApp->translate("CroissanceOp", "Initial germ : "), initBox);
    layout->insertRow(2, qApp->translate("CroissanceOp", "Stopping point : "), stopBox);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal, dialog);
    layout->insertRow(3, buttonBox);
    QObject::connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), dialog, SLOT(reject()));

    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());

    if(code!=QDialog::Accepted) return;

    GrayscaleImage* image = Converter<GrayscaleImage>::convert(*img);

    Croissance cr;
    Image *lum, *color;
    int nbRegion;
    int threshold = thresholdBox->value();
    if(initBox->currentIndex() == 0) {
        if(stopBox->currentIndex() == 0) {
            nbRegion = cr.croissance1a(image, threshold, &lum, &color);
        }
        else if(stopBox->currentIndex() == 1) {
            nbRegion = cr.croissance1b(image, threshold, &lum, &color);
        }
    }
    else if (initBox->currentIndex() == 1) {
        if(stopBox->currentIndex() == 0) {
            nbRegion = cr.croissance2a(image, threshold, &lum, &color);
        }
        else if(stopBox->currentIndex() == 1) {
            nbRegion = cr.croissance2b(image, threshold, &lum, &color);
        }
    }
    outImage(lum, qApp->translate("CroissanceOp", "Luminance").toStdString());
    outImage(color, qApp->translate("CroissanceOp", "Color").toStdString());
    outText(qApp->translate("CroissanceOp", "Total number of area : %1").arg(nbRegion).toStdString());
    outText(qApp->translate("CroissanceOp", "Mean number of point per area : %1").arg((double)image->getWidth()*image->getHeight()/nbRegion).toStdString());
    delete image;
}

