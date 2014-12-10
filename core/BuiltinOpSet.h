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

#ifndef EIIMAGE_BUILTINOPSET_H
#define EIIMAGE_BUILTINOPSET_H

#include <string>
#include <vector>

#include "OpSet.h"

/**
 * @brief A BuiltinOpSet is an implementation of OpSet containing GenericOperation
 *
 * This class is used inside the eiimage application to organize the built-in operations.
 *
 */
class BuiltinOpSet : public OpSet {
  public:
/**
 * @brief Constructor
 *
 * @param name Name of the operation set, as it should be displayed to the user
 */
    BuiltinOpSet(std::string name);
    std::vector<GenericOperation*> getOperations();
    /**
     * @brief Add a GenericOperation to this operation set
     *
     * @param The operation to add.
     */
    void addOperation(GenericOperation* op);
    
  private:
    std::vector<GenericOperation*> _operations; /**< List of the operations stored in this OpSet */
};

#endif //!EIIMAGE_BUILTINOPSET_H
