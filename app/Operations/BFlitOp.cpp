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

#include <QDialog>
#include <QFormLayout>
#include <QRadioButton>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include "BFlitOp.h"

#include "../Tools.h"

using namespace std;
using namespace imagein;

BFlitOp::BFlitOp() : Operation(qApp->translate("Operations", "BFilt").toStdString())
{
}

bool BFlitOp::needCurrentImg() const {
    return true;
}

Image* bflit(const Image* image, unsigned int nPointSide, unsigned int nIteration);

void BFlitOp::operator()(const imagein::Image* image, const std::map<const imagein::Image*, std::string>&) {
    QDialog* dialog = new QDialog();
    dialog->setWindowTitle(qApp->translate("Operations", "Parameters"));
    dialog->setMinimumWidth(180);
    QFormLayout* layout = new QFormLayout(dialog);

    QSpinBox* nPointBox = new QSpinBox(dialog);
    QSpinBox* nIterBox = new QSpinBox(dialog);
    const unsigned int nPointImg = image->getWidth()*image->getHeight();
    nPointBox->setRange(0, nPointImg/2-1);
    nPointBox->setValue(nPointImg/4);
    nIterBox->setValue(1);
    layout->insertRow(0, qApp->translate("BFlit", "Number of point on each side : "), nPointBox);
    layout->insertRow(1, qApp->translate("BFlit", "Number of iteration : "), nIterBox);

    QPushButton *okButton = new QPushButton(qApp->translate("Operations", "Validate"), dialog);
    okButton->setDefault(true);
    layout->addWidget(okButton);
    QObject::connect(okButton, SIGNAL(clicked()), dialog, SLOT(accept()));

    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());

    if(code!=QDialog::Accepted) {
        return;
    }

    Image* resImg = bflit(image, nPointBox->value(), nIterBox->value());
    this->outImage(resImg);
}

inline int sq(int x) { return x*x; }

Image* bflit(const Image* image, unsigned int nPointSide, unsigned int nIteration) {

    Image* resImg = new Image(*image);

    /* filtrage bimodal */

    /* determination de lmin et lmax sur l'histogramme */
    Histogram histo = image->getHistogram();
    int lMin = 0, lMax = 255;
    for(unsigned int n = 0; n < nPointSide; n += histo[++lMin]);
    for(unsigned int n = 0; n < nPointSide; n += histo[--lMax]);

    Image* bufImg = new Image(resImg->getWidth(), resImg->getHeight(), resImg->getNbChannels());
    for(unsigned int n = 0; n < nIteration; ++n)   /* boucle sur le nombre d'itrations */
    {
        for(Image::iterator it = resImg->begin(), jt = bufImg->begin(); it < resImg->end(); ++it, ++jt) {
            *jt = *it;
        }

        for(unsigned int c = 0; c < image->getNbChannels(); ++c)
        {
            for(unsigned int j = 1; j < image->getHeight()-1; ++j)
            {
                for(unsigned int i = 1; i < image->getWidth()-1; ++i) {
                    /* sauvegarde des 4 points voisins dans lumc[i] */
                    Image::depth_t north = bufImg->getPixel(i, j-1, c);
                    Image::depth_t south = bufImg->getPixel(i, j+1, c);
                    Image::depth_t east = bufImg->getPixel(i-1, j, c);
                    Image::depth_t west = bufImg->getPixel(i+1, j, c);
                    /* calcul des distances d1 et d2 */
                    const int d1 = sq(north-lMin) + sq(south-lMin) + sq(east-lMin) + sq(west-lMin);
                    const int d2 = sq(north-lMax) + sq(south-lMax) + sq(east-lMax) + sq(west-lMax);

                    if(d1 <= d2)     /* test des distances d1 et d2 */
                    {
                        const Image::depth_t value = min(north, min(south, min(east, west) ) );
                        resImg->setPixel(i, j, c, value);
                    }
                    else
                    {
                        const Image::depth_t value = max(north, max(south, max(east, west) ) );
                        resImg->setPixel(i, j, c, value);
                    }
                }
            }
        }

    } /* fin de la boucle sur le nombre d'itrations */
    delete bufImg;
    return resImg;
}
