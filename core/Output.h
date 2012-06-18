#ifndef EIIMAGE_OUTPUT_H
#define EIIMAGE_OUTPUT_H

#include <vector>
#include <string>


class QWidget;
class Output {
  public:
    Output(std::string name) : _name(name) {}
    virtual QWidget* getWidget() = 0;
    virtual Output* clone() const = 0;
  protected:
    std::string _name;
};

#endif //!EIIMAGE_OUTPUT_H