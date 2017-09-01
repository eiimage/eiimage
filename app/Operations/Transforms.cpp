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

#include "Transforms.h"
#include <GrayscaleImage.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <Converter.h>

#include <Utilities/Log.h>
using namespace std;
using namespace imagein;
const double pi = 3.1415926535897932384626433832795;
const double pid2 = 1.57079632679489661923132169163975144209858469968755291;
const double sqrt2 = 1.414213562373095048801688724209698078569671875376948;

Image_t<double>* Transforms::hough(const GrayscaleImage *image ) {   //TODO : inverser i et j i (ligne) et j (colonne)
//    int height = image->getHeight();
//    int width = image->getWidth();
//    const uint8_t* tabim = image->begin();
//    double* itab = new double[ image->size() ];
//    for(unsigned int i = 0; i < image->size(); ++i) itab[i] = 0;
    double diag = sqrt(image->getWidth()*image->getWidth() + image->getHeight()*image->getHeight());
    Image_t<double>* resImg = new Image_t<double>(diag+0.5, 90+180, 1, 0.);

    for(unsigned int i = 0; i < image->getHeight(); i++) {
        for(unsigned int j = 0; j < image->getWidth(); j++) {

            if(image->getPixelAt(j, i) == 255) {

                int i1 = i;

                for(unsigned int j1 = j+1; j1 < image->getWidth(); j1++) {

                    if(image->getPixelAt(j1, i1) == 255) {

                        //passage dans le repere x horieontal, y vertical et origine en bas à gauche
                        const double x0 = j;
                        const double y0 = image->getHeight() -i;
                        const double x1 = j1;
                        const double y1 = image->getHeight()-i1;

                        //calcul de la droite passant par les deux points
                        const double l0 = sqrt((double)((y0-y1)*(y0-y1) + (x0-x1)*(x0-x1)));
                        const double l1 = fabs((double)(x0*y1 - x1*y0));
                        const double rho = l1/l0;
                        double y2;
                        const int j2 = rho + 0.5;
                        double theta;
                        double thetadeg;
                        if(x0==x1){
                            theta = 0;
                            //Log::info("x0==x1");

                        }else{
                            //Log::info("x0!=x1");
                            theta = atan((y1-y0) / (x1-x0))+ pid2;

                            //cas ou theta entre 0 et -pi/2
                            //Calcul de l'ordonnee a l'origine
                            int ordorig = y0 - x0 * ((y1-y0) / (x1-x0));

                            //Log::info("theta = "+std::to_string(theta)+"atan = "+std::to_string(atan((y1-y0) / (x1-x0))+pid2)+ " ; ordorig = "+std::to_string(ordorig));
                            if(ordorig < 0 && ((y1-y0) / (x1-x0)) > 0) theta = theta - pi;

                        }
                        //Passage de radian a degre
                        thetadeg = theta*(180/pi);
                        //Coordonées du point a colorier dans l'espace de Hough
                        //Espace de Hough :
                        //Origine en bas a gauche
                        //Rho horizontal vers la droite de 0 a imageDiag
                        //Theta vertical vers le haut de -90 a 180
                        int rhoStep = 1;
                        int angleStep = 1;
                        int jDisplayRho = round(rho / rhoStep);
                        int iDisplayTheta = round((180-thetadeg) / angleStep);
                        //Log::info("x0 = "+std::to_string(x0)+ "; x1 = "+std::to_string(x1)+" ; y0 = "+std::to_string(y0)+" ; y1 = "+std::to_string(y1)+" ; Rho = "+std::to_string(rho)+" ; Theta = "+std::to_string(thetadeg)+" ; ThetaDisplay = "+std::to_string(iDisplayTheta));



//                            y2 = x1 != x0 ? atan((y1-y0) / (x1-x0))+ pid2 : y1 > y0 ? pi : pid2;//pid2 dans les deux cas (?)
//                            theta = (int)(180-((y2 / pi) * 180. +180 + 0.5)) +1;//conversion en entier + modulo 180
                        //theta = (int) ((y2/pi)*180);




//                            y2 = (x1 != x0) ? atan((y1-y0) / (x1-x0)) + pid2 : y1 > y0 ? pi : pid2;
//                            theta = (int)(180-((y2 / pi) * 180. +180 + 0.5)) +1;
//                            //theta = (int) (y2*(pi/180));
//                            //Coordonées du point a colorier dans l'espace de Hough
//                            //Espace de Hough :
//                            //Origine en bas a gauche
//                            //Rho horizontal vers la droite de 0 a imageDiag
//                            //Theta vertical vers le haut de -90 a 180
//                            int jDisplayRho = round(j2);
//                            int iDisplayTheta = round(theta+180);


                        resImg->pixelAt(jDisplayRho, iDisplayTheta, 0)++;
//                            resImg->setPixelAt(jDisplayRho, iDisplayTheta, resImg->getPixelAt(jDisplayRho, iDisplayTheta) + 1.);

                        //resImg->setPixelAt(j2, theta, resImg->getPixelAt(j2, theta) + 1.);
                    }
                }

                for(unsigned int i1 = i+1; i1 < image->getHeight(); i1++) {
                    for(unsigned int j1 = 0; j1 < image->getWidth(); j1++) {
                        if(image->getPixelAt(j1, i1) == 255) {
                            const double x0 = j;
                            const double y0 = image->getHeight()-i;//-i
                            const double x1 = j1;
                            const double y1 = image->getHeight()-i1;//-i1
                            const double l0 = sqrt((double)((y0-y1)*(y0-y1) + (x0-x1)*(x0-x1)));
                            const double l1 = fabs((double)(x0*y1 - x1*y0));
                            const double rho = l1/l0; //Rho reel
                            double theta;// theta radian
                            double thetadeg;//theta degre
                            if(x0==x1){
                                theta = 0;
                                //Log::info("x0==x1");

                            }else{
                                theta = atan((y1-y0) / (x1-x0))+ pid2;
                                //Log::info("x0!=x1");

                                //cas ou theta entre 0 et -pi/2
                                //Calcul de l'ordonnee a l'origine
                                int ordorig = y0 - x0 * ((y1-y0) / (x1-x0));
                                //Log::info("theta = "+std::to_string(theta)+"atan = "+std::to_string(atan((y1-y0) / (x1-x0))+pid2)+ " ; ordorig = "+std::to_string(ordorig));
                                if(ordorig < 0 && ((y1-y0) / (x1-x0)) > 0) theta = theta - pi;
                            }
                            //Passage de radian a degre
                            thetadeg = theta*(180/pi);

//                            y2 = x1 != x0 ? atan((y1-y0) / (x1-x0))+ pid2 : y1 > y0 ? pi : pid2;//pid2 dans les deux cas (?)
//                            theta = (int)(180-((y2 / pi) * 180. +180 + 0.5)) +1;//conversion en entier + modulo 180
                            //theta = (int) ((y2/pi)*180);


                            //Coordonées du point a colorier dans l'espace de Hough
                            //Espace de Hough :
                            //Origine en bas a gauche
                            //Rho horizontal vers la droite de 0 a imageDiag
                            //Theta vertical vers le haut de -90 a 180
                            int rhoStep = 1;
                            int angleStep = 1;
                            int jDisplayRho = round(rho / rhoStep);
                            int iDisplayTheta = round((180-thetadeg) / angleStep);
                            //Log::info("x0 = "+std::to_string(x0)+ "; x1 = "+std::to_string(x1)+" ; y0 = "+std::to_string(y0)+" ; y1 = "+std::to_string(y1)+" ; Rho = "+std::to_string(rho)+" ; Theta = "+std::to_string(thetadeg)+" ; ThetaDisplay = "+std::to_string(iDisplayTheta));

                            resImg->pixelAt(jDisplayRho, iDisplayTheta, 0)++;
//                          resImg->setPixelAt(jDisplayRho, iDisplayTheta, resImg->getPixelAt(jDisplayRho, iDisplayTheta) + 1.);

                            //resImg->setPixelAt(j2, theta, resImg->getPixelAt(j2, theta) + 1.);
                        }
                    }
                }
            }
        }
    }
//    Image_t<double> *returnval = new Image_t<double>(width, height, 1, itab);
//    delete itab;
//    return returnval;
    return resImg;

}


