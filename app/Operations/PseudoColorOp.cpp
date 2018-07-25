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

#include "PseudoColorOp.h"
#include "PseudoColorDialog.h"
#include "../Tools.h"
#include <Converter.h>
#include <QColor>

using namespace std;
using namespace imagein;

PseudoColorOp::PseudoColorOp() : Operation(qApp->translate("Operations", "Pseudo color").toStdString())
{
  _test=false, _testHue=255;
}

bool PseudoColorOp::needCurrentImg() const {
    return true;
}

static int getLinearHue(int value, int nhue){
    return floor(value * nhue / 255);
}

void PseudoColorOp::operator()(const imagein::Image* image, const std::map<const imagein::Image*, std::string>&) {

    PseudoColorDialog* dialog = new PseudoColorDialog(QApplication::activeWindow());

    if(_test){
        dialog->setHue(_testHue);
    }
    else{
        QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());
        if(code!=QDialog::Accepted) return;
    }

    GrayscaleImage* tmpImg = Converter<GrayscaleImage>::convert(*image);
    Image* resImg = new Image(tmpImg->getWidth(), tmpImg->getHeight(), 3);

    int nhue = dialog->getHue(); // changer cette valeur en fonction de la fenêtre de dialog

    int (*getHue)(int a, int b);

    getHue = getLinearHue;

    for(unsigned int j = 0; j < tmpImg->getHeight(); ++j) {
        for(unsigned int i = 0; i < tmpImg->getWidth(); ++i) {
            Image::depth_t value = tmpImg->getPixel(i, j);

            const int ngrad = ceil(255. / (double)nhue);
            const int hue =  getHue(value, nhue); /* € [0, nhue[ */
            const int grad = value - ceil((double)hue * 255 / (double)nhue); /* € [0, ngrad[ */
            QColor color = QColor::fromHsl(300 - hue * 300 / nhue, 255, (grad + 1) * 255 / (ngrad + 1));
            resImg->setPixel(i, j, 0, color.red());
            resImg->setPixel(i, j, 1, color.green());
            resImg->setPixel(i, j, 2, color.blue());
        }
    }
    delete tmpImg;
    outImage(resImg, qApp->translate("PseudoColorOp", "Pseudo color").toStdString());

}

void PseudoColorOp::setTest(bool a){
  _test =a;
}

void PseudoColorOp::setHue(int a){
  _testHue=a;
}
