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
        for(int i = 0; i < nbThresholds(); ++i) {
            if(value < _threshold[i]) {
                return _values[i];
            }
        }
        return _values[nbThresholds()];
    }

    inline int nbValues() const {return size;}

    inline int nbThresholds() const {return size - 1;}

    inline int value(int i) const {return _values[i];}
    inline void setValue(int i, int v) {_values[i] = v;}

    inline int threshold(int i) const {return _threshold[i];}
    inline void setThreshold(int i, int v) {_threshold[i] = v;}

    static Quantification linearQuant(int size);
    static Quantification nonLinearQuant(int size, const imagein::Image *image, unsigned int c);
    static Quantification nonLinearQuantOptimized(int size, const imagein::Image *image, unsigned int c);

private:
    int size;
    int* _threshold;
    int* _values;
};

#endif // QUANTIFICATION_H
