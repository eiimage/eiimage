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

#include "PyramidOp.h"

#include <QApplication>
#include "../Algorithms/Pyramid.h"
#include "PyramidDialog.h"
#include <Converter.h>
#include <GrayscaleImage.h>
#include <QMessageBox>

using namespace std;
using namespace imagein;

PyramidOp::PyramidOp() : Operation(qApp->translate("Operations", "Pyramidal decomposition").toStdString())
{
}

bool PyramidOp::needCurrentImg() const {
    return true;
}

void PyramidOp::operator()(const imagein::Image* img, const std::map<const imagein::Image*, std::string>&) {

    if(img->getWidth() != img->getHeight()) {
        QMessageBox::warning(NULL, qApp->translate("Operations", "The operation can't be applied on this image"),
                             qApp->translate("Operations", "The image width must equal the image height."));
        return;
    }
    if(!Pyramid::isPowerOf2(img->getWidth())) {
        QMessageBox::warning(NULL, qApp->translate("Operations", "The operation can't be applied on this image"),
                             qApp->translate("Operations", "The image dimensions must be power of 2."));
        return;
    }

    PyramidDialog* dialog = new PyramidDialog(QApplication::activeWindow());
    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());

    if(code!=QDialog::Accepted) return;

    GrayscaleImage* image = Converter<GrayscaleImage>::convert(*img);
    Image* resImg = NULL;
    string s;
    string e;
    Pyramid::Filtre filtre = dialog->getFilter(e);
    QString titleFilter;

    try {
        if(dialog->onlyOneStep()) {
            if(dialog->isGaussian()) {
                resImg = Pyramid::n_pyram_g(image, dialog->onlyStep(), filtre);
                titleFilter = QString(qApp->translate("Operations","Gaussian"));
            }
            else {
                resImg = Pyramid::n_pyram_l(image, dialog->onlyStep(), filtre);
                titleFilter = QString(qApp->translate("Operations","Laplacian"));
            }
        }
        else {
            if(dialog->isGaussian()) {
                resImg = Pyramid::pyram_g(image, dialog->getNbStep(), filtre, s);
                titleFilter = QString(qApp->translate("Operations","Gaussian"));
            }
            else {
                resImg = Pyramid::pyram_l(image, dialog->getNbStep(), filtre, s);
                titleFilter = QString(qApp->translate("Operations","Laplacian"));
            }
        }
    }
    catch(const char*e) {
        QMessageBox::critical(NULL, "Error", QString(e));
        return;
    }

    string winTitle = e;
    outImage(resImg, titleFilter.toStdString() + " - " + winTitle.erase(e.length()-3, e.length()));
    outText(e);
    outText(s);
}
