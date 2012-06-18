#include <iostream>
#include <string>

#include "Image.h"
#include "Algorithm/Binarization.h"
#include "Algorithm/Otsu.h"
#include "Algorithm/Dithering.h"
#include "Converter.h"

#include "../ImageEIICore/Operation.h"
#include "../ImageEIICore/Plugin.h"
#include "../ImageEIICore/ImgParam.h"
#include "../ImageEIICore/IntParam.h"
#include "../ImageEIICore/ImgOutput.h"

using namespace std;
using namespace imagein;

class Thresholding : public Operation {
  public:
    Thresholding(string name) : Operation(name) {
        this->addParam(CurrentImg(), &Thresholding::img);
        this->addParam(IntParam("Entier", 0, 255, 127), &Thresholding::threshold);
    }
    
    void operation() {
        algorithm::Binarization_t<depth_default_t> thresholding(threshold);
        GrayscaleImage* grayImg = imagein::Converter<GrayscaleImage>::convert(img);
        GrayscaleImage* result = thresholding(grayImg);
        this->addOutput(ImgOutput("", *result));
    }
  private:
    int threshold;
    Image img;
};

class Otsu : public Operation {
  public:
    Otsu(string name) : Operation(name) {
        this->addParam(CurrentImg(), &Otsu::img);
    }
    
    void operation() {
        algorithm::Otsu_t<depth_default_t> otsu;
        GrayscaleImage* grayImg = imagein::Converter<GrayscaleImage>::convert(img);
        GrayscaleImage* result = otsu(grayImg);
        this->addOutput(ImgOutput("", *result));
    }
  private:
    Image img;
};

class Dithering : public Operation {
  public:
    Dithering(string name) : Operation(name) {
        this->addParam(CurrentImg(), &Dithering::img);
    }
    
    void operation() {
        algorithm::Dithering_t<depth_default_t> dithering;
        Image* result = dithering(&img);
        this->addOutput(ImgOutput("", *result));
    }
  private:
    Image img;
};

class Segmentation : public Plugin {
  public:
    Segmentation(string name) : Plugin(name) {
        try {
            Thresholding* thresholding = new Thresholding("Thresholding");
            this->addOperation(thresholding);
            Otsu* otsu = new Otsu("Otsu");
            this->addOperation(otsu);
            Dithering* dithering = new Dithering("Dithering");
            this->addOperation(dithering);
        }
        catch(const char* msg) {
            std::cerr << msg << std::endl;
        }
    }
};

extern "C" Plugin* getPlugin() {
    Plugin* segmentation = new Segmentation("Segmentation");
    return segmentation;
}