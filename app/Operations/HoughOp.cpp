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

#include "HoughOp.h"
#include <QApplication>
#include "Transforms.h"
#include <Converter.h>
#include <GrayscaleImage.h>
#include "HoughDialog.h"

using namespace std;
using namespace imagein;

HoughOp::HoughOp() : Operation(qApp->translate("Operations", "Hough transform").toStdString())
{
}

bool HoughOp::needCurrentImg() const {
    return true;
}

void HoughOp::operator()(const imagein::Image* img, const std::map<const imagein::Image*, std::string>&) {
    HoughDialog* dialog = new HoughDialog(QApplication::activeWindow());
    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());

    Image_t<double>* resImg;

    if(code!=QDialog::Accepted) return;
    if(dialog->isMethod1()) {
        method1 = true;
        GrayscaleImage* image = Converter<GrayscaleImage>::convert(*img);
        resImg = Transforms::hough(image);
        delete image;
    }
    else {
        angleStep = dialog->getAngleStep();
        distanceStep = dialog->getDistanceStep();
        resImg = Transforms::hough2(img, dialog->getAngleStep(), dialog->getDistanceStep());
        method1=false;
    }
    outDoubleImage(resImg, qApp->translate("Hough", "Hough transform").toStdString(), true, true);
//    Image* resImg2;
//    Transforms::hough2_inverse(resImg, &resImg2, 512, 0.);
//    outImage(resImg2, qApp->translate("Hough", "Hough inverse transform").toStdString());
}