Image_t<double>* Transforms::hough2(const Image *image, double angleStep, double rhoStep) {

//    double angleStep = .5;
//    double rhoStep = 1.;


//    double imageDiag = image->getWidth() * sqrt(2.);
    double imageDiag = sqrt(image->getWidth()*image->getWidth() + image->getHeight()*image->getHeight());
    Image_t<double>* resImg = new Image_t<double>(1. + imageDiag / rhoStep, (180.+90) / angleStep + 0.5, image->getNbChannels(), 0.);


    for(unsigned int c = 0; c < image->getNbChannels(); ++c) {

        for(unsigned int i = 0; i < image->getHeight(); ++i) // on parcourt l'image
        {
            for(unsigned int j = 0; j < image->getWidth(); ++j)
            {
                if(image->getPixelAt(j, i, c) == 255)//getPixelAt demande la colonne j puis la ligne i
                {
                    //Changement de repere : origine en bas  a gauche de l'image, x horizontal vers la droite et y vertical vers le haut
                    int x = j;
                    int y = image->getHeight()-i;
                    //Parcours de tous les angles possibles dans l'image
                    for(double te=-90; te < 180; te += angleStep) // on parcourt la matrice
                    {
                        const double coste = cos(te * pi / 180.);
                        double sinte = sin(te * pi / 180.);

                        //Calcul de rho pour l'angle courant
                        const double rho = x * coste + y * sinte;

                        if(rho >= 0. && rho < imageDiag)
                        {
                           // resImg->pixelAt(rho / rhoStep + 0.5, (resImg->getHeight()-(te+90)) / angleStep + 0.5, c)++;
                            //Coordonées du point a colorier dans l'espace de Hough
                            //Espace de Hough :
                            //Origine en bas a gauche
                            //Rho horizontal vers la droite de 0 a imageDiag
                            //Theta vertical vers le haut de -90 a 180
                            int jDisplayRho = round(rho / rhoStep);
                            int iDisplayTheta = round((180-te) / angleStep);

                            resImg->pixelAt(jDisplayRho, iDisplayTheta, c)++;
                        }
                    }
                }
            }
        }
    }



//    //Exemple d'affichage de texte dans la fentre "texte"
//    char buffer[455];
//    sprintf( buffer, "Lecture du rsultat :\nLigne du haut : angle=0\nLigne du bas : angle=+180\nColonne de gauche : distance=0\nColonne de droite : distance max (diagonale de l'image)\nPoint de rfrence pour les distances : coin en haut  gauche\nDroite de rfrence pour les angles : axe VERTICAL\nAngles positifs dans le sens trigonomtrique indirect");
//    oh->AddString( buffer );



    return resImg;
}
string Transforms::hough2_inverse(const Image_t<double> *image, Image** resImgptr, unsigned int width, unsigned int height, unsigned int threshold) {
//string Transforms::hough2_inverse(const Image_t<double> *image, Image** resImgptr, unsigned int size, unsigned int threshold) {

    Image_t<uint32_t>* resImg = new Image_t<uint32_t>(width, height, image->getNbChannels(), uint32_t(0));
   // Image_t<uint32_t>* resImg = new Image_t<uint32_t>(size, size, image->getNbChannels(), uint32_t(0));

//    int param = 5000 + 20 * image->getWidth() * image->getHeight();

//    char *buffer;
//    buffer=(char *)calloc(param,sizeof(char));


//    char tampon[50];
//    sprintf( buffer, "Valeur Max de la matrice d'entre=%d",(int)(max+0.1));


    double angleStep = 271. / image->getHeight(); //les angles varient de -90 à +180 = intervalle de longueur 271 degre
    double imageDiag = resImg->getWidth() * sqrt(2.);
    double rhoStep = imageDiag / image->getWidth();

    //Algorithme de traitement

    int cmpt = 0;
    for(unsigned int c = 0; c < image->getNbChannels(); ++c) {
        for(unsigned int i = 0; i < image->getHeight(); ++i) {
            for(unsigned int j = 0; j < image->getWidth(); ++j) {

                int n = image->getPixelAt(j, i, c);
                if(n >= threshold)
                {
                    cmpt++;
                    double angle = (180 - (angleStep * i)) / 180. * pi;
                    double rho = rhoStep * j;
                    double sinte = sin(angle);
                    double coste = cos(angle);

    //                sprintf( tampon,"\nniveau=%d\tangle=%1.0f\tdistance=%1.0f",(int)(0.1+tab_image[i+nl*j]),angle/pi*180.,rho);
    //                strcat( buffer, tampon);

                    //Construction de la droite d'quation rho = x*coste + y*sinte
                    for(unsigned int jj = 0; jj < width; ++jj) {
                        double x = jj;
                        double y = (rho - x*coste)/sinte;
                        int ii = height-round(y);
//                        int kk = rho * (cos(angle) + tan(angle) * sin(angle)) - tan(angle)*jj;
                        if( ii >= 0 && ii < height) {
                            resImg->pixelAt(jj, ii, c) += n;
                        }

                    }
                    for(unsigned int ii = 0; ii < height; ++ii) {
    //                    int kk = ( rho * (cos(angle) + tan(angle) * sin(angle)) -ii ) / tan(angle);
                        double y = height - ii;
                        double x = (rho-y*sinte)/coste;
                        int jj = round(x);
                        if( jj>=0 && jj < width) {
                            resImg->pixelAt(jj, ii, c) += n;
                        }
                     }
                }
            }
        }
    }

//        sprintf( tampon,"\nNombre de droites traces=%d",cmpt);
//        strcat( buffer, tampon);

    //On applique une mise à l'echelle de l'image pour mettre la valeur max a 255
    std::cout << resImg->max() << std::endl;
    Image* resStdImg = new Image(resImg->getWidth(), resImg->getHeight(), resImg->getNbChannels());
    Image_t<uint32_t>::iterator it = resImg->begin();
    Image::iterator jt = resStdImg->begin();
    double max = resImg->max();
    while(it != resImg->end()) {
        // tester avec 255 et sans le +0.5
        *jt = *it * 256. / max + 0.5;
        ++it;
        ++jt;
    }

    *resImgptr = resStdImg;
    return "";
}

