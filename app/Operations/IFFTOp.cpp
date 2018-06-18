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
#include <QLabel>
#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <Widgets/ImageListBox.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QRadioButton>

#include "IFFTOp.h"
#include "../Tools.h"
#include "../Algorithms/FFT.h"
#include <cmath>
#include <QGroupBox>
#include <QCheckBox>

using namespace std;
using namespace imagein;

IFFTOp::IFFTOp() : DoubleOperation(qApp->translate("Operations", "Inverse Fourier transform").toStdString())
{
}

bool IFFTOp::needCurrentImg() const {
    return false;
}

void IFFTOp::operator()(const imagein::Image_t<double>*, const map<const imagein::Image_t<double>*, string>& imgList) {

    QDialog* dialog = new QDialog();
    dialog->setWindowTitle(qApp->translate("Operations", "Parameters"));
    dialog->setMinimumWidth(180);
    QVBoxLayout* layout = new QVBoxLayout(dialog);

    QGroupBox* groupBox = new QGroupBox(dialog);
    QHBoxLayout* groupLayout = new QHBoxLayout(groupBox);
    QRadioButton* magButton = new QRadioButton(qApp->translate("IFFTOp","Magnitude/Phase"));
    QRadioButton* realButton = new QRadioButton(qApp->translate("IFFTOp", "Real/Imaginary"));
    groupLayout->addWidget(magButton);
    groupLayout->addWidget(realButton);
    layout->addWidget(groupBox);
    magButton->setChecked(true);

    QWidget* magWidget = new QWidget();
    QFormLayout* magLayout = new QFormLayout(magWidget);
    ImageListBox_t<double>* magtdImgBox = new ImageListBox_t<double>(dialog, NULL, imgList);
    ImageListBox_t<double>* phaseImgBox = new ImageListBox_t<double>(dialog, NULL, imgList);
    magLayout->insertRow(0, qApp->translate("IFFTOp", "Magnitude : "), magtdImgBox);
    magLayout->insertRow(1, qApp->translate("IFFTOp", "Phase : "), phaseImgBox);
    QWidget* realWidget = new QWidget();
    QFormLayout* realLayout = new QFormLayout(realWidget);
    ImageListBox_t<double>* realImgBox = new ImageListBox_t<double>(dialog, NULL, imgList);
    ImageListBox_t<double>* imagImgBox = new ImageListBox_t<double>(dialog, NULL, imgList);
    realLayout->insertRow(0, qApp->translate("IFFTOp", "Real part : "), realImgBox);
    realLayout->insertRow(1, qApp->translate("IFFTOp", "Imaginary part : "), imagImgBox);

    layout->addWidget(magWidget);
    layout->addWidget(realWidget);
    realWidget->setVisible(false);

    QCheckBox* centerBox = new QCheckBox(qApp->translate("IFFTOp", "Source is centered"));
    centerBox->setChecked(true);
    layout->addWidget(centerBox);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal, dialog);
    layout->addWidget(buttonBox);
    QObject::connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), dialog, SLOT(reject()));

    QObject::connect(realButton, SIGNAL(toggled(bool)), realWidget, SLOT(setVisible(bool)));
    QObject::connect(magButton, SIGNAL(toggled(bool)), magWidget, SLOT(setVisible(bool)));

    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());

    if(code!=QDialog::Accepted) {
        return;
    }

    Image* resImg;
    if(magButton->isChecked()) {

        const Image_t<double>* magnitudeImg = magtdImgBox->currentImage();
        const Image_t<double>* phaseImg = phaseImgBox->currentImage();
        if(magnitudeImg == NULL || phaseImg == NULL) return;
        unsigned int width = min(nearestUpPower2(magnitudeImg->getWidth()), nearestUpPower2(phaseImg->getWidth()));
        unsigned int height = min(nearestUpPower2(magnitudeImg->getHeight()), nearestUpPower2(phaseImg->getHeight()));
        unsigned int channels = min(magnitudeImg->getNbChannels(), phaseImg->getNbChannels());

        resImg = new Image(width, height, channels);

        complex<double>** data = new complex<double>*[width];
        for(unsigned int i = 0; i < width; ++i) data[i] = new complex<double>[height];

        const unsigned int cw = width/2;
        const unsigned int ch = height/2;
        for(unsigned int c = 0; c < channels; ++c) {
            if(centerBox->isChecked()) {
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
            }
            else {
                for(unsigned int j = 0; j < height; ++j) {
                    for(unsigned int i = 0; i < width; ++i) {
                        const double magtd = magnitudeImg->getPixel(i, j, c);
                        const double phase = phaseImg->getPixel(i, j, c);
                        const double real = magtd * cos(phase);
                        const double imag = magtd * sin(phase);
                        data[i][j] = complex<double>(real,imag);
                    }
                }
            }

            FFT2D(data, width, height, -1);
            for(unsigned int j = 0; j < height; ++j) {
                for(unsigned int i = 0; i < width; ++i) {
                    double value = floor(data[i][j].real()+0.5);
                    value = min(255.0, max(0.0, value));
                    resImg->setPixel(i, j, c, value);
                }
            }
        }
    }
    else {

        const Image_t<double>* realImg = realImgBox->currentImage();
        const Image_t<double>* imagImg = imagImgBox->currentImage();
        if(realImg == NULL || imagImg == NULL) return;
        unsigned int width = min(nearestUpPower2(realImg->getWidth()), nearestUpPower2(imagImg->getWidth()));
        unsigned int height = min(nearestUpPower2(realImg->getHeight()), nearestUpPower2(imagImg->getHeight()));
        unsigned int channels = min(realImg->getNbChannels(), imagImg->getNbChannels());

        resImg = new Image(width, height, channels);

        complex<double>** data = new complex<double>*[width];
        for(unsigned int i = 0; i < width; ++i) data[i] = new complex<double>[height];

        const unsigned int cw = width/2;
        const unsigned int ch = height/2;
        for(unsigned int c = 0; c < channels; ++c) {
            if(centerBox->isChecked()) {
                for(unsigned int j = 0; j < height; ++j) {
                    for(unsigned int i = 0; i < width; ++i) {
                        const unsigned int ci = i >= cw ? i - cw : i + cw;
                        const unsigned int cj = j >= ch ? j - ch : j + ch;
                        const double real = realImg->getPixel(ci, cj, c);
                        const double imag = imagImg->getPixel(ci, cj, c);
                        data[i][j] = complex<double>(real,imag);
                    }
                }
            }
            else {
                for(unsigned int j = 0; j < height; ++j) {
                    for(unsigned int i = 0; i < width; ++i) {
                        const double real = realImg->getPixel(i, j, c);
                        const double imag = imagImg->getPixel(i, j, c);
                        data[i][j] = complex<double>(real,imag);
                    }
                }
            }

            FFT2D(data, width, height, -1);
            for(unsigned int j = 0; j < height; ++j) {
                for(unsigned int i = 0; i < width; ++i) {
                    double value = floor(data[i][j].real()+0.5);
                    value = min(255.0, max(0.0, value));
                    resImg->setPixel(i, j, c, value);
                }
            }
        }
    }

    this->outImage(resImg, qApp->translate("IFFTOp", "DFT-reconstructed image").toStdString());
}
