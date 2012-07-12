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

#ifndef EIIMAGE_PLUGIN_H
#define EIIMAGE_PLUGIN_H

#include <string>
#include <vector>

#include "OpSet.h"
#include "PlugOperation.h"

class Plugin : public OpSet {
  public:
    Plugin(std::string name);
    std::vector<GenericOperation*> getOperations();
    void addOperation(PlugOperation*);
    
  private:
    std::vector<GenericOperation*> _operations;
};

#endif //!EIIMAGE_PLUGIN_H
