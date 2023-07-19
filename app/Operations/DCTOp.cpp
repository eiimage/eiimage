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

#include "DCTOp.h"
#include <QApplication>
#include <QString>
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
        s.pop_back();
        string str = QString(qApp->translate("Operations", " bpp avec 8 bits par coefficient non nul")).toStdString();
        s += str;
    }
    else {
        s = dct16x16(image, &resImg, &invImg, false, 0, dialog->getNbBitInit(), dialog->getSlope());
        s.pop_back();
        s += " bpp";
    }

    outText(s);
    outText("-------------------------------------------");
    outDoubleImage(resImg, qApp->translate("DCT", "DCT").toStdString(), ENABLE,DISABLE, true, 128., true);
    outImage(invImg, qApp->translate("DCT", "inverse DCT").toStdString());
}