/***************************************************************************************************/
string hadamard_haar_88( const Image *im, Image_t<double> **result, Image **result_inverse, double *rmat, GrayscaleImage_t<bool> *selection );

string Transforms::Hadamard( const Image *im, Image_t<double> **result, Image **result_inverse, GrayscaleImage_t<bool> *selection ) {
    if(!( im != NULL && result != NULL && result_inverse != NULL )) {
        char buffer[255];
        sprintf( buffer, "Error in Transforms::Hadamard:\nim = %p, result = %p, result_inverse = %p", im, result, result_inverse );
        throw buffer;
    }
    double rmat[8][8] = {
        { 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000},
        { 1.000, 1.000, 1.000, 1.000,-1.000,-1.000,-1.000,-1.000},
        { 1.000, 1.000,-1.000,-1.000,-1.000,-1.000, 1.000, 1.000},
        { 1.000, 1.000,-1.000,-1.000, 1.000, 1.000,-1.000,-1.000},
        { 1.000,-1.000,-1.000, 1.000, 1.000,-1.000,-1.000, 1.000},
        { 1.000,-1.000,-1.000, 1.000,-1.000, 1.000, 1.000,-1.000},
        { 1.000,-1.000, 1.000,-1.000,-1.000, 1.000,-1.000, 1.000},
        { 1.000,-1.000, 1.000,-1.000, 1.000,-1.000, 1.000,-1.000}
    };
    for(int i=0 ; i<8 ; i++)
    {
        for(int j=0 ; j<8 ; j++)
        {
            rmat[i][j] /= (double)sqrt(8.);
        }
    }
    string returnval = hadamard_haar_88( im, result, result_inverse, (double*)rmat, selection );
    return returnval;
}

