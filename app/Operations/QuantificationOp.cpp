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

string checkOptimumQuantizier(const imagein::Image* image, Quantification * quant, unsigned int c){
    
    float baricenter;
    float valueCenter; 
    float centroid = 0.0;
    float neighbor = 0.0;
    int som_lum = 0;
    int nb_points = 0;
    char buffer[100];
    
    Histogram hist = image->getHistogram(c);
    for (int j=1; j<quant->nbThresholds();j++){
        som_lum = 0;
        nb_points = 0;
        //Calcul des baricentres entre deux seuils  
        for(int i= quant->threshold(j-1); i <= quant->threshold(j); i++){
            som_lum += hist[i]*i;          
            nb_points += hist[i];
        }
        baricenter = som_lum/nb_points;

        centroid += abs( baricenter - quant->value(j) ) /( quant->threshold(j) - quant->threshold(j-1) ) * 100;
    }

    //cas spécial 
    if( quant->nbValues() == 2){
        som_lum = 0;
        nb_points = 0;
        for(int i= 0; i <= quant->threshold(0); i++){
                som_lum += hist[i]*i;          
                nb_points += hist[i];
        }
        baricenter = som_lum/nb_points;
        centroid = abs( baricenter - quant->value(0) ) /( quant->threshold(0) ) * 100;
        
        som_lum = 0;
        nb_points = 0;
        for(int i= quant->threshold(0); i <= 255 ; i++){
                som_lum += hist[i]*i;          
                nb_points += hist[i];
            }
        baricenter = som_lum/nb_points;
        centroid += abs( baricenter - quant->value(1) ) / ( 255 - quant->threshold(0) ) * 100;
    }

    for(int i = 0; i<quant->nbThresholds(); i++){
        
        valueCenter = ( quant->value(i) + quant->value(i+1) ) / 2 ; 
        neighbor += abs( valueCenter - quant->threshold(i) ) / ( quant->value(i) - quant->value(i+1) ) * 100;
    }
    
    neighbor = neighbor / quant->nbThresholds();
    if( quant->nbValues() == 2) centroid = centroid / 2;
    else centroid = centroid / ( quant->nbThresholds() - 1 );

    sprintf(buffer, "Canal : %d           Centroïd : %.2f %          Plus proche voisin : %.2f %\n", c, (100-centroid), (100-neighbor));

    return buffer;
}





void QuantificationOp::operator()(const imagein::Image* image, const std::map<const imagein::Image*, std::string>& imgList) {
    string quantType;
    string output_msg = "";
    string optiQuant;

    bool checkOptiQuant; 

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
            
            
            Quantification quantification = dialog->getQuantif(image, c, quantType, &checkOptiQuant);
            
            //Generate the text to print in the information window
            output_msg += quantificationOpLog(c, &quantification);


            for(unsigned int j = 0; j < image->getHeight(); ++j) {
                for(unsigned int i = 0; i < image->getWidth(); ++i) {
                    
                    const Image::depth_t value = image->getPixelAt(i, j, c);
                    resImg->setPixelAt(i, j, c, quantification.valueOf(value));
                }
            }
            if(checkOptiQuant)
                optiQuant += checkOptimumQuantizier(image, &quantification, c);
        }

        
        
        outText(quantType);
        outText(output_msg);
        if(checkOptiQuant){
            outText("Respect des conditions du quantifieur optimal : ");
            outText(optiQuant);
        }

        QString windowName;
        QString imgName;

        if(quantType=="Quantification non lineaire a valeurs centrees :")
            windowName = QString(qApp->translate("QuantificationOp","Quantification non lineaire a valeur centree"));
        else if(quantType=="Quantification non lineaire a valeurs moyennes : ")
            windowName = QString(qApp->translate("QuantificationOp","Quantification non lineaire a moyennes"));
        else if(quantType=="Quantification personnalisee :")
            windowName = QString(qApp->translate("QuantificationOp","Quantification personnalisee"));
        else if(quantType=="Quantification LloydMax :")
            windowName = QString(qApp->translate("QuantificationOp","Quantification LloydMax"));
        else windowName = QString(qApp->translate("QuantificationOp","Quantification lineaire a valeurs centrees"));


        if(image!=NULL){
            imgName = QString::fromStdString(imgList.find(image)->second);
            imgName.append(" - ");
        }
        else{
            imgName = QString("");
        }
        outImage(resImg, imgName.toStdString() + windowName.toStdString());
    }

}

