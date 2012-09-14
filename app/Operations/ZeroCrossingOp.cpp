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
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QApplication>

#include "ZeroCrossingOp.h"
#include "../Tools.h"

using namespace std;
using namespace imagein;

ZeroCrossingOp::ZeroCrossingOp() : DoubleOperation(qApp->translate("Operations", "Zero crossing").toStdString())
{
}

bool ZeroCrossingOp::needCurrentImg() const {
    return true;
}

void ZeroCrossingOp::operator()(const imagein::Image_t<double>* img, const std::map<const imagein::Image_t<double>*, std::string>&) {

    QDialog* dialog = new QDialog(QApplication::activeWindow());
    dialog->setWindowTitle(QString(qApp->translate("ZeroCrossingOp", "Zero crossing")));
    dialog->setMinimumWidth(180);
    QFormLayout* layout = new QFormLayout();
    dialog->setLayout(layout);

    QDoubleSpinBox* thresholdBox = new QDoubleSpinBox();
    thresholdBox->setRange(0., 65536.);

    layout->insertRow(0, qApp->translate("ZeroCrossingOp", "Threshold : "), thresholdBox);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal, dialog);
    layout->insertRow(3, buttonBox);
    QObject::connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), dialog, SLOT(reject()));

    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());

    if(code!=QDialog::Accepted) return;

    double threshold = thresholdBox->value(); //parametre de l'algorithme

    //Creation de l'image Résultat, de même taille que les images de départ
    Image_t<double>* result = new Image_t<double>(img->getWidth(), img->getHeight(), img->getNbChannels(), 0.); //image resultat

    //Algorithme de traitement
    for(unsigned int c = 0; c < img->getNbChannels(); ++c) {
        for(unsigned int j = 2; j < img->getHeight() - 2; ++j) {
            for(unsigned int i = 2; i < img->getWidth() - 2; ++i) {
                bool edge = false;

                for(unsigned int k = i-1; k < i+1; ++k) {
                    for(unsigned int l = j-1; l < j+1; ++l) {
                        if(img->getPixelAt(i, j, c) <= 0 && img->getPixelAt(k, l, c) > 0) {
                            double dist = abs(img->getPixelAt(i, j, c) - img->getPixelAt(k, l, c));
                            if(dist > threshold) edge = true;
                        }
                    }
                }

                if(edge) result->setPixel(i, j, c, 255.);
            }
        }
    }

    // Maintenant, on élimine les contours isolés.
    Image_t<double>* result2 = new Image_t<double>(*result); //image resultat

    for(unsigned int c = 0; c < result2->getNbChannels(); ++c) {
        for(unsigned int j = 2; j < result2->getHeight() - 2; ++j) {
            for(unsigned int i = 2; i < result2->getWidth() - 2; ++i) {
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
