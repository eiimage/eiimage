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

#include "../../lib/detiq-t/GenericInterface/Widgets/ImageWidgets/GraphicalHistogram.h"
#include "../../lib/detiq-t/GenericInterface/Widgets/ImageWidgets/GenericHistogramView.h"

using namespace std;
using namespace imagein;

EntropyOp::EntropyOp() : Operation(qApp->translate("Operations", "Calcul d'entropie").toStdString())
{
}

void EntropyOp::operator()(const Image* image, const std::map<const imagein::Image*, std::string>&) {
    /*
    std::vector<GraphicalHistogram*> _graphicalHistos;
    for(uint i = 0; i < nbChannels; ++i)
    {
        GraphicalHistogram* graphicalHisto;
        switch(i)
        {
        case 0:
            if(nbChannels == 1 || nbChannels == 2)
                graphicalHisto = new GraphicalHistogram(tr("Black"), Qt::black);
            else
                graphicalHisto = new GraphicalHistogram(tr("Red"), Qt::red);
            break;
        case 1:
            if(nbChannels == 1 || nbChannels == 2)
                graphicalHisto = new GraphicalHistogram(tr("Alpha"), Qt::white);
            else
                graphicalHisto = new GraphicalHistogram(tr("Green"), Qt::green);
            break;
        case 2:
            graphicalHisto = new GraphicalHistogram(tr("Blue"), Qt::blue);
            break;
        case 3:
            graphicalHisto = new GraphicalHistogram(tr("Alpha"), Qt::black);
            break;
        default:
            graphicalHisto = new GraphicalHistogram(tr("Channel"), Qt::black);
        }
        _graphicalHistos.push_back(graphicalHisto);
    }
    for(unsigned int channel = 0; channel < _graphicalHistos.size(); ++channel) {
        GraphicalHistogram* graphicalHisto = _graphicalHistos[channel];


            QMap<int, int> cumulativeValues;

            uint maxw = rect.w > 0 ? rect.x + rect.w : image->getWidth();
            uint maxh = rect.h > 0 ? rect.y + rect.h : image->getHeight();
            for(uint j = rect.y; j < maxh; j++) {
                for(uint i = rect.x; i < maxw; i++) {
                    double pixel = image->getPixel(i, j, channel);
                    cumulativeValues[qFloor(pixel)]++;
                }
            }

            QVector<QwtIntervalSample> samples;
            for(int i = qFloor(image->min()); i <= qFloor(image->max()); ++i) {
                QwtIntervalSample sample(cumulativeValues.value(i, 0), i, i + 1);
                samples << sample;
            }

            graphicalHisto->setData(new QwtIntervalSeriesData(samples));

    }
    */

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
    //return false;
}
