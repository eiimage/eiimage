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

#ifndef QUANTIFICATION_H
#define QUANTIFICATION_H

#include <Image.h>

#define N_MAX_THRESHOLD 256
#define N_MAX_THRESHOLD_FULL 512

class Quantification {
public:

    explicit Quantification(int size);
    explicit Quantification(std::string filename);
    /*identical quantification*/
    explicit Quantification();

    void saveAs(std::string filename);

    int valueOf(int value) const ;

    inline int nbValues() const {return size;}

    inline int nbThresholds() const {return size - 1;}

    inline int value(int i) const {return _values[i];}
    inline void setValue(int i, int v) {_values[i] = v;}

    inline int threshold(int i) const {return _threshold[i];}
    inline void setThreshold(int i, int v) {_threshold[i] = v;}

    static Quantification linearQuant(int size);
    static Quantification nonLinearQuant(int size, const imagein::Image *image, unsigned int c);
    static Quantification nonLinearQuantOptimized(int size, const imagein::Image *image, unsigned int c);
    static Quantification lloydMaxQuant(int size, const imagein::Image* image, unsigned int c);

    /*linear quantificator for dpcm(including negatif values)*/
    static Quantification linearQuant_DPCM(int size);

private:
    int size;
    int* _threshold;
    int* _values;
};

#endif // QUANTIFICATION_H
