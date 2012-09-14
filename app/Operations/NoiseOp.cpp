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
#include <Widgets/ImageWidgets/DoubleImageWindow.h>
#include <Widgets/ImageWidgets/StandardImageWindow.h>
#include "NoiseOp.h"
#include <RandomLib/Random.hpp>
#include <RandomLib/NormalDistribution.hpp>

#include "../Tools.h"

using namespace std;
using namespace imagein;
using namespace genericinterface;

NoiseOp::NoiseOp() : GenericOperation(qApp->translate("Operations", "Add noise").toStdString())
{
}

void NoiseOp::operator()(const ImageWindow* currentWnd, vector<ImageWindow*>&) {

    QDialog* dialog = new QDialog();
    dialog->setWindowTitle(qApp->translate("Operations", "Parameters"));
    dialog->setMinimumWidth(180);
    QFormLayout* layout = new QFormLayout(dialog);

    QGroupBox* radioGroup = new QGroupBox("Type of noise", dialog);
    QRadioButton* impulseButton = new QRadioButton(qApp->translate("NoiseOp", "Impulse noise"));
    QRadioButton* gaussianButton = new QRadioButton(qApp->translate("NoiseOp", "Gaussian noise"));
    QHBoxLayout* radioLayout = new QHBoxLayout(radioGroup);
    radioLayout->addWidget(impulseButton);
    radioLayout->addWidget(gaussianButton);
    gaussianButton->setChecked(true);
    layout->insertRow(0, radioGroup);

    QDoubleSpinBox* meanBox = new QDoubleSpinBox(dialog);
    QDoubleSpinBox* devBox = new QDoubleSpinBox(dialog);
    meanBox->setRange(-255.0, 255.0);
    meanBox->setValue(0.0);
    devBox->setValue(1.0);
    QLabel* meanLabel = new QLabel(qApp->translate("NoiseOp", "Mean : "));
    QLabel* devLabel = new QLabel(qApp->translate("NoiseOp", "Standard deviation : "));
    layout->insertRow(1, meanLabel, meanBox);
    layout->insertRow(2, devLabel, devBox);

    QDoubleSpinBox* percentBox = new QDoubleSpinBox(dialog);
    percentBox->setRange(0.0, 100.0);
    percentBox->setValue(10.0);
    percentBox->setSuffix("%");
    QLabel* percentLabel = new QLabel(qApp->translate("NoiseOp", "Percent of image : "));
    layout->insertRow(3, percentLabel, percentBox);
    percentBox->hide();
    percentLabel->hide();

    layout->setSizeConstraint(QLayout::SetFixedSize);

    QObject::connect(impulseButton, SIGNAL(toggled(bool)), percentLabel, SLOT(setVisible(bool)));
    QObject::connect(impulseButton, SIGNAL(toggled(bool)), percentBox, SLOT(setVisible(bool)));
    QObject::connect(gaussianButton, SIGNAL(toggled(bool)), meanLabel, SLOT(setVisible(bool)));
    QObject::connect(gaussianButton, SIGNAL(toggled(bool)), meanBox, SLOT(setVisible(bool)));
    QObject::connect(gaussianButton, SIGNAL(toggled(bool)), devLabel, SLOT(setVisible(bool)));
    QObject::connect(gaussianButton, SIGNAL(toggled(bool)), devBox, SLOT(setVisible(bool)));

    QPushButton *okButton = new QPushButton(qApp->translate("Operations", "Validate"), dialog);
    okButton->setDefault(true);
    layout->addWidget(okButton);
    QObject::connect(okButton, SIGNAL(clicked()), dialog, SLOT(accept()));

    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());

    if(code!=QDialog::Accepted) {
        return;
    }

    RandomLib::Random random;

    if(impulseButton->isChecked()) {
        double ratio = percentBox->value() / 100.0;
        if(currentWnd->isStandard()) {
            const StandardImageWindow* wnd = dynamic_cast<const StandardImageWindow*>(currentWnd);
            Image* image = new Image(*wnd->getImage());
            Image::depth_t min = 0;
            Image::depth_t max = 255;
            unsigned int n = ratio * image->getWidth()*image->getHeight()*image->getNbChannels();
            for(unsigned int i = 0; i < n; ++i) {
                unsigned int c = random.IntegerC<unsigned int>(0, image->getNbChannels()-1);
                unsigned int x = random.IntegerC<unsigned int>(0, image->getWidth()-1);
                unsigned int y = random.IntegerC<unsigned int>(0, image->getHeight()-1);
                Image::depth_t value = random.Boolean() ? min : max;
                image->setPixel(x, y, c, value);
            }
            this->outImage(image, qApp->translate("NoiseOp", "impulse noise").toStdString());

        }
        else if(currentWnd->isDouble()) {
            const DoubleImageWindow* wnd = dynamic_cast<const DoubleImageWindow*>(currentWnd);
            Image_t<double>* image = new Image_t<double>(*wnd->getImage());
            double min = image->min();
            double max = image->max();
            unsigned int n = ratio * image->getWidth()*image->getHeight()*image->getNbChannels();
            for(unsigned int i = 0; i < n; ++i) {
                unsigned int c = random.IntegerC<unsigned int>(0, image->getNbChannels()-1);
                unsigned int x = random.IntegerC<unsigned int>(0, image->getWidth()-1);
                unsigned int y = random.IntegerC<unsigned int>(0, image->getHeight()-1);
                double value = random.Boolean() ? min : max;
                image->setPixel(x, y, c, value);
            }
            this->outDoubleImage(image, qApp->translate("NoiseOp", "impulse noise").toStdString(), wnd->isNormalized(), wnd->isLogScaled());

        }
    }
    else if(gaussianButton->isChecked()) {
        double mean = meanBox->value();
        double deviation = devBox->value();
        RandomLib::NormalDistribution<double> normdist;

        if(currentWnd->isStandard()) {
            const StandardImageWindow* wnd = dynamic_cast<const StandardImageWindow*>(currentWnd);
            Image* image = new Image(*wnd->getImage());
            for(Image::iterator it = image->begin(); it < image->end(); ++it) {
                double value = *it;
                value += normdist.operator()(random, mean, deviation);
                value = min(255.0, max( 0.0, value ) );
                *it = static_cast<Image::depth_t>(value);
            }
            this->outImage(image, qApp->translate("NoiseOp", "gaussian noise").toStdString());

        }
        else if(currentWnd->isDouble()) {
            const DoubleImageWindow* wnd = dynamic_cast<const DoubleImageWindow*>(currentWnd);
            Image_t<double>* image = new Image_t<double>(*wnd->getImage());
            for(Image_t<double>::iterator it = image->begin(); it < image->end(); ++it) {
                *it += normdist.operator()(random, mean, deviation);
            }
            this->outDoubleImage(image, qApp->translate("NoiseOp", "gaussian noise").toStdString(), wnd->isNormalized(), wnd->isLogScaled());

        }
    }

}

bool NoiseOp::isValidImgWnd(const ImageWindow* imgWnd) const {
    return (imgWnd && (imgWnd->isStandard() || imgWnd->isDouble()));
}

bool NoiseOp::needCurrentImg() const {
    return true;
}
