#ifndef EIIMAGE_INPUT_H
#define EIIMAGE_INPUT_H

#include <vector>
#include <string>


class QDialog;
class Input {
  public:
    virtual Input* clone() const = 0;
    virtual void fillDialog(QDialog*) = 0;
    virtual void pickValue() = 0;
};

#endif //!EIIMAGE_INPUT_H