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

#ifndef EIIMAGE_OPSET_H
#define EIIMAGE_OPSET_H

#include <string>
#include <vector>

#include "Operation.h"

/**
 * @brief Set of GenericOperation objects.
 *
 * An Operation set is used to represent a set of instances of differents implementations of the interface GenericOperation.
 *
 */
class OpSet {
  public:
/**
 * @brief Constructor
 *
 * @param name The name of the OpSet, as it should appear to the user.
 */
    OpSet(std::string name) : _name(name) {}
    /**
     * @brief Operations accessor
     *
     * @return std::vector<GenericOperation *> The set of operations contained in this Opset
     */
    virtual std::vector<GenericOperation*> getOperations() = 0;
    /**
     * @brief Name accessor
     *
     * @return std::string The name of this OpSet
     */
    inline std::string getName() { return _name; }
    
  private:
    std::string _name; /**< The name of this OpSet */
};

#endif //!EIIMAGE_OPSET_H
