
#include "ImgParam.h"

using namespace imagein;

void ImgParam::fillDialog(QDialog*) {
}

void ImgParam::pickValue() {
}

Parameter<Image>* ImgParam::clone() const {
    return new ImgParam(*this);
}
