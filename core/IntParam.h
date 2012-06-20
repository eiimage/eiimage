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

#ifndef EIIMAGE_INTPARAM_H
#define EIIMAGE_INTPARAM_H

#include <vector>
#include <string>
#include "Image.h"

#include "Parameter.h"

class QSpinBox;
class QDialog;
class IntParam : public Parameter<int> {
  public:
    IntParam(std::string name, int min, int max, int def = 0);
    virtual void fillDialog(QDialog*);
    virtual void pickValue();
    virtual IntParam* clone() const;
  protected:
    int _min, _max, _def;
    QSpinBox* _spinbox;
};

#endif //!EIIMAGE_INTPARAM_H