string Transforms::Haar( const Image *im, Image_t<double> **result, Image **result_inverse, GrayscaleImage_t<bool> *selection ) {
    if(!( im != NULL && result != NULL && result_inverse != NULL )) {
        char buffer[255];
        sprintf( buffer, "Error in Transforms::Haar:\nim = %p, result = %p, result_inverse = %p", im, result, result_inverse );
        throw buffer;
    }
    // Returns result as the resulting image
    // Returns result_inverse as the inverse of the resulting image
    // Assume that this image is a black-and-white image, again
    double sqrt2 = sqrt(2.);
    double rmat[8][8] = {
        { 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000, 1.000},
        { 1.000, 1.000, 1.000, 1.000,-1.000,-1.000,-1.000,-1.000},
        { sqrt2, sqrt2,-sqrt2,-sqrt2, 0.000, 0.000, 0.000, 0.000},
        { 0.000, 0.000, 00000, 0.000, sqrt2, sqrt2,-sqrt2,-sqrt2},
        { 2.000,-2.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000},
        { 0.000, 0.000, 2.000,-2.000, 0.000, 0.000, 0.000, 0.000},
        { 0.000, 0.000, 0.000, 0.000, 2.000,-2.000, 0.000, 0.000},
        { 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 2.000,-2.000}
    };
    for(int i=0 ; i<8 ; i++)
    {
        for(int j=0 ; j<8 ; j++)
        {
            rmat[i][j] /= (double)sqrt(8.);
        }
    }
    string returnval = hadamard_haar_88( im, result, result_inverse, (double*)rmat, selection );
    return returnval;
}

