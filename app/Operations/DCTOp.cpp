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

#include "DCTOp.h"
#include <QApplication>
#include "../Algorithms/DCT.h"
#include <GrayscaleImage.h>
#include <Converter.h>
#include "DCTDialog.h"

using namespace imagein;
using namespace std;

DCTOp::DCTOp() : Operation(qApp->translate("Operations", "Discrete cosinus transform").toStdString())
{
}

bool DCTOp::needCurrentImg() const {
    return true;
}

void DCTOp::operator()(const imagein::Image* image, const std::map<const imagein::Image*, std::string>&) {
//    GrayscaleImage* img = Converter<GrayscaleImage>::convert(*image);
    DCTDialog* dialog = new DCTDialog(QApplication::activeWindow());
    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());


    if(code!=QDialog::Accepted) return;

    Image_t<double> *resImg;
    Image *invImg;
    string s;
    if(dialog->isTruncMode()) {
        s = dct16x16(image, &resImg, &invImg, true, dialog->getTruncLimit());
    }
    else {
        s = dct16x16(image, &resImg, &invImg, false, 0, dialog->getNbBitInit(), dialog->getSlope());
    }

    outText(s);
    outDoubleImage(resImg, qApp->translate("DCT", "DCT").toStdString(), true, true, 128., true);
    outImage(invImg, qApp->translate("DCT", "inverse DCT").toStdString());
}

