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
class EIImageService;
namespace genericinterface {
    class ImageWindow;
}

class GenericOperation {
  public:
    GenericOperation(std::string name) : _name(name), _ws(NULL), _curImgWnd(NULL) {}

    inline std::string getName() { return _name; }

    virtual void operator()(const genericinterface::ImageWindow* currentWnd, std::vector<genericinterface::ImageWindow*>&) = 0;
    virtual void operator()(EIImageService*);

    virtual bool needCurrentImg() const = 0;
    virtual bool isValidImgWnd(const genericinterface::ImageWindow* imgWnd) const = 0;



  protected:
    void outImage(imagein::Image*, std::string title = "");
    void outDoubleImage(imagein::Image_t<double>*, std::string title = "", bool norm=false, bool log=false, double logScale = 1., bool abs = false);
    void outText(std::string);
    std::string _name;
    EIImageService* _ws;
    genericinterface::ImageWindow* _curImgWnd;
  private:
    void outImgWnd(genericinterface::ImageWindow* imgWnd, std::string title);
};

class Operation : public GenericOperation {
  public:
    Operation(std::string name) : GenericOperation(name) {}
    virtual void operator()(const genericinterface::ImageWindow* currentWnd, std::vector<genericinterface::ImageWindow*>&);
    virtual void operator()(const imagein::Image*, const std::map<const imagein::Image*, std::string>&) = 0;
    virtual bool isValidImgWnd(const genericinterface::ImageWindow* imgWnd) const;
  protected:
    imagein::Image* _currentImg;
};

class DoubleOperation : public GenericOperation {
  public:
    DoubleOperation(std::string name) : GenericOperation(name) {}
    virtual void operator()(const genericinterface::ImageWindow* currentWnd, std::vector<genericinterface::ImageWindow*>&);
    virtual void operator()(const imagein::Image_t<double>*, const std::map<const imagein::Image_t<double>*, std::string>&) = 0;
    virtual bool isValidImgWnd(const genericinterface::ImageWindow* imgWnd) const;
  protected:
    imagein::Image_t<double>* _currentImg;
};

#endif //!EIIMAGE_OPERATION_H
