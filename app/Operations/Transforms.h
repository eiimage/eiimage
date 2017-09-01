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

#ifndef TRANSFORMS_H
#define TRANSFORMS_H

#include <Image.h>
#include <GrayscaleImage.h>
#include <string>

namespace Transforms
{
    imagein::Image_t<double> *hough( const imagein::GrayscaleImage *im ); // This function works
    imagein::Image_t<double> *hough2( const imagein::Image *im , double angleStep, double rhoStep); // This function works
    std::string hough2_inverse(const imagein::Image_t<double> *image, imagein::Image **resImg, unsigned int width, unsigned int height, unsigned int threshold);
    //std::string hough2_inverse(const imagein::Image_t<double> *image, imagein::Image **resImg, unsigned int size, unsigned int threshold);
    std::string Hadamard( const imagein::Image *im, imagein::Image_t<double> **result, imagein::Image **result_inverse, imagein::GrayscaleImage_t<bool> *selection = NULL);
    std::string Haar( const imagein::Image *im, imagein::Image_t<double> **result, imagein::Image **result_inverse, imagein::GrayscaleImage_t<bool> *selection = NULL );
    std::string cosinus( const imagein::Image *image, imagein::Image_t<double> **resImg, imagein::Image **invImg, imagein::GrayscaleImage_t<bool> *selection = NULL  );
};

#endif // TRANSFORMS_H
