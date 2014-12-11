/*
 * Copyright 2011-2012 INSA Rennes
 * 
 * This file is part of ImageINSA.
 * 
 * ImageINSA is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ImageINSA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with ImageINSA.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <string>

#include "Image.h"

#include "Operation.h"
#include "Plugin.h"
#include "ImgParam.h"
#include "IntParam.h"
#include "ImgOutput.h"

using namespace std;
using namespace imagein;

Image* generateRefImg() {
    //Reference Image
    int width = 768;
    int height = 512;
    int nbChannels = 3;
    unsigned char* dataRgb = new unsigned char[width * height * nbChannels];

    for(int i = 0 ; i < width ; ++i) {
        short red = std::max(255 - i, 255-(768-i));
        if(red < 0) red = 0;
        short green = 255 - std::abs(256-i);
        if(green < 0) green = 0;
        short blue = 255 - std::abs(512-i);
        if(blue < 0) blue = 0;

        double offset = std::max(std::max((double)red/255.0, (double)green/255.0), (double)blue/255.0);
        if(offset<1) red /= offset; green /=offset; blue/=offset;

        for(int j = 0 ; j < height ; ++j) {
        short r = red, g = green, b = blue;
            if(j<256) {
                r = std::min(r+256-j, 255);
                g = std::min(g+256-j, 255);
                b = std::min(b+256-j,255);
            }
            else {

                    r = std::max(r+256-j, 0);
                    g = std::max(g+256-j, 0);
                    b = std::max(b+256-j,0);
            }

            dataRgb[j*width*nbChannels + i*nbChannels] = r;
            dataRgb[j*width*nbChannels + i*nbChannels + 1] = g;
            dataRgb[j*width*nbChannels + i*nbChannels + 2] = b;
        }
    }
    return new Image(width, height, nbChannels, dataRgb);
}

class MyOperation : public Operation {
  public:
    MyOperation(string name) : Operation(name) {
        //this->addParam(ImgParam("image"), &MyOperation::image); 
        //this->addParam(CurrentImg(), &MyOperation::img);
        //this->addParam(IntParam("Entier", 0, 255, 127), &MyOperation::n); 
    }
    
    void operation() {
        cout << "n = " << n << endl;
        
        Image* res = generateRefImg();
        
        this->addOutput(ImgOutput("ImageRef", *res));
    }
  private:
    int n;
    Image img;
};
class Identity : public Operation {
  public:
    Identity(string name) : Operation(name) {
       this->addParam(CurrentImg(), &Identity::img);
    }
    
    void operation() {
        this->addOutput(ImgOutput("ImageRef", img));
    }
  private:
    Image img;
};

class MyPlugin : public Plugin {
  public:
    MyPlugin(string name) : Plugin(name) {
        try {
            MyOperation* op = new MyOperation("Générer image");
            this->addOperation(op);
            this->addOperation(new Identity("Identité"));
            /*MyOperation* op2 = new MyOperation("Dilatation");
            this->addOperation(op2);*/
        }
        catch(const char* msg) {
            std::cerr << msg << std::endl;
        }
    }
};

extern "C" Plugin* getPlugin() {
    Plugin* myPlugin = new MyPlugin("ImageRef");
    return myPlugin;
}
