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