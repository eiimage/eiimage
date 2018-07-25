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

#include "HuffmanOp.h"
#include  "Huffman.h"
#include "../Tools.h"
#include <GrayscaleImage.h>
#include <string>
#include <Converter.h>
using namespace std;
using namespace imagein;

HuffmanOp::HuffmanOp() : Operation(qApp->translate("Operations", "Huffman").toStdString())
{
}

bool HuffmanOp::needCurrentImg() const {
    return true;
}

void HuffmanOp::operator()(const imagein::Image* image, const std::map<const imagein::Image*, std::string>&) {
    Huffman huff;
    GrayscaleImage* grayImg = Converter<GrayscaleImage>::convert(*image);
    string res = huff.execute(grayImg);
    outText(res);
    outText("-------------------------------------------");
    delete grayImg;
}
