#ifndef EIIMAGE_IMGOUTPUT_H
#define EIIMAGE_IMGOUTPUT_H

#include <vector>
#include <string>

#include "Image.h"
#include "Output.h"


class QWidget;
class ImgOutput : public Output {
  public:
    ImgOutput(const imagein::Image& image);
    ImgOutput(std::string name, const imagein::Image& image);
    virtual QWidget* getWidget();
    ImgOutput* clone() const;
  private:
    imagein::Image* _img;
};

#endif //!EIIMAGE_IMGOUTPUT_H