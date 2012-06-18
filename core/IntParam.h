#ifndef EIIMAGE_INTPARAM_H
#define EIIMAGE_INTPARAM_H

#include <vector>
#include <string>
#include "Image.h"

#include "Parameter.h"

class QSpinBox;
class QDialog;
class IntParam : public Parameter<int> {
  public:
    IntParam(std::string name, int min, int max, int def = 0);
    virtual void fillDialog(QDialog*);
    virtual void pickValue();
    virtual IntParam* clone() const;
  protected:
    int _min, _max, _def;
    QSpinBox* _spinbox;
};

#endif //!EIIMAGE_INTPARAM_H