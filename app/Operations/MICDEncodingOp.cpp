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

#include "MICDEncodingOp.h"
#include "../Tools.h"
#include <QApplication>
#include "MICDDialog.h"
#include "MICD.h"
#include <QMessageBox>
#include <GrayscaleImage.h>
#include <Converter.h>

using namespace std;
using namespace imagein;

MICDEncodingOp::MICDEncodingOp() : Operation(qApp->translate("Operations", "MICD Encoding").toStdString())
{
}

bool MICDEncodingOp::needCurrentImg() const {
    return true;
}

void MICDEncodingOp::operator()(const imagein::Image* img, const std::map<const imagein::Image*, std::string>&) {

    MICDDialog* dialog = new MICDDialog(QApplication::activeWindow());
    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());


    if(code!=QDialog::Accepted) return;

    MICD micd;
    try {
        micd.setQuantification(dialog->getQuantification());
    }
    catch(const char* str) {
        QMessageBox::critical(NULL, qApp->translate("MICD", "Error while loading quantification file"), qApp->translate("MICD", "The specified quantification file could not be opened !"));
        return;
    }
    GrayscaleImage* image = Converter<GrayscaleImage>::convert(*img);
    Image *errorImage, *reconstructedImage;
    string s = micd.execute(image, dialog->getPrediction(), &errorImage, &reconstructedImage, dialog->getQ());
    outText(s);
    outImage(errorImage, qApp->translate("MICD", "Error image").toStdString());
    outImage(reconstructedImage, qApp->translate("MICD", "Reconstructed image").toStdString());
}

