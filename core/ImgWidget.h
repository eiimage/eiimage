#include "Image.h"
#include <QWidget>
#include <string>

class ImgWidget : public QWidget {
  public:
    ImgWidget(imagein::Image* img_, std::string name_) : img(img_), name(name_) {}
    imagein::Image* img;
    std::string name;
};