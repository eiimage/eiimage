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

#include <cmath>

#include "DoubleEntropyOp.h"

#include "../Tools.h"
#include <Image.h>

#include "GenericInterface.h"
#include <Array.h>
#include <Converter.h>

#include <QMap>
using namespace std;
using namespace imagein;

DoubleEntropyOp::DoubleEntropyOp() : DoubleOperation(qApp->translate("Operations", "Calcul d'entropie (image double)").toStdString())
{
}

void DoubleEntropyOp::operator()(const Image_t<double>* image, const std::map<const imagein::Image_t<double>*, std::string>&) {

    double entropy = image->getEntropy();
    outText(qApp->translate("Operations", "Entropy of the image (sum of channels entropy) = %1").arg(entropy).toStdString());

}

bool DoubleEntropyOp::needCurrentImg() const {
    return true;
}

DoubleEntropyOp::~DoubleEntropyOp() {
}
