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

#include <vector>
#include <QApplication>

#include <Operation.h>
#include <Image.h>
#include <ImgWidget.h>


#include "CenterOp.h"

using namespace std;
using namespace imagein;


CenterOp::CenterOp() : Operation(tr("Center").toStdString())
{

}

bool CenterOp::needCurrentImg() const {
    return true;
}

void CenterOp::operator()(const imagein::Image* image, const std::map<const imagein::Image*, std::string>&) {

    Image* resImg = new Image(image->getWidth()-image->getWidth()%2, image->getHeight()-image->getHeight()%2, image->getNbChannels());

    for(unsigned int c = 0;  c < resImg->getNbChannels(); ++c) {
        for(unsigned int j = 0; j < resImg->getHeight()/2; ++j)
        {
            for(unsigned int i = 0; i < resImg->getWidth()/2; ++i) {
                Image::depth_t value = image->getPixel(i, j, c);
                resImg->setPixel(resImg->getWidth()/2 + i, resImg->getHeight()/2 + j, c, value);
            }
            for(unsigned int i = 0; i < resImg->getWidth()/2; ++i) {
                Image::depth_t value = image->getPixel(image->getWidth()/2 + i, j, c);
                resImg->setPixel(i, resImg->getHeight()/2 + j, c, value);
            }

        }

        for(unsigned int j = 0; j < resImg->getHeight()/2; ++j)
        {
            for(unsigned int i = 0; i < resImg->getWidth()/2; ++i) {
                Image::depth_t value = image->getPixel(i, image->getHeight()/2 + j, c);
                resImg->setPixel(resImg->getWidth()/2 + i, j, c, value);
            }
            for(unsigned int i = 0; i < resImg->getWidth()/2; ++i) {
                Image::depth_t value = image->getPixel(image->getWidth()/2 + i, image->getHeight()/2 + j, c);
                resImg->setPixel(i, j, c, value);
            }
        }
    }

    this->outImage(resImg, " - centered");
}

