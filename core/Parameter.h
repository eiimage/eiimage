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

#ifndef EIIMAGE_PARAMETER_H
#define EIIMAGE_PARAMETER_H

#include <string>

#include "Input.h"

class Operation;

template<typename T>
class Parameter : public Input {
  public:
    
    Parameter(std::string name, T* ptr = NULL) : _ptr(ptr), _name(name) {}
    virtual Parameter<T>* clone() const = 0;
  
  protected:
    T* _ptr;
    std::string _name;
    
    friend class Operation;
  
};

#endif //!EIIMAGE_PARAMETER_H