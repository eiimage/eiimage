#include "ImgOutput.h"
#include "ImgWidget.h"

using namespace std;

ImgOutput::ImgOutput(const imagein::Image& image) : Output(""), _img(new imagein::Image(image)) {
}
ImgOutput::ImgOutput(string name, const imagein::Image& image) : Output(name), _img(new imagein::Image(image)) {
}
    
QWidget* ImgOutput::getWidget() {
    return new ImgWidget(_img, _name);
}


ImgOutput* ImgOutput::clone() const {
    return new ImgOutput(*this);
}