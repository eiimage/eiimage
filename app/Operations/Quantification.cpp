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

#include "Quantification.h"
#include <fstream>

using namespace std;
using namespace imagein;

Quantification::Quantification(int size_) {
    this->size = size_;
    threshold = new int[this->size - 1];
    values = new imagein::Image::depth_t[this->size];
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
    threshold = new int[this->size - 1];
    values = new imagein::Image::depth_t[this->size];
    for(int i = 0; i < size - 1; ++i) {
        double n;
        file >> n;
        cout << n << endl;
        threshold[i] = n;
    }
    for(int i = 0; i < size; ++i) {
        double n;
        file >> n;
        cout << n << endl;
        values[i] = n;
    }
}

void Quantification::saveAs(std::string filename) {
    ofstream file(filename.c_str(), fstream::out);
    file << "Quant_Level_File" << endl;
    file << (this->size - 1) << endl;
    for(int i = 0; i < size - 1; ++i) {
        double n = threshold[i];
        file << n << endl;
    }
    for(int i = 0; i < size; ++i) {
        double n = values[i];
        file << n << endl;
    }
}


Quantifier::Quantifier(Quantification quant) {
    for(int i = 0; i < 256; ++i) {
        values[i] = quant.valueOf(i);
    }
}

Quantification Quantification::linearQuant(int size) {

    Quantification quant(size);

    for(int i = 0; i < size - 1; ++i) {
        quant.threshold[i] = floor( (i + 1) * 256. / size + 0.5);
    }
    if(size > 0) {
        quant.values[0] = floor( quant.threshold[0] / 2. + 0.5 );
        quant.values[size - 1] = floor( (256. + quant.threshold[size - 2]) / 2. + 0.5 );
    }
    for(int i = 1; i < size - 1; ++i) {
        quant.values[i] = floor( (double)(quant.threshold[i] + quant.threshold[i-1]) / 2. + 0.5 );
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
        quant.threshold[i] = value - 1;
    }

    if(size > 0) {
        quant.values[0] = floor( quant.threshold[0] / 2. + 0.5 );
        quant.values[size - 1] = floor( (256. + quant.threshold[size - 2]) / 2. + 0.5 );
    }
    for(int i = 1; i < size - 1; ++i) {
        quant.values[i] = floor( (double)(quant.threshold[i] + quant.threshold[i-1]) / 2. + 0.5 );
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
        quant.threshold[i] = value - 1;
    }

    if(size > 0) {
        quant.values[0] = floor( quant.threshold[0] / 2. + 0.5 );
        quant.values[size - 1] = floor( (256. + quant.threshold[size - 2]) / 2. + 0.5 );
    }
    for(int i = 1; i < size - 1; ++i) {
        quant.values[i] = floor( (double)(quant.threshold[i] + quant.threshold[i-1]) / 2. + 0.5 );
    }


    double som_lum = 0;
    int nb_points = 0;

    for(int j = 0; j < quant.threshold[0]; j++){
        som_lum += histogram[j] * j;
        nb_points += histogram[j];
    }


    quant.values[0] = som_lum / nb_points + 0.5;


    for(int i = 0; i < size - 2; ++i){
        som_lum = 0;
        nb_points = 0;
        for(int j = quant.threshold[i]; j < quant.threshold[i+1]; ++j) {
            som_lum += histogram[j] * j;
            nb_points += histogram[j];
        }
        quant.values[i+1] = som_lum / nb_points + 0.5;
    }


    som_lum = 0;
    nb_points = 0;
    for(int j = quant.threshold[size-2]; j < 256; ++j) {
        som_lum += histogram[j] * j;
        nb_points += histogram[j];
    }
    quant.values[size-1] = som_lum / nb_points + 0.5;
    return quant;
}