string Transforms::cosinus( const Image *im, Image_t<double> **result, Image **result_inverse, GrayscaleImage_t<bool> *selection ) {
    if(!( im != NULL && result != NULL && result_inverse != NULL )) {
        char buffer[255];
        sprintf( buffer, "Error in Transforms::Haar:\nim = %p, result = %p, result_inverse = %p", im, result, result_inverse );
        throw buffer;
    }
    // Returns result as the resulting image
    // Returns result_inverse as the inverse of the resulting image
    // Assume that this image is a black-and-white image, again
    double rmat[8][8];
    const double pi = 3.1415926535;
    for(int i=0 ; i<8 ; i++)
    {
        for(int j=0 ; j<8 ; j++)
        {
            rmat[i][j] = (double)((cos((2*j+1)*i*pi/16.))/2.);
        }
    }
    for(int i=0 ; i<8 ; i++)
        rmat[0][i] = (double)(1/sqrt(8.));
    string returnval = hadamard_haar_88( im, result, result_inverse, (double*)rmat, selection );
    return returnval;
}

string hadamard_haar_88( const Image *im, Image_t<double> **resImg, Image **invImg, double *rmat, GrayscaleImage_t<bool> *selection ) {
    if(!( im != NULL && resImg != NULL && invImg != NULL )) {
        char buffer[255];
        sprintf( buffer, "Error in Transforms::hadamard_haar_88:\nim = %p, result = %p, result_inverse = %p", im, resImg, invImg );
        throw buffer;
    }
    // Returns result as the resulting image
    // Returns result_inverse as the inverse of the resulting image
    int idt = 8;
    double res[8];
    string returnval;

/*----------------------------------------------------------------------
*
*     OUVERTURE ET LECTURE DU FICHIER IMAGE ORIGINE
*     ALLOCATION MEMOIRE POUR LES TABLEAUX IMAGES
*
*----------------------------------------------------------------------*/
    Image_t<double>* tmpImg = Converter<Image_t<double> >::convert(*im);

/*---------------------------------------------------------------------
*
*     CALCUL DES COEFFICIENTS DE LA MATRICE DE TRANSFORMATION
*
*---------------------------------------------------------------------*/

    char buffer[100];
    returnval = "\n\nmatrice de transformation utilisee : \n";

    for(int i=0 ; i<idt ; i++)
    {
        for(int j=0 ; j<idt ; j++)
        {
            sprintf( buffer,"%5.2f  ",rmat[i * idt + j]);
            returnval = returnval + buffer;
        }
        returnval = returnval + "\n";
    }

/*----------------------------------------------------------------------
*
*     TRANSFORMATION
*
*----------------------------------------------------------------------*/

    for(unsigned int c = 0; c < tmpImg->getNbChannels(); ++c) {
      for(unsigned int i=0 ; i < tmpImg->getWidth() ; i += idt) {
          for(unsigned int j=0 ; j < tmpImg->getHeight() ; j += idt) {

              double res[8];
              for(int k = 0 ; k < idt ; ++k) res[k] = 0.;


              for(int k = 0; k < idt; ++k) {

                  for(int l = 0; l < idt; ++l) {
                      for(int m=0 ; m<idt ; m++) {
                          res[l] += rmat[l * idt + m] * tmpImg->getPixelAt(i+k, j+m, c);
                      }
                  }

                  for(int l=0 ; l<idt ; l++) {
                      tmpImg->setPixelAt(i+k, j+l, c, res[l]);
                      res[l] = 0.;
                  }
              }

              for(int k = 0; k < idt; ++k) {

                  for(int l=0 ; l<idt ; l++) {
                      for(int m=0 ; m<idt ; m++) {
                          res[l] += rmat[l * idt + m] * tmpImg->getPixelAt(i+m, j+k, c);
                      }
                  }

                  for(int l=0 ; l<idt ; l++) {
                      tmpImg->setPixelAt(i+l, j+k, c, +res[l]);
                      res[l] = 0.;
                  }
              }
          }
      }
    }

/*----------------------------------------------------------------------
*
*     CODAGE
*
*----------------------------------------------------------------------*/

  if(selection != NULL) {
    for(unsigned int c = 0; c < tmpImg->getNbChannels(); ++c) {
      for(unsigned int j = 0; j < tmpImg->getHeight(); ++j) {
          for(unsigned int i = 0; i < tmpImg->getWidth(); ++i) {
              if(!selection->getPixelAt(i % 8, j % 8)) {
                  tmpImg->setPixelAt(i, j, c, 0.);
              }
          }
      }
    }
  }

/*----------------------------------------------------------------------
*
*     STOCKAGE DE L'IMAGE TRANSFORMEE DANS UN FICHIER
*
*----------------------------------------------------------------------*/
  *resImg = new Image_t<double>(*tmpImg);

/*----------------------------------------------------------------------
*
*     TRANSFORMATION INVERSE
*
*----------------------------------------------------------------------*/

for(unsigned int c = 0; c < tmpImg->getNbChannels(); ++c) {
  for(unsigned int i=0 ; i< tmpImg->getHeight() ; i+=idt) {
     for(unsigned int j=0 ; j<tmpImg->getWidth(); j+=idt)
     {
        for(int k=0 ; k<idt ; k++)
          res[k] = 0.;

        for(int k=0 ; k<idt ; k++)
        {
          for(int l=0 ; l<idt ; l++)
             for(int m=0 ; m<idt ; m++)
                res[l] += rmat[m * idt + l] * tmpImg->getPixelAt(i+m, j+k, c);

          for(int l=0 ; l<idt ; l++)
          {
             tmpImg->setPixelAt(i+l, j+k, c, res[l]);
             res[l] = 0.;
          }
        }

        for(int k=0 ; k<idt ; k++)
        {
          for(int l=0 ; l<idt ; l++)
             for(int m=0 ; m<idt ; m++)
                res[l] += rmat[m * idt + l] * tmpImg->getPixelAt(i+k, j+m, c);

          for(int l=0 ; l<idt ; l++)
          {
             tmpImg->setPixelAt(i+k, j+l, c, res[l]);
             res[l] = 0.;
          }
        }
     }
  }
}

/*----------------------------------------------------------------------
*
*     STOCKAGE DE L'IMAGE RESULTAT DANS UN FICHIER
*
*----------------------------------------------------------------------*/
//  Image *tabout = new Image(im->getWidth(), im->getHeight(), tmpImg->getNbChannels());
//  for(unsigned int i=0 ; i<tmpImg->getHeight() ; i++)
//     for(unsigned int j=0 ; j<tmpImg->getWidth() ; j++)
//     {
//        double ftemp = tmpImg->getPixelAt(j, i);
//        if(ftemp < 0)
//          ftemp = 0;
//        if(ftemp > 255)
//          ftemp = 255;
//        tabout->setPixel( j, i, (uint8_t)(ftemp + 0.5) );
//     }

//     *invImg = new Image(tmpImg->getWidth(), tmpImg->getHeight(), tmpImg->getNbChannels());
//    for(unsigned int c = 0; c < tmpImg->getNbChannels(); ++c) {
//        for(unsigned int j = 0; j < tmpImg->getHeight(); ++j) {
//            for(unsigned int i = 0; i < tmpImg->getWidth(); ++i) {
//                double value = tmpImg->getPixelAt(i, j, c);
//                (*invImg)->setPixelAt(i, j, c, value + 0.5);
//            }
//        }
//    }
     *invImg = Converter<Image>::convertAndRound(*tmpImg);


//    delete tabin;
    return returnval;
}

