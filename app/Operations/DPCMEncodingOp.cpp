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

#include "DPCMEncodingOp.h"
#include "../Tools.h"
#include <QApplication>
#include "DPCMDialog.h"
#include "DPCM.h"
#include <QMessageBox>
#include <GrayscaleImage.h>
#include <Converter.h>
#include <QDebug>

using namespace std;
using namespace imagein;

DPCMEncodingOp::DPCMEncodingOp() : Operation(qApp->translate("Operations", "DPCM Encoding").toStdString())
{
}

bool DPCMEncodingOp::needCurrentImg() const {
    return true;
}

void DPCMEncodingOp::operator()(const imagein::Image* img, const std::map<const imagein::Image*, std::string>&) {

    DPCMDialog* dialog = new DPCMDialog(QApplication::activeWindow());
    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());


    if(code != QDialog::Accepted) return;

    DPCM micd;

    try {
        micd.setQuantification(dialog->getQuantification());
    }
    catch(const char* str) {
        QMessageBox::critical(NULL, qApp->translate("DPCM", "Error while loading quantification file"),
                              qApp->translate("DPCM", "The specified quantification file could not be opened !"));
        return;
    }
    GrayscaleImage* image = Converter<GrayscaleImage>::convert(*img);
    Image *reconstructedImage;
    Image *predictionImage;
    ImageDouble *quant_errorImage;
    ImageDouble *errorImage;
    ImageDouble *coding_error_image;
    string s = micd.execute(image, dialog->getPrediction(), &quant_errorImage, &errorImage,&reconstructedImage, &predictionImage, &coding_error_image, dialog->getQ());
    outText(s);
    outText("-------------------------------------------");
    outDoubleImage(errorImage, qApp->translate("DPCM", "Prediction error image").toStdString(), true, true, 0.1, false);
    outDoubleImage(quant_errorImage, qApp->translate("DPCM", "Quantized prediction error image").toStdString(), true, true, 0.1, false);
    outImage(predictionImage, qApp->translate("DPCM", "Prediction image").toStdString());
    outImage(reconstructedImage, qApp->translate("DPCM", "Reconstructed image").toStdString());
    outDoubleImage(coding_error_image, qApp->translate("DPCM", "Coding error image").toStdString(), true, true, 0.1, false);
}
