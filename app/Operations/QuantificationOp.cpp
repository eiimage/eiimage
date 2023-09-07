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
#include <Converter.h>
#include <QApplication>
#include <Widgets/ImageWidgets/DoubleImageWindow.h>
#include <Widgets/ImageWidgets/StandardImageWindow.h>

using namespace std;
using namespace imagein;
using namespace genericinterface;

QuantificationOp::QuantificationOp() : GenericOperation(qApp->translate("Operations", "Quantification").toStdString())
{
  _test=false, _quantif=0, _values=2;
}


bool QuantificationOp::needCurrentImg() const {
    return true;
}

string QuantificationOp::quantificationOpLog(unsigned int c, Quantification * quant){
    char buffer[30];
    string output_msg;
    int val;
    QString bob(qApp->translate("Operations", "\nCanal%d : \n"));

    sprintf(buffer, bob.toUtf8(), c);
    output_msg += buffer;

    output_msg += QString(qApp->translate("Operations","             Valeurs :  ")).toStdString();

    for(int i = 0; i < quant->nbValues(); ++i) {

        val =(int)quant->value(i);
        if(i != 0) output_msg += " |  ";

        sprintf(buffer, "%d ", val);
        output_msg += buffer;
        if(val < 10) output_msg += " ";
        if(val < 100 ) output_msg += " ";


    }
    output_msg += "\n";
    output_msg += QString(qApp->translate("Operations","             Seuils    :       ")).toStdString();

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
        if(nb_points != 0) baricenter = som_lum/nb_points;
        else baricenter =  ( quant->threshold(j) + quant->threshold(j-1) )/2;

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
        if(nb_points != 0) baricenter = som_lum/nb_points;
        else baricenter =  (quant->threshold(0))/2;;
        centroid = abs( baricenter - quant->value(0) ) /( quant->threshold(0) ) * 100;

        som_lum = 0;
        nb_points = 0;
        for(int i= quant->threshold(0); i <= 255 ; i++){
                som_lum += hist[i]*i;
                nb_points += hist[i];
            }
        if(nb_points != 0) baricenter = som_lum/nb_points;
        else baricenter =  255 - quant->threshold(0)/2;
        centroid += abs( baricenter - quant->value(1) ) / ( 255 - quant->threshold(0) ) * 100;
    }
    for(int i = 0; i<quant->nbThresholds(); i++){

        valueCenter = ( quant->value(i) + quant->value(i+1) ) / 2 ;
        neighbor += abs( valueCenter - quant->threshold(i) ) / ( quant->value(i) - quant->value(i+1) ) * 100;
    }

    neighbor = neighbor / quant->nbThresholds();
    if( quant->nbValues() == 2) centroid = centroid / 2;
    else centroid = centroid / ( quant->nbThresholds() - 1 );

    sprintf(buffer, QString(qApp->translate("Operations","Canal : %d           Centroïd : %.2f %          Plus proche voisin : %.2f %\n")).toUtf8(), c, (100-centroid), (100-neighbor));

    return buffer;
}



void QuantificationOp::operator()(const genericinterface::ImageWindow *currentWnd,
                                  const vector<const genericinterface::ImageWindow *> &imgWndList) {

    string quantType;
    string output_msg;


    if(currentWnd->isStandard()) {
        QuantificationDialog* dialog;

        QString imgName = dynamic_cast<const StandardImageWindow *>(currentWnd)->windowTitle();
        dialog = new QuantificationDialog(QApplication::activeWindow(), imgName);


        if(_test){
            dialog->setQuantif(_quantif);
            dialog->setValues(_values);
        }else{
            auto code = static_cast<QDialog::DialogCode>(dialog->exec());
            if(code!=QDialog::Accepted) return;
        }


        const auto *wnd = dynamic_cast<const StandardImageWindow *>(currentWnd);
        auto* resImg = new Image(*wnd->getImage());

        for(unsigned int c = 0; c < resImg->getNbChannels(); ++c) {
            Quantification quantification = dialog->getQuantif(currentWnd, c, quantType);

            //Generate the text to print in the information window
            output_msg += quantificationOpLog(c, &quantification);

            for(unsigned int j = 0; j < resImg->getHeight(); ++j) {
                for(unsigned int i = 0; i < resImg->getWidth(); ++i) {

                    const Image::depth_t value = resImg->getPixelAt(i, j, c);
                    resImg->setPixelAt(i, j, c, quantification.valueOf(value));
                }
            }
        }


        outText(quantType);
        outText(output_msg);
        outText("-------------------------------------------");

        this->outImage(resImg, quantType.erase(quantType.length()-2, quantType.length()));
    }

    else if(currentWnd->isDouble()) {
        QuantificationDialog* dialog;

        QString imgName = dynamic_cast<const DoubleImageWindow *>(currentWnd)->windowTitle();
        dialog = new QuantificationDialog(QApplication::activeWindow(), imgName);


        if(_test){
            dialog->setQuantif(_quantif);
            dialog->setValues(_values);
        }else{
            auto code = static_cast<QDialog::DialogCode>(dialog->exec());
            if(code!=QDialog::Accepted) return;
        }


        const auto *wnd = dynamic_cast<const DoubleImageWindow *>(currentWnd);
        auto* resImg = new Image_t<double>(*wnd->getImage());

        for(unsigned int c = 0; c < resImg->getNbChannels(); ++c) {
            Quantification quantification = dialog->getQuantif(currentWnd, c, quantType);

            //Generate the text to print in the information window
            output_msg += quantificationOpLog(c, &quantification);


            for(unsigned int j = 0; j < resImg->getHeight(); ++j) {
                for(unsigned int i = 0; i < resImg->getWidth(); ++i) {
                    const auto value = (int)resImg->getPixelAt(i, j, c);    //On utilise le type d'image double pour faire du signed int
                    resImg->setPixelAt(i, j, c, quantification.valueOf(value));
                }
            }
        }



        outText(quantType);
        outText(output_msg);
        outText("-------------------------------------------");

        this->outDoubleImage(resImg, imgName.toStdString() + quantType.erase(quantType.length()-2, quantType.length()));
    }
}

void QuantificationOp::setTest(bool a){
  _test=a;
}

void QuantificationOp::setQuantif(int a){
  _quantif=a;
}

void QuantificationOp::setValues(int a ){
  _values = a;
}

bool QuantificationOp::isValidImgWnd(const genericinterface::ImageWindow *imgWnd) const{
    return (imgWnd && (imgWnd->isStandard() || imgWnd->isDouble()));
}


