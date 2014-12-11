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

#ifndef CLASSANALYSIS_H
#define CLASSANALYSIS_H

#include <Image.h>
#include <string>
#include <vector>
#include <GrayscaleImage.h>

namespace ClassAnalysis
{
    void analyse(int fen, uint8_t *imorig,double *param1,double *param2,int nbl,int nbc );
    void estimateur(double *param1,double *param2,double moy1[],double var1[],double moy2[],double var2[],int fen,int nbl,int nbc, const std::vector<imagein::Rectangle>& rectangles);
    void classif(double *param1,double *param2,double moy1[],double var1[],double moy2[],double var2[],int fen,int nbl,int nbc, imagein::Image **classify_result, int num_classes );
    void write_to_file( imagein::GrayscaleImage *learning, const std::vector<imagein::Rectangle>& rectangles, FILE *f, int learning_fen, imagein::Image_t<double> **returnval_mean, imagein::Image_t<double> **returnval_stdev );
    void NormalizeRectangle( imagein::Rectangle &rect );
    imagein::Image *classify_from_file( imagein::GrayscaleImage *to_classify, FILE *f, int classify_fen, imagein::Image_t<double> **returnval_mean, imagein::Image_t<double> **returnval_stdev );
    std::string print_file_info( FILE *f );
}

#endif // CLASSANALYSIS_H