//void Transforms::cosinus( const Image *image, Image_t<double> **resImg, Image **invImg ) {
//    if(!( im != NULL && result != NULL && result_inverse != NULL )) {
//        char buffer[255];
//        sprintf( buffer, "Error in Transforms::cosinus:\nim = %p, result = %p, result_inverse = %p", im, result, result_inverse );
//        throw buffer;
//    }
//    int idt = 8;
//    double res[8];
//    double rmat[8][8];
//    double pi = 3.141592;
//    double *tabin;
//    int i,j,k,l,m;
//    long nbl,nbc,size;
//    double a,max;

///*----------------------------------------------------------------------
//*
//*     RECUPERATION DES PARAMETRES DE LANCEMENT
//*
//*----------------------------------------------------------------------*/

///*----------------------------------------------------------------------
//*
//*     OUVERTURE ET LECTURE DU FICHIER IMAGE ORIGINE
//*     ALLOCATION MEMOIRE POUR LES TABLEAUX IMAGES
//*
//*----------------------------------------------------------------------*/
//    nbc = im->getWidth();
//    nbl = im->getHeight();
//    size = nbc * nbl;
//    GrayscaleImage* tabout = new GrayscaleImage(im->getWidth(), im->getHeight());
//    Image_t<double>* tmpImg = Converter<Image_t<double> >::convert(*image);
////    tabin = new double[ size ];
////    for(i=0 ; i<nbl ; i++)
////        for(j=0 ; j<nbc ; j++)
////            tabin[i*nbc+j] = (double)im->getPixel( j, i );

