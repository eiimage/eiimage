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
#include "Converter.h"

#include "Operation.h"
#include "Plugin.h"
#include "ImgParam.h"
#include "IntParam.h"
#include "PlugOperation.h"

#include <cstring>
#include <cstdio>
#include <cmath>

using namespace std;
using namespace imagein;


class EntropyOp : public PlugOperation {
  public:
    EntropyOp() : PlugOperation("Entropy") {
        addParam(CurrentImg(), &EntropyOp::img);
//        addParam(IntParam("Entier", 0, 255, 127), &Entropy::threshold);
        //this->addParam(ImgParam("Image"), &Entropy::img);
    }

    void operation() {

        double entropy = 0.;

        // for each image channel
        for(unsigned int c = 0; c < img.getNbChannels(); ++c) {
            Histogram histo = img.getHistogram(c);

            // A MODIFIER !!!!!
            for(int i = 0; i < 256; ++i) {
                if(histo[i] > 0) {
                    double p = (double)histo[i] / img.getWidth() /img.getHeight();
                    entropy +=  p * log(p);
                }
            }
        }
        entropy = - entropy / log(2);

    //        this->outImage(result);
        char buffer[255];
        sprintf( buffer, "Entropy of the image = %.2f bpp", entropy );
        string s = buffer ;
        outText(s);
    }
  private:
    Image img;
};

extern "C" Plugin* loadPlugin() {
    Plugin* plugin = new Plugin("Entropy");
    plugin->addOperation(new EntropyOp());
    return plugin;
}

extern "C" void unloadPlugin(Plugin* plugin) {
    delete plugin;
}

//
//extern "C" Plugin* getPlugin() {
//    Plugin* myPlugin = new MyPlugin("ImageRef");
//    return myPlugin;
//}
//
//class MyOperation : public Operation {
//  public:
//    MyOperation(string name) : Operation(name) {
//        //this->addParam(ImgParam("image"), &MyOperation::image);
//        //this->addParam(CurrentImg(), &MyOperation::img);
//        //this->addParam(IntParam("Entier", 0, 255, 127), &MyOperation::n);
//    }
//
//    void operation() {
//        cout << "n = " << n << endl;
//
//        Image* res = generateRefImg();
//
//        this->addOutput(ImgOutput("ImageRef", *res));
//    }
//  private:
//    int n;
//    Image img;
//};
//
//class MyPlugin : public Plugin {
//  public:
//    MyPlugin(string name) : Plugin(name) {
//        try {
//            MyOperation* op = new MyOperation("Générer image");
//            this->addOperation(op);
//            //this->addOperation(new Identity("Identité"));
//            /*MyOperation* op2 = new MyOperation("Dilatation");
//            this->addOperation(op2);*/
//        }
//        catch(const char* msg) {
//            std::cerr << msg << std::endl;
//        }
//    }
//};
//
//
//extern "C" Plugin* getPlugin() {
//    Plugin* myPlugin = new MyPlugin("ImageRef");
//    return myPlugin;
//}
