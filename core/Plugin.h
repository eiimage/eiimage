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

#ifndef IMAGEINSA_PLUGIN_H
#define IMAGEINSA_PLUGIN_H

#include <string>
#include <vector>

#include "OpSet.h"
#include "PlugOperation.h"

/**
 * @brief A Plugin is an implementation of OpSet containing PlugOperation
 *
 * Unlike a BuiltinOpSet, a Plugin should only contain PlugOperation
 */
class Plugin : public OpSet {
  public:
/**
 * @brief Constructor
 *
 * @param name The name of the plugin, as it should be displayed to the user.
 */
    Plugin(std::string name);

    std::vector<GenericOperation*> getOperations();
    /**
     * @brief Add a PlugOperation to this Plugin.
     *
     * @param The operation to add.
     */
    void addOperation(PlugOperation* op);
    
  private:
    std::vector<GenericOperation*> _operations; /**< List of all the operations stored in this OpSet */
};

#endif //!IMAGEINSA_PLUGIN_H
