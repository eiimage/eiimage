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
#include <GrayscaleImage.h>

#include "ImgWidget.h"

#include "SplitColorOp.h"
#include "../Tools.h"

using namespace std;
using namespace imagein;

bool SplitColorOp::needCurrentImg() const {
    return true;
}

SplitColorOp::SplitColorOp() : Operation(tr("Split color planes").toStdString())
{
}

std::vector<QWidget*>SplitColorOp:: operator()(const imagein::Image* image, const std::map<const imagein::Image*, std::string>&) {

    vector<QWidget*> result;

    for(unsigned int c = 0; c < image->getNbChannels(); ++c) {
        GrayscaleImage* resImg = new GrayscaleImage(image->getWidth(), image->getHeight());
        for(unsigned int j = 0; j < resImg->getHeight(); ++j) {
            for(unsigned int i = 0; i < resImg->getWidth(); ++i) {
                resImg->setPixel(i, j, image->getPixel(i, j, c));
            }
        }
        QString name(" - ");
        name += Tools::colorName(c, image->getNbChannels());
        result.push_back(new ImgWidget(resImg, name.toStdString()));
    }

    return result;

}


