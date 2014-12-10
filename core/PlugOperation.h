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

#ifndef INSAIMAGE_PLUGOPERATION_H
#define INSAIMAGE_PLUGOPERATION_H

#include <vector>
#include <string>

#include "Input.h"
#include "Parameter.h"
#include <Image.h>
#include "Operation.h"

class QWidget;

/**
 * @brief Empty class used to identify the current standard image input.
 *
 */
struct CurrentImg {};
/**
 * @brief Empty class used to identify the current double image input.
 *
 */
struct CurrentDoubleImg {};

/**
 * @brief A PlugOperation is the base class for all Plugin's operations
 *
 * The PlugOperation class is used to easily implement an operation without writing user-interface code.\n
 * All the parameters of a PlugOperation must be defined in it's constructor using the addParam() methods. \n
 * The operation() method must implement the image processing algorithm and output some data using methods such as GenericOperation::outImage() or GenericOperation::outText().
 */
class PlugOperation : public GenericOperation {
  private:
    template<class D, class B> struct Derived_from {
        static void constraints(D* p) { B* pb = p; }
        Derived_from() { void(*p)(D*) = constraints; }
    };
    
  public:
/**
 * @brief A PlugOperation
 *
 * @param name Name of the operation
 */
    PlugOperation(std::string name);
  
    void operator()(const genericinterface::ImageWindow* currentWnd, const std::vector<const genericinterface::ImageWindow*>& imgWndList);
    virtual bool needCurrentImg() const { return _needCurrentImg; }
    virtual bool isValidImgWnd(const genericinterface::ImageWindow* imgWnd) const;

  protected:
    /**
     * @brief This method is called after the user specified all the operation's parameters, it should contains the operation's algorithm.
     *
     */
    virtual void operation() = 0;
    
    /**
     * @brief Add an input to the PlugOperation.
     *
     * @param input The input to add.
     */
    void addInput(Input* input);
    /**
     * @brief Add an input to the PlugOperation.
     *
     * @param input The input to add.
     */
    void addInput(const Input& input);
    
    template<typename T, class C>
    /**
     * @brief Add a Parameter input to the PlugOperation
     *
     * @param param The parameter to add.
     * @param C::ptr A pointer to the location where the input will be stored.
     */
    void addParam(const Parameter<T>& param, T C::* ptr) {
        Derived_from<C, GenericOperation>();
        C* object = dynamic_cast<C*>(this);
        if(object==NULL) {
            throw "The parameter's pointer doesn't belong to the class which add it";
        }
        Parameter<T>* newParam = param.clone();
        newParam->_ptr = &(object->*ptr);
        this->_inputs.push_back(newParam);
    }
    
    template<class C>
    /**
     * @brief Add the current standard image as a parameter
     *
     * @param C::ptr A pointer to the location where the input will be stored.
     */
    void addParam(const CurrentImg&, imagein::Image C::* ptr) {
        Derived_from<C, GenericOperation>();
        C* object = dynamic_cast<C*>(this);
        if(object==NULL) {
            throw "The parameter's pointer doesn't belong to the class which add it";
        }
        _needCurrentImg = true;
        _doubleCurrentImg = false;
        _currentStdImg = &(object->*ptr);
    }

    template<class C>
    /**
     * @brief Add the current double image as a parameter
     *
     * @param C::ptr A pointer to the location where the input will be stored.
     */
    void addParam(const CurrentDoubleImg&, imagein::Image_t<double> C::* ptr) {
        Derived_from<C, GenericOperation>();
        C* object = dynamic_cast<C*>(this);
        if(object==NULL) {
            throw "The parameter's pointer doesn't belong to the class which add it";
        }
        _needCurrentImg = true;
        _doubleCurrentImg = true;
        _currentDblImg = &(object->*ptr);
    }

  private:
    std::vector<Input*> _inputs; /**< List of the PlugOperation's inputs. */
    bool _needCurrentImg; /**< Wether this operation need a current image of not. */
    imagein::Image* _currentStdImg; /**< Pointer to the current standard Image if such exists, NULL otherwise. */
    imagein::Image_t<double>* _currentDblImg; /**< Point to the current double Image if such exists, NULL otherwise. */
    bool _doubleCurrentImg; /**< Wether this operation need a double wurrent image or a standard current image. */
};

#endif //!INSAIMAGE_OPERATION_H
