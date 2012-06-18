#ifndef EIIMAGE_IMGPARAM_H
#define EIIMAGE_IMGPARAM_H

#include <vector>
#include <string>
#include "Image.h"

#include "Parameter.h"

class ImgParam : public Parameter<imagein::Image> {
  public:
    ImgParam(std::string name, imagein::Image* ptr = NULL) : Parameter(name, ptr) {}
    virtual void fillDialog(QDialog*);
    virtual void pickValue();
    virtual Parameter* clone() const;
};

#endif //!EIIMAGE_IMGPARAM_H