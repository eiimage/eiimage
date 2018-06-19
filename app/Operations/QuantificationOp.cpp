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

string QuantificationOp::quantificationOpLog(unsigned int c, Quantification * quant){
    char buffer[30];
    string output_msg;
    int val;    
    sprintf(buffer, "\nCanal %d : \n", c);
    output_msg += buffer;

    output_msg += "             Valeurs :  ";

    for(int i = 0; i < quant->nbValues(); ++i) {
        
        val =(int)quant->value(i);
        if(i != 0) output_msg += " |  ";
                
        sprintf(buffer, "%d ", val);
        output_msg += buffer;
        if(val < 10) output_msg += " ";
        if(val < 100 ) output_msg += " ";


    }
    output_msg += "\n";
    output_msg += "             Seuils    :       ";
            
    for(int i = 0; i < quant->nbThresholds(); ++i) {
        val =(int)quant->threshold(i);

        if(i != 0) output_msg += " |  ";
                
        sprintf(buffer, "%d ", val);
        output_msg += buffer;
                
        if(val < 10) output_msg += " ";
        if(val < 100 ) output_msg += " ";
    }

    output_msg += "\n";

    return output_msg;
}


void QuantificationOp::operator()(const imagein::Image* image, const std::map<const imagein::Image*, std::string>& imgList) {
    string quantType;
    string output_msg ="" ;

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
            
            
            Quantification quantification = dialog->getQuantif(image, c, quantType);
            
            //Generate the text to print in the information window
            output_msg += quantificationOpLog(c, &quantification);


            for(unsigned int j = 0; j < image->getHeight(); ++j) {
                for(unsigned int i = 0; i < image->getWidth(); ++i) {
                    
                    const Image::depth_t value = image->getPixelAt(i, j, c);
                    resImg->setPixelAt(i, j, c, quantification.valueOf(value));
                }
            }
        }
        
        outText(quantType);
        outText(output_msg);
        outImage(resImg, qApp->translate("QuantificationOp", "quantified").toStdString());
    }

}

