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

#include "QuantificationOp.h"
#include "../Tools.h"
#include <Converter.h>
#include "QuantificationDialog.h"

#include <QApplication>

using namespace std;
using namespace imagein;
using namespace genericinterface;

QuantificationOp::QuantificationOp() : Operation(qApp->translate("Operations", "Quantification").toStdString())
{
}


bool QuantificationOp::needCurrentImg() const {
    return false;
}

void QuantificationOp::operator()(const imagein::Image* image, const std::map<const imagein::Image*, std::string>& imgList) {
    QuantificationDialog* dialog;
    if(image != NULL) {
        QString imgName = QString::fromStdString(imgList.find(image)->second);
        dialog = new QuantificationDialog(QApplication::activeWindow(), imgName);
    }
    else {
        dialog = new QuantificationDialog(QApplication::activeWindow());
    }

    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());


    if(code!=QDialog::Accepted) return;

    if(image != NULL) {
        Image* resImg = new Image(image->getWidth(), image->getHeight(), image->getNbChannels());
        for(unsigned int c = 0; c < image->getNbChannels(); ++c) {
            Quantification quantification = dialog->getQuantif(image, c);
            for(int i = 0; i < quantification.size; ++i) {
                cout << (int)quantification.values[i] << ".";
            }
            cout << endl;
            for(int i = 0; i < quantification.size-1; ++i) {
                cout << quantification.threshold[i] << ".";
            }
            cout << endl;
            Quantifier quantifier = Quantifier(quantification);
            for(unsigned int j = 0; j < image->getHeight(); ++j) {
                for(unsigned int i = 0; i < image->getWidth(); ++i) {
                    const Image::depth_t value = image->getPixelAt(i, j, c);
                    resImg->setPixelAt(i, j, c, quantifier.valueOf(value));
                }
            }
        }
        outImage(resImg, qApp->translate("QuantificationOp", "quantified").toStdString());
    }

}

