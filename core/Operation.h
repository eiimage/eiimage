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

#ifndef EIIMAGE_OPERATION_H
#define EIIMAGE_OPERATION_H

#include <map>
#include <string>

#include "Output.h"
#include "Image.h"

class QWidget;
namespace genericinterface {
    class ImageWindow;
}

class GenericOperation {
  public:
    GenericOperation(std::string name) : _name(name) {}

    inline std::string getName() { return _name; }

    virtual std::vector<QWidget*> operator()(const genericinterface::ImageWindow* currentWnd, std::map<const genericinterface::ImageWindow*, std::string>&) = 0;

    virtual bool needCurrentImg() const = 0;
    virtual bool isValidImgWnd(const genericinterface::ImageWindow* imgWnd) const = 0;

  protected:
    std::string _name;
};

class Operation : public GenericOperation {
  public:
    Operation(std::string name) : GenericOperation(name) {}
    virtual std::vector<QWidget*> operator()(const genericinterface::ImageWindow* currentWnd, std::map<const genericinterface::ImageWindow*, std::string>&);
    virtual std::vector<QWidget*> operator()(const imagein::Image*, const std::map<const imagein::Image*, std::string>&) = 0;
    virtual bool isValidImgWnd(const genericinterface::ImageWindow* imgWnd) const;
  protected:
    imagein::Image* _currentImg;
};

class DoubleOperation : public GenericOperation {
  public:
    DoubleOperation(std::string name) : GenericOperation(name) {}
    virtual std::vector<QWidget*> operator()(const genericinterface::ImageWindow* currentWnd, std::map<const genericinterface::ImageWindow*, std::string>&);
    virtual std::vector<QWidget*> operator()(const imagein::Image_t<double>*, const std::map<const imagein::Image_t<double>*, std::string>&) = 0;
    virtual bool isValidImgWnd(const genericinterface::ImageWindow* imgWnd) const;
  protected:
    imagein::Image_t<double>* _currentImg;
};

#endif //!EIIMAGE_OPERATION_H
