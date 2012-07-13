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

#include <Image.h>

#include "ImgWidget.h"
#include "FlipOp.h"

using namespace std;
using namespace imagein;

FlipOp::FlipOp(Direction dir)
    : Operation(QString(tr("Flip %1")).arg(dir == Horizontal ? tr("horizontal") : tr("vertical")).toStdString()), _dir(dir)
{
}

bool FlipOp::needCurrentImg() const {
    return true;
}

void FlipOp::operator()(const imagein::Image* image, const std::map<const imagein::Image*, std::string>&) {

    Image* resImg = new Image(image->getWidth(), image->getHeight(), image->getNbChannels());

    if(_dir == Horizontal) {
       for(unsigned int c = 0; c < resImg->getNbChannels(); ++c) {
           for(unsigned int j = 0; j < resImg->getHeight(); ++j) {
                for(unsigned int i = 0; i < resImg->getWidth(); ++i) {
                    Image::depth_t value = image->getPixel(image->getWidth() - i - 1, j, c);
                    resImg->setPixel(i, j, c, value);
                }
           }
       }
    }
    else if(_dir == Vertical) {
       for(unsigned int c = 0; c < resImg->getNbChannels(); ++c) {
           for(unsigned int j = 0; j < resImg->getHeight(); ++j) {
                for(unsigned int i = 0; i < resImg->getWidth(); ++i) {
                    Image::depth_t value = image->getPixel(i, image->getHeight() - j - 1, c);
                    resImg->setPixel(i, j, c, value);
                }
           }
       }
    }
    QString name = QString(tr(" -  flipped %1")).arg(_dir == Horizontal ? tr("horizontal") : tr("vertical"));

    this->outImage(resImg, name.toStdString());
}

