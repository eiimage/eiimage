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
#include <QMessageBox>

#include "SplitColorOp.h"
#include "../Tools.h"

using namespace std;
using namespace imagein;

bool SplitColorOp::needCurrentImg() const {
    return true;
}

SplitColorOp::SplitColorOp() : Operation(qApp->translate("Operations", "Split color planes").toStdString())
{
}

void SplitColorOp:: operator()(const imagein::Image* image, const std::map<const imagein::Image*, std::string>&) {
    if(image->getNbChannels()==1){
        QMessageBox::warning(nullptr, qApp->translate("Operations", "The operation cannot be applied on this image"),
                             qApp->translate("Operations", "The image is not in color."));
        return;
    }

    for(unsigned int c = 0; c < image->getNbChannels(); ++c) {
        auto* resImg = new GrayscaleImage(image->getWidth(), image->getHeight());
        for(unsigned int j = 0; j < resImg->getHeight(); ++j) {
            for(unsigned int i = 0; i < resImg->getWidth(); ++i) {
                resImg->setPixel(i, j, image->getPixel(i, j, c));
            }
        }
        QString name("");
        name += Tools::colorName(c, image->getNbChannels());
        this->outImage(resImg, name.toStdString());
    }

}


