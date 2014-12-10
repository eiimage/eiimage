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

#include "InverseHoughOp.h"
#include <QApplication>
#include "Transforms.h"
#include "InverseHoughDialog.h"

using namespace std;
using namespace imagein;

InverseHoughOp::InverseHoughOp() : DoubleOperation(qApp->translate("Operations", "Houghman inverse transform").toStdString())
{
}

bool InverseHoughOp::needCurrentImg() const {
    return true;
}

void InverseHoughOp::operator()(const imagein::Image_t<double>* img, const std::map<const imagein::Image_t<double>*, std::string>&) {
    InverseHoughDialog* dialog = new InverseHoughDialog(QApplication::activeWindow());
    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());


    if(code!=QDialog::Accepted) return;
    Image* resImg2;
    Transforms::hough2_inverse(img, &resImg2, dialog->getSize(), dialog->getThreshodl());
    outImage(resImg2, qApp->translate("Hough", "Hough inverse transform").toStdString());
}
