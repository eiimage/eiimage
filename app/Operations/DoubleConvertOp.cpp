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


#include "DoubleConvertOp.h"
#include "../Tools.h"
#include <QApplication>
#include <QObject>
#include "Operation.h"
#include "Image.h"
#include <Converter.h>
#include <QLabel>
#include <QStringList>

using namespace imagein;

DoubleConvertOp::DoubleConvertOp() : Operation(qApp->translate("Operations", "Convert to Double").toStdString())
{
}

bool DoubleConvertOp::needCurrentImg() const{
    return true;
}

void DoubleConvertOp::operator()(const imagein::Image* from, const std::map<const imagein::Image*, std::string>&){

    ImageDouble* resImg = new ImageDouble(from->getWidth(), from->getHeight(), from->getNbChannels());

    for(unsigned int i = 0; i < from->getWidth(); i++){
        for(unsigned int j = 0; j < from->getHeight(); j++){
            for(unsigned int k = 0; k < from->getNbChannels(); k++){
                double newPixel = from->getPixel(i, j, k);
                resImg->setPixel(i, j, k, newPixel );
            }
        }
    }
    outDoubleImage(resImg, "Double", AUTO, AUTO, false, false, false);
}
