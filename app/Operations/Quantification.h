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

struct Quantification {
    static Quantification linearQuant(int size);
    static Quantification nonLinearQuant(int size, const imagein::Image *image, unsigned int c);
    static Quantification nonLinearQuantOptimized(int size, const imagein::Image *image, unsigned int c);

    explicit Quantification(std::string filename);

    void saveAs(std::string filename);

    explicit Quantification(int size_);

    inline int valueOf(int value) const {
        for(int i = 0; i < this->size - 1; ++i) {
            if(value < this->threshold[i]) {
                return this->values[i];
            }
        }
        return this->values[this->size - 1];
    }
    int size;
    int* threshold;
    int* values;
};

struct Quantifier {
    Quantifier(Quantification quant);
    inline int valueOf(imagein::Image::depth_t value) {
        return this->values[value];
    }
  private:
    int values[N_MAX_THRESHOLD];
};

#endif // QUANTIFICATION_H
