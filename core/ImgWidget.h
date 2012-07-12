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

#include "Image.h"
#include <QWidget>
#include <string>

class ImgWidget : public QWidget {
  public:
    ImgWidget(imagein::Image* img_, std::string name_) : img(img_), name(name_) {}
    imagein::Image* img;
    std::string name;
};


class DoubleImgWidget : public QWidget {
  public:
    DoubleImgWidget(imagein::Image_t<double>* img_, std::string name_, bool normalize_ = false, bool logScale_ = false)
        : img(img_), name(name_), normalize(normalize_), logScale(logScale_) {}
    imagein::Image_t<double>* img;
    std::string name;
    bool normalize;
    bool logScale;
};
