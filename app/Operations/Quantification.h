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

#ifndef QUANTIFICATION_H
#define QUANTIFICATION_H

#include <Image.h>

#define N_MAX_THRESHOLD 256

class Quantification {
public:
    explicit Quantification(int size);
    explicit Quantification(std::string filename);

    void saveAs(std::string filename);

    inline int valueOf(int value) const {
        for(int i = 0; i < this->size - 1; ++i) {
            if(value < this->_threshold[i]) {
                return this->_values[i];
            }
        }
        return this->_values[this->size - 1];
    }

    inline int nbValues() {return size;}

    inline int nbThresholds() {return size - 1;}

    inline int value(int i) {return _values[i];}
    inline void setValue(int i, int v) {_values[i] = v;}

    inline int threshold(int i) {return _threshold[i];}
    inline void setThreshold(int i, int v) {_threshold[i] = v;}

    static Quantification linearQuant(int size);
    static Quantification nonLinearQuant(int size, const imagein::Image *image, unsigned int c);
    static Quantification nonLinearQuantOptimized(int size, const imagein::Image *image, unsigned int c);

private:
    int size;
    int* _threshold;
    int* _values;
};

class Quantifier {
public:
    Quantifier(Quantification quant);
    inline int valueOf(imagein::Image::depth_t value) {
        return this->values[value];
    }
private:
    int values[N_MAX_THRESHOLD];
};

#endif // QUANTIFICATION_H
