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
#include <GrayscaleImage.h>

#include <QColor>

#include "SplitHsvOp.h"
#include "../Tools.h"
#include <QMessageBox>

using namespace std;
using namespace imagein;

bool SplitHsvOp::needCurrentImg() const {
    return true;
}

SplitHsvOp::SplitHsvOp() : Operation(qApp->translate("Operations", "Split HSV planes").toStdString())
{
}

void SplitHsvOp:: operator()(const imagein::Image* image, const std::map<const imagein::Image*, std::string>&) {

    if(image->getNbChannels()==1){
        QMessageBox::warning(NULL, qApp->translate("Operations", "The operation cannot be applied on this image"),
                             qApp->translate("Operations", "The image is not in color."));
        return;
    }

        int width = image->getWidth();
        int height = image->getHeight();

        Image_t<double>* resImgH = new Image_t<double>(width, height, 1);
        Image_t<double>* resImgS = new Image_t<double>(width, height, 1);
        Image_t<double>* resImgV = new Image_t<double>(width, height, 1);

        for(unsigned int i = 0; i < width; i++) {
            for(unsigned int j = 0; j < height; j++) {
                QColor color = QColor(image->getPixel(i, j, 0), image->getPixel(i, j, 1), image->getPixel(i, j, 2));
                resImgH->setPixelAt(i,j,color.hue());
                resImgS->setPixelAt(i,j,color.saturation());
                resImgV->setPixelAt(i,j,color.value());

            }
        }
        this->outDoubleImage(resImgH, "Hue",false,false);
        this->outDoubleImage(resImgS, "Saturation",false,false);
        this->outDoubleImage(resImgV, "Value",false,false);


}


