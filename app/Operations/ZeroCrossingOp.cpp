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
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QApplication>
#include <cmath>

#include "ZeroCrossingOp.h"

using namespace imagein;

ZeroCrossingOp::ZeroCrossingOp() : DoubleOperation(qApp->translate("Operations", "Zero crossing").toStdString())
{
}

bool ZeroCrossingOp::needCurrentImg() const {
    return true;
}

void ZeroCrossingOp::operator()(const imagein::Image_t<double>* img, const std::map<const imagein::Image_t<double>*, std::string>&) {

    auto* dialog = new QDialog(QApplication::activeWindow());
    dialog->setWindowTitle(QString(qApp->translate("ZeroCrossingOp", "Zero crossing")));
    dialog->setMinimumWidth(180);
    auto* layout = new QFormLayout();
    dialog->setLayout(layout);

    auto* thresholdBox = new QDoubleSpinBox();
    thresholdBox->setRange(0., 65536.);

    layout->insertRow(0, qApp->translate("ZeroCrossingOp", "Threshold : "), thresholdBox);

    auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal, dialog);
    layout->insertRow(3, buttonBox);
    QObject::connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), dialog, SLOT(reject()));

    auto code = static_cast<QDialog::DialogCode>(dialog->exec());

    if(code!=QDialog::Accepted) return;

    double threshold = thresholdBox->value(); //algorithm setting set by the user

    //Creation of the result image of the same size as the input one
    auto* result = new Image_t<double>(img->getWidth(), img->getHeight(), img->getNbChannels(), 0.); //image resultat

    //Processing algorithm
    //Step 1 : navigate through every pixel
    for(int c = 0; c < img->getNbChannels(); ++c) {
        for(int j = 0; j < img->getHeight(); ++j) {
            for(int i = 0; i < img->getWidth(); ++i) {
                bool edge = false;
                double currentPix = img->getPixelAt(i, j, c);

    //Step 2 : Navigate through the neighborhood of the current pîxel
                for(int k = i-1; k <= i+1; ++k) {
                    for(int l = j-1; l <= j+1; ++l) {
                        if (k >= 0 && k < img->getWidth() && l >= 0 && l < img->getHeight()) {
                            double neighborPix = img->getPixelAt(k, l, c);
                            //Step 3 : Is it an edge or not ?
                            if (currentPix <= 0 && neighborPix > 0 && std::abs(currentPix - neighborPix) > threshold) {
                                edge = true;
                            }
                        }
                    }
                }
                if(edge) result->setPixel(i, j, c, 255.);
            }
        }
    }


    //Generation of a new image with a clean of isolated edges
    auto* result2 = new Image_t<double>(*result); //image resultat

    for(unsigned int c = 0; c < result2->getNbChannels(); ++c) {
        for(unsigned int j = 1; j < result2->getHeight() - 1; ++j) {
            for(unsigned int i = 1; i < result2->getWidth() - 1; ++i) {
                if(result2->getPixelAt(i, j, c) > 127) {
                    if(result2->getPixelAt(i-1, j, c) == 0
                        && result2->getPixelAt(i, j-1, c) == 0
                        && result2->getPixelAt(i+1, j, c) == 0
                        && result2->getPixelAt(i, j+1, c) == 0
                        && result2->getPixelAt(i+1, j+1, c) == 0
                        && result2->getPixelAt(i+1, j-1, c) == 0
                        && result2->getPixelAt(i-1, j+1, c) == 0
                        && result2->getPixelAt(i-1, j-1, c) == 0) {
                        result2->setPixelAt(i, j, c, 0.);
                    }
                }
            }
        }
    }

    outDoubleImage(result, qApp->translate("ZeroCrossingOp", "contours bruts").toStdString());
    outDoubleImage(result2, qApp->translate("ZeroCrossingOp", "contours nettoyes").toStdString());
}
