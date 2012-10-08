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

#include "InversePyramidOp.h"
#include <QApplication>
#include <QMessageBox>
#include "../Algorithms/Pyramid.h"
#include "InversePyramidDialog.h"
#include <GrayscaleImage.h>
#include <Converter.h>

using namespace std;
using namespace imagein;

InversePyramidOp::InversePyramidOp() : Operation(qApp->translate("Operations", "Pyramidal reconstruction").toStdString())
{
}

bool InversePyramidOp::needCurrentImg() const {
    return true;
}

void InversePyramidOp::operator()(const imagein::Image* img, const std::map<const imagein::Image*, std::string>&) {
    if(2 * img->getWidth() != img->getHeight()) {
        QMessageBox::warning(NULL, qApp->translate("Operations", "The operation can't be applied on this image"),
                             qApp->translate("Operations", "The image width must be twice the image height."));
        return;
    }
    if(!Pyramid::isPowerOf2(img->getWidth())) {
        QMessageBox::warning(NULL, qApp->translate("Operations", "The operation can't be applied on this image"),
                             qApp->translate("Operations", "The image dimensions must be power of 2."));
        return;
    }

    InversePyramidDialog* dialog = new InversePyramidDialog(QApplication::activeWindow());
    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());

    if(code!=QDialog::Accepted) return;

    GrayscaleImage* image = Converter<GrayscaleImage>::convert(*img);
    Image* resImg = NULL;
    string s;
    Pyramid::filtre filter = dialog->getFilter();
    try {
        resImg = Pyramid::rebuild_interface(image, dialog->getNbStep(), dialog->getStep(), filter);
    }
    catch(const char*e) {
        QMessageBox::critical(NULL, "Error", QString(e));
        return;
    }
    outImage(resImg, "Pyramid");
    outText(s);

}
