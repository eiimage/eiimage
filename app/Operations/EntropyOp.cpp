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

#include "EntropyOp.h"

#include "../Tools.h"
#include <Image.h>

#include "GenericInterface.h"
#include <Array.h>
#include <Converter.h>

#include <QMap>
using namespace std;
using namespace imagein;

EntropyOp::EntropyOp() : Operation(qApp->translate("Operations", "Calcul d'entropie (image standard)").toStdString())
{
}

void EntropyOp::operator()(const Image* image, const std::map<const imagein::Image*, std::string>&) {

    double entropy = 0.;

    for(unsigned int c = 0; c < image->getNbChannels(); ++c) {
        Histogram histo = image->getHistogram(c);
        for(int i = 0; i < 256; ++i) {
            if(histo[i] > 0) {
                double p = (double)histo[i] / image->getWidth() /image->getHeight();
                entropy +=  p * log(p);
            }
        }
    }

    entropy = - entropy / log(2);
    outText(qApp->translate("Operations", "Entropy of the image = %1").arg(entropy).toStdString());
}

bool EntropyOp::needCurrentImg() const {
    return true;
}

EntropyOp::~EntropyOp() {
}
