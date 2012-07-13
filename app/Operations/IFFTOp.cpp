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
#include <QLabel>
#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include "ImageListBox.h"

#include "IFFTOp.h"
#include "ImgWidget.h"
#include "../Tools.h"
#include "../Algorithms/FFT.h"
#include <cmath>

using namespace std;
using namespace imagein;

IFFTOp::IFFTOp() : DoubleOperation(Tools::tr("Discrete Fourier reconstruction").toStdString())
{
}

bool IFFTOp::needCurrentImg() const {
    return false;
}

void IFFTOp::operator()(const imagein::Image_t<double>*, const map<const imagein::Image_t<double>*, string>& imgList) {

    QDialog* dialog = new QDialog();
    dialog->setWindowTitle(dialog->tr("Parameters"));
    dialog->setMinimumWidth(180);
    QFormLayout* layout = new QFormLayout();
    dialog->setLayout(layout);

    ImageListBox_t<double>* magtdImgBox = new ImageListBox_t<double>(dialog, NULL, imgList);
    ImageListBox_t<double>* phaseImgBox = new ImageListBox_t<double>(dialog, NULL, imgList);
    layout->insertRow(0, "Magnitude : ", magtdImgBox);
    layout->insertRow(1, "Phase : ", phaseImgBox);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal, dialog);
    layout->insertRow(2, buttonBox);
    QObject::connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), dialog, SLOT(reject()));

    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());

    if(code!=QDialog::Accepted) {
        return;
    }

    const Image_t<double>* magnitudeImg = magtdImgBox->currentImage();
    const Image_t<double>* phaseImg = phaseImgBox->currentImage();
    unsigned int width = min(nearestUpPower2(magnitudeImg->getWidth()), nearestUpPower2(phaseImg->getWidth()));
    unsigned int height = min(nearestUpPower2(magnitudeImg->getHeight()), nearestUpPower2(phaseImg->getHeight()));
    unsigned int channels = min(magnitudeImg->getNbChannels(), phaseImg->getNbChannels());

    Image* resImg = new Image(width, height, channels);
//    Image_t<double>* realImg = new Image_t<double>(width, height, image->getNbChannels());
//    Image_t<double>* imagImg = new Image_t<double>(width, height, image->getNbChannels());

    complex<double>** data = new complex<double>*[width];
    for(unsigned int i = 0; i < width; ++i) data[i] = new complex<double>[height];

    const unsigned int cw = width/2;
    const unsigned int ch = height/2;
    for(unsigned int c = 0; c < channels; ++c) {
        for(unsigned int j = 0; j < height; ++j) {
            for(unsigned int i = 0; i < width; ++i) {
                const unsigned int ci = i >= cw ? i - cw : i + cw;
                const unsigned int cj = j >= ch ? j - ch : j + ch;
                const double magtd = magnitudeImg->getPixel(ci, cj, c);
                const double phase = phaseImg->getPixel(ci, cj, c);
                const double real = magtd * cos(phase);
                const double imag = magtd * sin(phase);
                data[i][j] = complex<double>(real,imag);
            }
        }

        FFT2D(data, width, height, -1);
        for(unsigned int j = 0; j < height; ++j) {
            for(unsigned int i = 0; i < width; ++i) {
//                const double real = data[i][j].real();
//                const double imag = data[i][j].imag();
                Image::depth_t value = floor(data[i][j].real()+0.5);
                value = min(255, max(0, value));
                resImg->setPixel(i, j, c, value);
            }
        }
    }

    this->outImage(resImg, "DFT-reconstructed image");
}
