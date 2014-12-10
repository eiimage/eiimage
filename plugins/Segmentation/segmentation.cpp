/*
 * Copyright 2011-2012 INSA Rennes
 *
 * This file is part of INSAimage.
 *
 * INSAimage is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * INSAimage is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with INSAimage.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <string>

#include "Image.h"
#include "Algorithm/Binarization.h"
#include "Algorithm/Otsu.h"
#include "Algorithm/Dithering.h"
#include "Converter.h"

#include "Operation.h"
#include "Plugin.h"
#include "ImgParam.h"
#include "IntParam.h"

using namespace std;
using namespace imagein;

class Thresholding : public PlugOperation {
  public:
    Thresholding() : PlugOperation("Thresholding") {
        addParam(CurrentImg(), &Thresholding::img);
        addParam(IntParam("Entier", 0, 255, 127), &Thresholding::threshold);
    }

    void operation() {
        algorithm::Binarization_t<depth_default_t> thresholding(threshold);
        GrayscaleImage* grayImg = imagein::Converter<GrayscaleImage>::convert(img);
        GrayscaleImage* result = thresholding(grayImg);
        outImage(result);
    }
  private:
    int threshold;
    Image img;
};

class Otsu : public PlugOperation {
  public:
    Otsu() : PlugOperation("Otsu") {
        this->addParam(CurrentImg(), &Otsu::img);
    }

    void operation() {
        algorithm::Otsu_t<depth_default_t> otsu;
        GrayscaleImage* grayImg = imagein::Converter<GrayscaleImage>::convert(img);
        GrayscaleImage* result = otsu(grayImg);
        outImage(result);
    }
  private:
    Image img;
};

class Dithering : public PlugOperation {
  public:
    Dithering() : PlugOperation("Dithering") {
        this->addParam(CurrentImg(), &Dithering::img);
//        this->addParam(ImgParam("Image"), &Dithering::img);
    }

    void operation() {
        algorithm::Dithering_t<depth_default_t> dithering;
        Image* result = dithering(&img);
        this->outImage(result);
        this->outText("test");
    }
  private:
    Image img;
};

extern "C" Plugin* loadPlugin() {
    Plugin* plugin = new Plugin("Binarization");
    plugin->addOperation(new Thresholding());
    plugin->addOperation(new Otsu());
    plugin->addOperation(new Dithering());
    return plugin;
}

extern "C" void unloadPlugin(Plugin* plugin) {
    delete plugin;
}
