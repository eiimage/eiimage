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

#ifndef MICD_H
#define MICD_H

#include <string>
#include <GrayscaleImage.h>
#include "Quantification.h"

class MICD
{
public:
    MICD();
    enum Prediction {PX_EQ_A, PX_EQ_B, PX_EQ_APC, PX_EQ_Q};
    virtual ~MICD();
    std::string execute( const imagein::GrayscaleImage *im, Prediction prediction_alg, imagein::Image **err_image, imagein::Image **recons_image, double Q = 0 );
    void setQuantification( Quantification* tquantdef );
private:
    std::string print_iloiqu();
    Quantification* quantdef;
    void codlq(int m);
    void codec(int nlq,int ier,int *icode,int *ireco);
    void set_levels();
    int iloiqu[128];
    int itcod[2048][20],itrco[2048][20],ktab[2][20];
};

#endif // MICD_H
