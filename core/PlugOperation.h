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

#ifndef EIIMAGE_PLUGOPERATION_H
#define EIIMAGE_PLUGOPERATION_H

#include <vector>
#include <string>

#include "Input.h"
#include "Parameter.h"
#include "Output.h"
#include "Image.h"
#include "Operation.h"

class QWidget;

struct CurrentImg {
};

class PlugOperation : public Operation {
  private:
    template<class D, class B> struct Derived_from {
        static void constraints(D* p) { B* pb = p; }
        Derived_from() { void(*p)(D*) = constraints; }
    };
    
  public:
    PlugOperation(std::string name);
  
    std::vector<QWidget*> operator()(const imagein::Image*, const std::vector<const imagein::Image*>&);
    
    inline bool needCurrentImg() { return _needCurrentImg; }
    
  protected:
    virtual void operation() = 0;
    
    void addInput(Input* input);
    void addInput(const Input& input);
    void addOutput(const Output& output);
    
    template<typename T, class C>
    void addParam(const Parameter<T>& param, T C::* ptr) {
        Derived_from<C, Operation>();
        C* object = dynamic_cast<C*>(this);
        if(object==NULL) {
            throw "The parameter's pointer doesn't belong to the class which add it";
        }
        Parameter<T>* newParam = param.clone();
        newParam->_ptr = &(object->*ptr);
        this->_inputs.push_back(newParam);
    }
    
    template<class C>
    void addParam(const CurrentImg&, imagein::Image C::* ptr) {
        Derived_from<C, Operation>();
        C* object = dynamic_cast<C*>(this);
        if(object==NULL) {
            throw "The parameter's pointer doesn't belong to the class which add it";
        }
        _needCurrentImg = true;
        _currentImg = &(object->*ptr);
    }
    
  private:
    std::vector<Input*> _inputs;
    std::vector<Output*> _outputs;
    bool _needCurrentImg;
    imagein::Image* _currentImg;
};

#endif //!EIIMAGE_OPERATION_H
