#ifndef EIIMAGE_PLUGIN_H
#define EIIMAGE_PLUGIN_H

#include <string>
#include <vector>

#include "Operation.h"

class Plugin {
  public:
    Plugin(std::string name);
    std::vector<Operation*> getOperations();
    std::string getName();
    
  protected:
    void addOperation(Operation*);
    
  private:
    std::vector<Operation*> _operations;
    std::string _name;
};

#endif //!EIIMAGE_PLUGIN_H