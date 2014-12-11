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

#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <GrayscaleImage.h>

class Huffman
{
  public:
    Huffman();
    virtual ~Huffman();
    std::string execute( const imagein::GrayscaleImage *im );
  private:
    void clearMem();
    int size,nbeff,*ilon;
    double *Pi,*codPi,H,nbbit;
    short *indicePi;
    char *chain;
    std::string prob_Pi(const imagein::GrayscaleImage *im, int nbpt);
    void codhuffman(void);
};
#endif // HUFFMAN_H