///*---------------------------------------------------------------------
//*
//*     CALCUL DES COEFFICIENTS DE LA MATRICE DE TRANSFORMATION
//*
//*---------------------------------------------------------------------*/
//    for(int i=0 ; i<idt ; i++)
//    {
//        for(int j=0 ; j<idt ; j++)
//        {
//            rmat[i][j] = (double)((cos((2*j+1)*i*pi/16.))/2.);
//        }
//    }
//    for(int i=0 ; i<idt ; i++)
//        rmat[0][i] = (double)(1/sqrt((double)idt));

///*----------------------------------------------------------------------
//*
//*     TRANSFORMATION
//*
//*----------------------------------------------------------------------*/
//  for(unsigned int i=0 ; i<tmpImg->getHeight() ; i+=idt)
//     for(unsigned int j=0 ; j<tmpImg->getWidth() ; j+=idt)
//     {
//        for(int k=0 ; k<idt ; k++)
//          res[k] = 0.;

//        for(int k=0 ; k<idt ; k++)
//        {
//          for(int l=0 ; l<idt ; l++)
//             for(int m=0 ; m<idt ; m++)
//                res[l] += rmat[l][m]*(*(tabin+(i+k)*nbc+(j+m)));

//          for(l=0 ; l<idt ; l++)
//          {
//             *(tabin+(i+k)*nbc+(j+l)) = res[l];
//             res[l] = 0.;
//          }
//        }

