#ifndef EIIMAGE_OPERATION_H
#define EIIMAGE_OPERATION_H

#include <vector>
#include <string>

#include "Input.h"
#include "Parameter.h"
#include "Output.h"
#include "Image.h"

class QWidget;

struct CurrentImg {
};

class Operation {
  private:
    template<class D, class B> struct Derived_from {
        static void constraints(D* p) { B* pb = p; }
        Derived_from() { void(*p)(D*) = constraints; }
    };
    
  public:
    Operation(std::string name);
  
    std::vector<QWidget*> operator()(const imagein::Image*);
    std::string getName();
    
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
    std::string _name;
    std::vector<Input*> _inputs;
    std::vector<Output*> _outputs;
    bool _needCurrentImg;
    imagein::Image* _currentImg;
};

#endif //!EIIMAGE_OPERATION_H