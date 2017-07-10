/*
 * Copyright 2011-2012 INSA Rennes
 *
 * This file is part of EIImage.
 *
 * EIImage is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EIImage is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EIImage.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <string>
#include <algorithm>

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
#include <vector>
#include <algorithm>

using namespace std;
using namespace imagein;

class MedianCroix : public PlugOperation {
  public:
    MedianCroix() : PlugOperation("MedianCroix") {
        addParam(CurrentImg(), &MedianCroix::img);
        addParam(IntParam("Taille",1,30,3),&MedianCroix::wSide);
    }
    void operation() {
        Image *outputImage = new Image(img);
        unsigned int histo[256];
        unsigned int imageWidth = img.getWidth();
        unsigned int imageHeight = img.getHeight();
        unsigned int nbChannels = img.getNbChannels();
        int iStart, iStop, jStart, jStop;

   // parcours des composantes de couleur
    for (int c=0;c<nbChannels;c++){

        // parcours de l'image
        for (int x=0;x<imageWidth;x++){
            for(int y=0;y<imageHeight;y++){
                    //parcours de la fenêtre du filtre

                    x-wSide/2 < 0 ? iStart = 0 : iStart = x-wSide/2;
                    x+wSide/2 > imageWidth-1 ? iStop = imageWidth : iStop = x+wSide/2+1;
                    y-wSide/2 < 0 ? jStart = 0 : jStart = y-wSide/2;
                    y+wSide/2 > imageHeight-1 ? jStop = imageHeight : jStop = y+wSide/2+1;


                    for(int i=iStart; i< iStop ; i++){
                        for(int j=jStart;j<jStop;j++){
                            //remplissage de la fenêtre du filtre
                            if (i==x || j==y)window.push_back(img.getPixelAt(i,j,c));

                        }//j
                    }//i

                size_t size = window.size();
                sort(window.begin(), window.begin()+size);
                outputImage->setPixelAt(x, y, c, window[size/2]);
                window.clear();
            }//y
        }//x
    }//c

        //conversion de la taille de la fenêtre en string pour l'affichage de l'image résultat
        string str;
        ostringstream convert;
        convert << wSide;
        str = convert.str();

        outImage(outputImage, "Image filtered (median"+str+"x"+str+")");
    }
  private:
    Image img;
    int wSide;//largeur de la fenêtre
    vector<int> window;
};

class MedianCarre : public PlugOperation {
  public:
    MedianCarre() : PlugOperation("MedianCarre") {
        addParam(CurrentImg(), &MedianCarre::img);
        addParam(IntParam("Taille",1,30,3),&MedianCarre::wSide);
    }

    void operation() {
        Image *outputImage = new Image(img);
        unsigned int histo[256];
        unsigned int imageWidth = img.getWidth();
        unsigned int imageHeight = img.getHeight();
        unsigned int nbChannels = img.getNbChannels();
        int iStart, iStop, jStart, jStop;
        int i,j;

       // parcours des composantes de couleur
    for (int c=0;c<nbChannels;c++){

        // parcours de l'image
        for (int x=0;x<imageWidth;x++){
            for(int y=0;y<imageHeight;y++){
                    //parcours de la fenêtre du filtre

                    x-wSide/2 < 0 ? iStart = 0 : iStart = x-wSide/2;
                    x+wSide/2 > imageWidth-1 ? iStop = imageWidth : iStop = x+wSide/2+1;
                    y-wSide/2 < 0 ? jStart = 0 : jStart = y-wSide/2;
                    y+wSide/2 > imageHeight-1 ? jStop = imageHeight : jStop = y+wSide/2+1;

                    for(i=iStart; i< iStop ; i++){
                        for(j=jStart;j<jStop;j++){
                            //remplissage de la fenêtre du filtre
                            window.push_back(img.getPixelAt(i,j,c));
                        }//j
                    }//i

                size_t size = window.size();
                sort(window.begin(), window.begin()+size);
                outputImage->setPixelAt(x, y, c, window[size/2]);
                window.clear();
            }//y
        }//x
    }//c

        //conversion de la taille de la fenêtre en string pour l'affichage de l'image résultat
        string str;
        ostringstream convert;
        convert << wSide;
        str = convert.str();

        outImage(outputImage, "Image filtered (median"+str+"x"+str+")");
    }
  private:
    Image img;
    int wSide;//largeur de la fenêtre
    vector<int> window;
};

extern "C" Plugin* loadPlugin() {
    Plugin* plugin = new Plugin("Median");
    plugin->addOperation(new MedianCroix());
    plugin->addOperation(new MedianCarre());
    return plugin;
}

extern "C" void unloadPlugin(Plugin* plugin) {
    delete plugin;
}
