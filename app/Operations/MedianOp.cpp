#include "MedianOp.h"
#include "MedianDialog.h"
#include <QApplication>
#include <vector>
#include <cstring>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <algorithm>


MedianOp::MedianOp(): Operation(qApp->translate("Operations", "Median Filter").toStdString())
{

}
bool MedianOp::needCurrentImg() const {
    return true;
}

void MedianOp::operator()(const imagein::Image* image, const std::map<const imagein::Image*, std::string>&) {

    using namespace std;
    using namespace imagein;
    Image* resImg = new Image(image->getWidth(), image->getHeight(), image->getNbChannels());
    int wSide; // taille de la fenetre du filtre
    vector<int> window; // fenetre du filtre
    unsigned int imageWidth = image->getWidth();
    unsigned int imageHeight = image->getHeight();
    unsigned int nbChannels = image->getNbChannels();
    int iStart, iStop, jStart, jStop;
    int i,j;



    MedianDialog* dialog = new MedianDialog(QApplication::activeWindow());
    dialog->setWindowTitle(QString(qApp->translate("Operations", "Median Filter")));

    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());

    if(code!=QDialog::Accepted) return;
    if(dialog->square()){
        wSide = dialog->getSize();

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
                             window.push_back(image->getPixelAt(i,j,c));
                         }//j
                     }//i

                 size_t size = window.size();
                 sort(window.begin(), window.begin()+size);
                 resImg->setPixelAt(x, y, c, window[size/2]);
                 window.clear();
             }//y
         }//x
     }//c
    }// if square
    if(dialog->cross()){
        wSide = dialog->getSize();

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
                                 if (i==x || j==y)window.push_back(image->getPixelAt(i,j,c));

                             }//j
                         }//i

                     size_t size = window.size();
                     sort(window.begin(), window.begin()+size);
                     resImg->setPixelAt(x, y, c, window[size/2]);
                     window.clear();
                 }//y
             }//x
         }//c
    }// if cross

    //conversion de la taille de la fenêtre en string pour l'affichage de l'image résultat
    string str;
    ostringstream convert;
    convert << wSide;
    str = convert.str();
    outImage(resImg, "Filtered (median"+str+"x"+str+")");
}
