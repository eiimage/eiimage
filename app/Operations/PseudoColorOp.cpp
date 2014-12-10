/*
 * Copyright 2011-2012 INSA Rennes
 *
 * This file is part of INSAimage.
 *
 * INSAimage is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * INSAimage is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with INSAimage.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "PseudoColorOp.h"
#include "../Tools.h"
#include <Converter.h>
#include <QColor>

using namespace std;
using namespace imagein;

PseudoColorOp::PseudoColorOp() : Operation(qApp->translate("Operations", "Pseudo color").toStdString())
{
}

bool PseudoColorOp::needCurrentImg() const {
    return true;
}

void PseudoColorOp::operator()(const imagein::Image* image, const std::map<const imagein::Image*, std::string>&) {
   GrayscaleImage* tmpImg = Converter<GrayscaleImage>::convert(*image);
   Image* resImg = new Image(tmpImg->getWidth(), tmpImg->getHeight(), 3);
    for(unsigned int j = 0; j < tmpImg->getHeight(); ++j) {
        for(unsigned int i = 0; i < tmpImg->getWidth(); ++i) {
            Image::depth_t value = tmpImg->getPixel(i, j);
            const int nhue = 256;
            const int ngrad = ceil(256. / (double)nhue);
            const int hue = floor(value * nhue / 256); /* € [0, nhue[ */
            const int grad = value - ceil((double)hue * 256. / (double)nhue); /* € [0, ngrad[ */
            QColor color = QColor::fromHsl(300 - hue * 300 / nhue, 255, (grad + 1) * 255 / (ngrad + 1));
            resImg->setPixel(i, j, 0, color.red());
            resImg->setPixel(i, j, 1, color.green());
            resImg->setPixel(i, j, 2, color.blue());
        }
    }
    delete tmpImg;
    outImage(resImg, qApp->translate("PseudoColorOp", "Pseudo color").toStdString());

}

