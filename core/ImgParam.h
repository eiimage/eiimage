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

#ifndef EIIMAGE_IMGPARAM_H
#define EIIMAGE_IMGPARAM_H

#include <vector>
#include <string>
#include "Image.h"

#include "Parameter.h"

class ImgParam : public Parameter<imagein::Image> {
  public:
    ImgParam(std::string name, imagein::Image* ptr = NULL) : Parameter(name, ptr) {}
    virtual void fillDialog(QDialog*);
    virtual void pickValue();
    virtual Parameter* clone() const;
};

#endif //!EIIMAGE_IMGPARAM_H