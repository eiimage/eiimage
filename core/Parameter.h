/*
 * Copyright 2011-2012 INSA Rennes
 * 
 * This file is part of INSAimage.
 * 
 * INSAimage is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * INSAimage is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with INSAimage.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef EIIMAGE_PARAMETER_H
#define EIIMAGE_PARAMETER_H

#include <string>

#include "Input.h"

template<typename T>
/**
 * @brief Parameter is a generic partial implementation of Input
 *
 * This class gather common code of most of the Input implementations.
 *
 */
class Parameter : public Input {
  public:
    
/**
 * @brief Constructor
 *
 * @param name Name of the input, as it will be displayed to the user
 * @param ptr Pointer to the location in which to store the given input.
 */
    Parameter(std::string name, T* ptr = NULL) : _ptr(ptr), _name(name) {}
    /**
     * @brief Copy constructor
     *
     * @return Parameter<T> Copy ot this instance
     */
    virtual Parameter<T>* clone() const = 0;
  
  protected:
    T* _ptr; /**< Pointer to the location in which to store the input */
    std::string _name; /**< Name of the input */
    
    friend class PlugOperation;
  
};

#endif //!EIIMAGE_PARAMETER_H
