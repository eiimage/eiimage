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

#include "HistogramOp.h"
#include "../Tools.h"
#include <QDialog>
#include <QFormLayout>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QApplication>
#include <Histogram.h>

using namespace std;
using namespace imagein;

HistogramOp::HistogramOp() : Operation(qApp->translate("Operations", "Histogram operations").toStdString())
{
}

bool HistogramOp::needCurrentImg() const {
    return true;
}

void HistogramOp::operator()(const imagein::Image* img, const std::map<const imagein::Image*, std::string>&) {
    QDialog* dialog = new QDialog(QApplication::activeWindow());
    dialog->setWindowTitle(QString(qApp->translate("HistogramOp", "Histogram operations")));
    dialog->setMinimumWidth(180);
    QFormLayout* layout = new QFormLayout();
    dialog->setLayout(layout);

    QComboBox* opBox = new QComboBox();
    opBox->addItem(qApp->translate("HistogramOp", "Equalize"));
    opBox->addItem(qApp->translate("HistogramOp", "Linear stretching"));
    layout->insertRow(0, qApp->translate("HistogramOp", "Operation : "), opBox);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal, dialog);
    layout->insertRow(3, buttonBox);
    QObject::connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), dialog, SLOT(reject()));

    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());

    if(code!=QDialog::Accepted) return;

    if(opBox->currentIndex()==0) {
        Image* resImg = equalize(img);
        outImage(resImg, qApp->translate("HistogramOp", "Equalized").toStdString());
    }
    else if(opBox->currentIndex()==1) {
        Image* resImg = linearStretching(img);
        outImage(resImg, qApp->translate("HistogramOp", "Linear stretched").toStdString());
    }
}

Image* HistogramOp::equalize( const Image *img ) {

    Image* resImg = new Image(img->getWidth(), img->getHeight(), img->getNbChannels());

    for(unsigned int c = 0; c < img->getNbChannels(); ++c) {
        imagein::Rectangle rect;
        CumulatedHistogram hist_sum  = CumulatedHistogram(*img, c, rect);
        double Dm = 255; // number of levels in output image
        for(unsigned int j = 0; j < img->getHeight(); j++) {
            for(unsigned int i = 0; i < img->getWidth(); i++) {
                double K = img->getPixel(i, j, c);
                K = Dm * hist_sum[(int)K];
                if( K < 0 ) K = 0;
                if( K > 255 ) K = 255;
                resImg->setPixel(i, j, c,  K);
            }
        }
    }
    return resImg;
}




Image *HistogramOp::linearStretching( const Image *img ) {

    Image* resImg = new Image(*img);
    for(unsigned int c = 0; c < img->getNbChannels(); ++c) {
        int min = img->min(c);
        int max = img->max(c);
        for(unsigned int j = 0; j < img->getHeight(); ++j) {
            for(unsigned int i = 0; i < img->getWidth(); ++i) {
                double K = img->getPixel(i, j, c);
                if( (max-min)!=0.) {
                    resImg->setPixel(i, j, c, (255*K-255*min)/(max-min));
                }
                else {
                    resImg->setPixel(i, j, c, K);
                }
            }
        }
    }
    return resImg;
}