//        for(k=0 ; k<idt ; k++)
//        {
//          for(l=0 ; l<idt ; l++)
//             for(m=0 ; m<idt ; m++)
//                res[l] += rmat[l][m]*(*(tabin+(i+m)*nbc+(j+k)));

//          for(l=0 ; l<idt ; l++)
//          {
//             *(tabin+(i+l)*nbc+(j+k)) = res[l];
//             res[l] = 0.;
//          }
//        }
//     }

///*----------------------------------------------------------------------
//*
//*     CODAGE
//*
//*----------------------------------------------------------------------*/

///*----------------------------------------------------------------------
//*
//*     STOCKAGE DE L'IMAGE TRANSFORMEE DANS UN FICHIER
//*
//*----------------------------------------------------------------------*/
//  max = 0.;
//  for(i=0 ; i<nbl ; i++)
//     for(j=0 ; j<nbc ; j++)
//     {
//        a = (double)log(fabs((double)(*(tabin+i*nbc+j))) + 1.);
//        if(a > max) max = a;
//     }

//  for(i=0 ; i<nbl ; i++)
//     for(j=0 ; j<nbc ; j++)
//     {
//         if( (i%idt)==0 && (j%idt)==0 ) {
//             tabout->setPixel( j, i, 0 );
//         }
//         else {
//             uint8_t p = (uint8_t)(log(fabs((double)(*(tabin+i*nbc+j))) + 1.)*255/max + 0.5);
//             tabout->setPixel( j, i, p );
//         }
//     }
//     *result = tabout;

///*----------------------------------------------------------------------
//*
//*     TRANSFORMATION INVERSE
//*
//*----------------------------------------------------------------------*/
//    tabout = new GrayscaleImage(im->getWidth(), im->getHeight());
//    for(unsigned int hcounter=0; hcounter< tabout->getHeight(); hcounter++ ) {
//      for(unsigned int wcounter=0; wcounter< tabout->getWidth(); wcounter++ ) {
//          tabout->setPixel( wcounter, hcounter, (*result)->getPixel( wcounter, hcounter ) );
//        }
//    }

//  for(i=0 ; i<nbl ; i+=idt)
//     for(j=0 ; j<nbc ; j+=idt)
//     {
//        for(k=0 ; k<idt ; k++)
//          res[k] = 0.;

//        for(k=0 ; k<idt ; k++)
//        {
//          for(l=0 ; l<idt ; l++)
//             for(m=0 ; m<idt ; m++)
//                res[l] += rmat[m][l]*(*(tabin+(i+m)*nbc+(j+k)));

//          for(l=0 ; l<idt ; l++)
//          {
//             *(tabin+(i+l)*nbc+(j+k)) = res[l];
//             res[l] = 0.;
//          }
//        }

//        for(k=0 ; k<idt ; k++)
//        {
//          for(l=0 ; l<idt ; l++)
//             for(m=0 ; m<idt ; m++)
//                res[l] += rmat[m][l]*(*(tabin+(i+k)*nbc+(j+m)));

//          for(l=0 ; l<idt ; l++)
//          {
//             *(tabin+(i+k)*nbc+(j+l)) = res[l];
//             res[l] = 0.;
//          }
//        }
//     }

///*----------------------------------------------------------------------
//*
//*     STOCKAGE DE L'IMAGE RESULTAT DANS UN FICHIER
//*
//*----------------------------------------------------------------------*/
//  for(i=0 ; i<nbl ; i++)
//     for(j=0 ; j<nbc ; j++)
//     {
//        double ftemp = (*(tabin+i*nbc+j));
//        if(ftemp < 0)
//          ftemp = 0;
//        if(ftemp > 255)
//          ftemp = 255;
//        tabout->setPixel( j, i, (uint8_t)ftemp );
//     }

//     *result_inverse = tabout;

//  delete tabin;
//}
