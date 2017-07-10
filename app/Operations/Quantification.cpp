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

#include "Quantification.h"
#include <fstream>
#include <iostream>
#include <cmath>

using namespace std;
using namespace imagein;

Quantification::Quantification(int size) {
    this->size = size;
    _threshold = new int[this->size - 1];
    _values = new int[this->size];
}

Quantification::Quantification(std::string filename) {
    ifstream file(filename.c_str(), fstream::in);
//    string s = file.getline();
    string s;
    file >> s;
    if(s.compare("Quant_Level_File") != 0) throw exception();
    file >> this->size;
    this->size++;
    if(this->size < 2) throw exception();
    _threshold = new int[this->size - 1];
    _values = new int[this->size];
    for(int i = 0; i < size - 1; ++i) {
        double n;
        file >> n;
        cout << n << endl;
        _threshold[i] = n;
    }
    for(int i = 0; i < size; ++i) {
        double n;
        file >> n;
        cout << n << endl;
        _values[i] = n;
    }
}

void Quantification::saveAs(std::string filename) {
    ofstream file(filename.c_str(), fstream::out);
    file << "Quant_Level_File" << endl;
    file << (this->size - 1) << endl;
    for(int i = 0; i < size - 1; ++i) {
        double n = _threshold[i];
        file << n << endl;
    }
    for(int i = 0; i < size; ++i) {
        double n = _values[i];
        file << n << endl;
    }
}

Quantification Quantification::linearQuant(int size) {

    Quantification quant(size);

    for(int i = 0; i < size - 1; ++i) {
        quant._threshold[i] = floor( (i + 1) * (float)N_MAX_THRESHOLD / size + 0.5);
    }
    if(size > 0) {
        quant._values[0] = floor( quant._threshold[0] / 2. + 0.5 );
        quant._values[size - 1] = floor( ((float)N_MAX_THRESHOLD + quant._threshold[size - 2]) / 2. + 0.5 );
    }
    for(int i = 1; i < size - 1; ++i) {
        quant._values[i] = floor( (double)(quant._threshold[i] + quant._threshold[i-1]) / 2. + 0.5 );
    }

    return quant;
}

Quantification Quantification::nonLinearQuant(int size, const Image* image, unsigned int c) {

    Quantification quant(size);
    Histogram histogram = image->getHistogram(c);

    double imageSize = image->getWidth() * image->getHeight();

    double histogramSum = 0;
    Image::depth_t value = 0;
    for(int i = 0; i < size - 1; ++i) {
        double percent = (i + 1.) / size;
        while(percent * imageSize > histogramSum) {
            histogramSum += histogram[value];
            ++value;
        }
        quant._threshold[i] = value - 1;
    }

    if(size > 0) {
        quant._values[0] = floor( quant._threshold[0] / 2. + 0.5 );
        quant._values[size - 1] = floor( ((float)N_MAX_THRESHOLD + quant._threshold[size - 2]) / 2. + 0.5 );
    }
    for(int i = 1; i < size - 1; ++i) {
        quant._values[i] = floor( (double)(quant._threshold[i] + quant._threshold[i-1]) / 2. + 0.5 );
    }
    return quant;
}

Quantification Quantification::nonLinearQuantOptimized(int size, const Image* image, unsigned int c) {

    Quantification quant(size);
    Histogram histogram = image->getHistogram(c);

    double imageSize = image->getWidth() * image->getHeight();

    double histogramSum = 0;
    Image::depth_t value = 0;
    for(int i = 0; i < size - 1; ++i) {
        double percent = (i + 1.) / size;
        while(percent * imageSize > histogramSum) {
            histogramSum += histogram[value];
            ++value;
        }
        quant._threshold[i] = value - 1;
    }

    if(size > 0) {
        quant._values[0] = floor( quant._threshold[0] / 2. + 0.5 );
        quant._values[size - 1] = floor( ((float)N_MAX_THRESHOLD + quant._threshold[size - 2]) / 2. + 0.5 );
    }
    for(int i = 1; i < size - 1; ++i) {
        quant._values[i] = floor( (double)(quant._threshold[i] + quant._threshold[i-1]) / 2. + 0.5 );
    }


    double som_lum = 0;
    int nb_points = 0;

    for(int j = 0; j < quant._threshold[0]; j++){
        som_lum += histogram[j] * j;
        nb_points += histogram[j];
    }

    quant._values[0] = som_lum / nb_points + 0.5;

    for(int i = 0; i < size - 2; ++i){
        som_lum = 0;
        nb_points = 0;
        for(int j = quant._threshold[i]; j < quant._threshold[i+1]; ++j) {
            som_lum += histogram[j] * j;
            nb_points += histogram[j];
        }
        quant._values[i+1] = som_lum / nb_points + 0.5;
    }

    som_lum = 0;
    nb_points = 0;
    for(int j = quant._threshold[size-2]; j < N_MAX_THRESHOLD; ++j) {
        som_lum += histogram[j] * j;
        nb_points += histogram[j];
    }
    quant._values[size-1] = som_lum / nb_points + 0.5;
    return quant;
}

Quantification Quantification::lloydMaxQuant(int size, const Image* image, unsigned int c) {

//    Quantification quant(size);

//    for(int i = 0; i < size - 1; ++i) {
//        quant._threshold[i] = floor( (i + 1) * (float)N_MAX_THRESHOLD / size + 0.5);
//    }
//    if(size > 0) {
//        quant._values[0] = floor( quant._threshold[0] / 2. + 0.5 );
//        quant._values[size - 1] = floor( ((float)N_MAX_THRESHOLD + quant._threshold[size - 2]) / 2. + 0.5 );
//    }
//    for(int i = 1; i < size - 1; ++i) {
//        quant._values[i] = floor( (double)(quant._threshold[i] + quant._threshold[i-1]) / 2. + 0.5 );
//    }

//    return quant;


    Histogram histogram = image->getHistogram(c);
    int som_lum = 0;
    int nb_points = 0;
    int cpt = 6;

    // initialisation : repartion lineaire des niveaux de quantification
    Quantification quant = linearQuant(size);

    while(cpt > 0){

        // calcul des nouveaux seuils de quantification
        for (int i=0; i<size-2;i++){
            quant._threshold[i] = (quant._values[i]+quant._values[i+1])/2;
        }//for (parcours du tableau de seuils)

        // calcul des nouveaux niveaux de quantification

        //calcul pdf

        for (int j=0; j<size-1;j++){
            for(int i= ((quant._threshold)[j]); i <= ((quant._threshold)[j+1]); i++){
                som_lum += histogram[i]*i;
                nb_points += histogram[i];
            }

        quant._values[j]= (int) quant._values[j]*(som_lum/nb_points) / (som_lum/nb_points);
        }//for (parcours du tableau de valeurs)

        cpt--;

    }//while

    return quant;
}
