#include "ImgWidget.h"

#include "Algorithm/Binarization.h"
#include "Converter.h"

#include "ThresholdOp.h"
#include "ThresholdDialog.h"

using namespace std     ;
using namespace imagein;
using namespace imagein::algorithm;

ThresholdOp::ThresholdOp() : Operation("ThresholdOp") {
}

bool ThresholdOp::needCurrentImg() {
    return true;
}

std::vector<QWidget*> ThresholdOp::operator()(const imagein::Image* image, const std::map<std::string, const imagein::Image*>&) {
    vector<QWidget*> result;
    
    const GrayscaleImage* img = dynamic_cast<const GrayscaleImage*>(image);
    bool convert = (img == NULL);
    if(convert) {
        img = Converter<GrayscaleImage>::convert(*image);
    }

    ThresholdDialog* dialog = new ThresholdDialog(img, convert);
    
    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());
    
    if(code!=QDialog::Accepted) {
        return result;
    }

    Binarization_t<GrayscaleImage::depth_t>* algo;
    if(dialog->doubleThreshold()) {
        algo = new Binarization_t<GrayscaleImage::depth_t>(dialog->threshold1(), dialog->threshold2(), dialog->blackBand());
    }
    else {
        algo = new Binarization_t<GrayscaleImage::depth_t>(dialog->threshold1());
    }

    GrayscaleImage* resImg = algo->operator()(img);
    result.push_back(new ImgWidget(resImg, ""));

    return result;
}
