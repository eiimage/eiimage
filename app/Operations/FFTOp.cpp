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
#include "FFTOp.h"
#include "../Tools.h"
#include "../Algorithms/FFT.h"
#include "FFTDialog.h"
#include <cmath>

using namespace std;
using namespace imagein;

FFTOp::FFTOp() : Operation(qApp->translate("Operations", "Fourier transform").toStdString())
{
}

bool FFTOp::needCurrentImg() const {
    return true;
}

void FFTOp::operator()(const imagein::Image* image, const map<const imagein::Image*, string>&) {
    auto* dialog = new FFTDialog(QApplication::activeWindow());
    auto code = static_cast<QDialog::DialogCode>(dialog->exec());


    if(code!=QDialog::Accepted) return;

    int width = nearestUpPower2((int)image->getWidth());
    int height = nearestUpPower2((int)image->getHeight());


    auto** data = new complex<double>*[width];
    for(unsigned int i = 0; i < width; ++i) data[i] = new complex<double>[height];

    if(dialog->isMagPhase()) {
        auto* magnitudeImg = new Image_t<double>(width, height, image->getNbChannels());
        auto* phaseImg = new Image_t<double>(width, height, image->getNbChannels());
        for(unsigned int c = 0; c < image->getNbChannels(); ++c) {
            for(unsigned int j = 0; j < image->getHeight(); ++j) {
                for(unsigned int i = 0; i < image->getWidth(); ++i) {
                    data[i][j] = static_cast<double>(image->getPixel(i, j, c));
                }
            }
            for(unsigned int j = image->getHeight(); j < height; ++j) {
                for(unsigned int i = image->getWidth(); i < width; ++i) {
                    data[i][j] = 0;
                }
            }

            FFT2D(data, width, height, 1);

            if(dialog->isCentered()) {
                for(unsigned int j = 0; j < height; ++j) {
                    for(unsigned int i = 0; i < width; ++i) {
                        const double real = data[i][j].real();
                        const double imag = data[i][j].imag();
                        const double magnitude = sqrt( real*real + imag*imag );
                        const double phase = atan2(imag, real);
                        const unsigned int cw = width/2;
                        const unsigned int ch = height/2;
                        const unsigned int ci = i >= cw ? i - cw : i + cw;
                        const unsigned int cj = j >= ch ? j - ch : j + ch;
                        magnitudeImg->setPixel(ci, cj, c, magnitude);
                        phaseImg->setPixel(ci, cj, c, phase);
                    }
                }
            }
            else {
                for(unsigned int j = 0; j < height; ++j) {
                    for(unsigned int i = 0; i < width; ++i) {
                        const double real = data[i][j].real();
                        const double imag = data[i][j].imag();
                        const double magnitude = sqrt( real*real + imag*imag );
                        const double phase = atan2(imag, real);
                        magnitudeImg->setPixel(i, j, c, magnitude);
                        phaseImg->setPixel(i, j, c, phase);
                    }
                }
            }
        }

        /*(imagein::ImageDouble* img, std::string title = "", bool norm=false, bool log=false, double logScale = 1., bool abs = false)*/
        this->outDoubleImage(phaseImg, qApp->translate("FFTOp", "DFT (Phase)").toStdString(), AUTO, AUTO, false);
        this->outDoubleImage(magnitudeImg, qApp->translate("FFTOp", "DFT (Magnitude)").toStdString(), AUTO, AUTO, true);
    }
    else {
        auto* realImg = new Image_t<double>(width, height, image->getNbChannels());
        auto* imagImg = new Image_t<double>(width, height, image->getNbChannels());
        for(unsigned int c = 0; c < image->getNbChannels(); ++c) {
            for(unsigned int j = 0; j < image->getHeight(); ++j) {
                for(unsigned int i = 0; i < image->getWidth(); ++i) {
                    data[i][j] = static_cast<double>(image->getPixel(i, j, c));
                }
            }
            for(unsigned int j = image->getHeight(); j < height; ++j) {
                for(unsigned int i = image->getWidth(); i < width; ++i) {
                    data[i][j] = 0;
                }
            }

            FFT2D(data, width, height, 1);

            if(dialog->isCentered()) {
                for(unsigned int j = 0; j < height; ++j) {
                    for(unsigned int i = 0; i < width; ++i) {
                        const double real = data[i][j].real();
                        const double imag = data[i][j].imag();
                        const unsigned int cw = width/2;
                        const unsigned int ch = height/2;
                        const unsigned int ci = i >= cw ? i - cw : i + cw;
                        const unsigned int cj = j >= ch ? j - ch : j + ch;
                        realImg->setPixel(ci, cj, c, real);
                        imagImg->setPixel(ci, cj, c, imag);
                    }
                }
            }
            else {
                for(unsigned int j = 0; j < height; ++j) {
                    for(unsigned int i = 0; i < width; ++i) {
                        const double real = data[i][j].real();
                        const double imag = data[i][j].imag();
                        realImg->setPixel(i, j, c, real);
                        imagImg->setPixel(i, j, c, imag);
                    }
                }
            }
        }
        this->outDoubleImage(realImg, qApp->translate("FFTOp", "(Real part)").toStdString(), AUTO, AUTO, true);
        this->outDoubleImage(imagImg, qApp->translate("FFTOp", "(Imaginary part)").toStdString(), AUTO, AUTO, true);
    }
}
