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

#include "../Tools.h"
#include "Transforms.h"
#include <GrayscaleImage.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <Converter.h>
#include <iostream>

#include <Utilities/Log.h>
using namespace std;
using namespace imagein;
const double pi = 3.1415926535897932384626433832795;
const double pid2 = 1.57079632679489661923132169163975144209858469968755291;
const double sqrt2 = 1.414213562373095048801688724209698078569671875376948;

Image_t<double>* Transforms::hough(const GrayscaleImage *image ) {   

    double diag = sqrt(image->getWidth()*image->getWidth() + image->getHeight()*image->getHeight());
    
    Image_t<double>* resImg = new Image_t<double>(181, 2*diag + 1, 1, 0.);

    for(unsigned int i = 0; i < image->getHeight(); i++) {
        for(unsigned int j = 0; j < image->getWidth(); j++) {

            if(image->getPixelAt(j, i) == 255) {
                bool firstlign = true;
                for(unsigned int i1 = i; i1 < image->getHeight(); i1++) {
                    for(unsigned int j1 = 0; j1 < image->getWidth(); j1++) {
                        
                        if(firstlign){
                            firstlign = false;
                            j1 = j+1;
                        }

                        if(image->getPixelAt(j1, i1) == 255) {
                            /* Origine en bas à gauche x horizontale y verticale 
                            const double x0 = j;
                            const double y0 = image->getHeight()-i;//-i
                            const double x1 = j1;
                            const double y1 = image->getHeight()-i1;//-i1
                            */
                           /* Origine en haut à gauche x verticale y horizontale */
                            const double x0 = i;
                            const double y0 = j;
                            const double x1 = i1;
                            const double y1 = j1;
                

                            const double l0 = sqrt((double)((y0-y1)*(y0-y1) + (x0-x1)*(x0-x1)));
                            const double l1 = fabs((double)(x0*y1 - x1*y0));
                            double rho = l1/l0; //Rho reel
                            double theta;// theta radian
                            double thetadeg;//theta degre
                            if(x0==x1){
                                theta = 0;

                            }else{
                                theta = atan((y1-y0) / (x1-x0))+ pid2;

                                //cas ou theta entre 0 et -pi/2
                                //Calcul de l'ordonnee a l'origine
                                int ordorig = y0 - x0 * ((y1-y0) / (x1-x0));
                                if(ordorig < 0 && ((y1-y0) / (x1-x0)) > 0) theta = theta - pi;
                            }
                            //Passage de radian a degre
                            thetadeg = theta*(180/pi);


                            //Coordonées du point a colorier dans l'espace de Hough
                            //Espace de Hough :
                            //Origine au centre
                            //Rho vertical vers la droite de -imageDiag a imageDiag
                            //Theta horizontale vers le haut de -90 à 90
                            int rhoStep = 1;
                            int angleStep = 1;
                            if(thetadeg > 90){
                                thetadeg = thetadeg - 180;
                                rho = -rho;
                            }
                            int jDisplayRho = round(( rho + diag )/ rhoStep) ;
                            int iDisplayTheta = round((thetadeg + 90 ) / angleStep);
                            

                            resImg->pixelAt(iDisplayTheta, jDisplayRho, 0)++;
                        }
                    }
                }
            }
        }
    }
    return resImg;

}


Image_t<double>* Transforms::hough2(const Image *image, double angleStep, double rhoStep) {

    double imageDiag = sqrt(image->getWidth()*image->getWidth() + image->getHeight()*image->getHeight());
    Image_t<double>* resImg = new Image_t<double>( 180 / angleStep + 1 , ( imageDiag * 2) / rhoStep + 1, image->getNbChannels(), 0.);


    for(unsigned int c = 0; c < image->getNbChannels(); c++) {

        for(unsigned int i = 0; i < image->getHeight(); i++) // on parcourt l'image
        {
            for(unsigned int j = 0; j < image->getWidth(); j++)
            {
                if(image->getPixelAt(j, i, c) == 255)//getPixelAt demande la colonne j puis la ligne i
                {
                    //Changement de repere : origine en haut  a gauche de l'image, x vertical vers le base et y horizontale vers la droite
                    int x = i;
                    int y = j;
                    //Parcours de tous les angles possibles dans l'image
                    for(double te=-90; te <= 180; te += angleStep) // on parcourt la matrice
                    {
                        const double coste = cos(te * pi / 180.);
                        double sinte = sin(te * pi / 180.);

                        //Calcul de rho pour l'angle courant
                        double rho = x * coste + y * sinte;

                        if(rho >= 0. && rho < imageDiag)
                        {
                            //Coordonées du point a colorier dans l'espace de Hough
                            //Espace de Hough :
                            //Origine au centre
                            //Rho vertical vers la droite de -imageDiag a imageDiag
                            //Theta horizontale vers le haut de -90 à 90
                            double theta = te;
                            if(te > 90){
                                theta = theta - 180;
                                rho = -rho;
                            }
                            int jDisplayRho = round( (rho + imageDiag) / rhoStep) ;
                            int iDisplayTheta = round( (theta + 90) / angleStep);
                            resImg->pixelAt( iDisplayTheta, jDisplayRho, c)++;
                        }
                    }
                }
            }
        }
    }
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


    double angleStep = 180. / (image->getWidth() - 1) ; 
    double imageDiag = sqrt(width*width + height*height);
    double rhoStep = imageDiag * 2 / (image->getHeight() - 1);
    
    //Algorithme de traitement

    for(unsigned int c = 0; c < image->getNbChannels(); ++c) {
        for(unsigned int i = 0; i < image->getHeight(); ++i) {
            for(unsigned int j = 0; j < image->getWidth(); ++j) {

                int n = image->getPixelAt(j, i, c);
                if(n >= threshold)
                {
                    double angle = pi * (j * angleStep - 90) / 180 ; 
                    double rho = rhoStep * i - imageDiag;
                    double sinte = sin(angle);
                    double coste = cos(angle);

                    //Construction de la droite d'equation rho = x*coste + y*sinte
                    for(unsigned int jj = 0; jj < height; ++jj) {
                        double x = jj;
                        double y = (rho - x*coste)/sinte;
                        int ii = round(y);
                        if( ii >= 0 && ii < width) {
                            resImg->pixelAt(ii, jj, c) += n;
                        }

                    }
                    for(unsigned int ii = 0; ii < width; ++ii) {
                        double y = ii;
                        double x = (rho-y*sinte)/coste;
                        int jj = round(x);
                        if( jj>=0 && jj < height) {
                            resImg->pixelAt(ii, jj, c) += n;
                        }
                     }
                }
            }
        }
    }

    //On applique une mise à l'echelle de l'image pour mettre la valeur max a 255
    std::cout << resImg->max() << std::endl;
    Image* resStdImg = new Image(resImg->getWidth(), resImg->getHeight(), resImg->getNbChannels());
    Image_t<uint32_t>::iterator it = resImg->begin();
    Image::iterator jt = resStdImg->begin();
    double max = resImg->max();
    while(it != resImg->end()) {
        *jt = *it * 255. / max + 0.5;
        ++it;
        ++jt;
    }

    *resImgptr = resStdImg;
    delete resImg;
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
    Image_t<double>* tmpImg;
    if(im->getWidth() % 8 != 0 || im->getHeight() % 8!= 0){
        int addcol = 0;
        int addlign = 0;
        if(im->getWidth()  % 8 != 0 )
            addcol = 8 - ( im->getWidth() % 8 );
        if(im->getHeight()  % 8 != 0 )
            addlign = 8 - ( im->getHeight() % 8 );
        
        tmpImg = new Image_t<double>(im->getWidth() + addcol, im->getHeight() + addlign, im->getNbChannels());
        for(unsigned int c = 0; c < tmpImg->getNbChannels(); c++) {
            for(unsigned int i = 0; i < tmpImg->getWidth() ; i++) {
                for(unsigned int j = 0; j < tmpImg->getHeight() ; j++) {
                    
                    tmpImg->setPixel(i, j, c, (double) im->getPixel(min(i, im->getWidth()-1 ), min(j, im->getHeight()-1), c));
                
                }
            }
        }
    }
    else {
       tmpImg = Converter<Image_t<double> >::convert(*im);
    }

/*---------------------------------------------------------------------
*
*     CALCUL DES COEFFICIENTS DE LA MATRICE DE TRANSFORMATION
*
*---------------------------------------------------------------------*/

    char buffer[100];
    returnval = QString(qApp->translate("Operations","\n\nmatrice de transformation utilisee : \n")).toStdString();

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

    for(unsigned int c = 0; c < tmpImg->getNbChannels(); c++) {
      for(unsigned int i=0 ; i < tmpImg->getWidth() ; i += idt) {
          for(unsigned int j=0 ; j < tmpImg->getHeight() ; j += idt) {

              double res[8];
              for(int k = 0 ; k < idt ; ++k) res[k] = 0.;


              for(int k = 0; k < idt; ++k) {

                  for(int l = 0; l < idt; l++) {
                      for(int m=0 ; m<idt ; m++) {
                            res[l] += rmat[l * idt + m] * tmpImg->getPixelAt(min(i+k, tmpImg->getWidth()-1 ), min(j+m, tmpImg->getHeight()-1),  c);
                      }
                  }

                  for(int l=0 ; l<idt ; l++) {
                      if( i+k < tmpImg->getWidth() &&  j+l < tmpImg->getHeight())
                        tmpImg->setPixelAt(i+k, j+l, c, res[l]);
                      res[l] = 0.;
                  }
              }

              for(int k = 0; k < idt; k++) {

                  for(int l=0 ; l<idt ; l++) {
                      for(int m=0 ; m<idt ; m++) {
                            res[l] += rmat[l * idt + m] * tmpImg->getPixelAt(min(i+m, tmpImg->getWidth()-1 ), min(j+k, tmpImg->getHeight()-1), c);
                      }
                  }

                  for(int l=0 ; l<idt ; l++) {
                      if( i+l < tmpImg->getWidth() &&  j+k < tmpImg->getHeight())
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
    for(unsigned int c = 0; c < tmpImg->getNbChannels(); c++) {
      for(unsigned int j = 0; j < tmpImg->getHeight(); j++) {
          for(unsigned int i = 0; i < tmpImg->getWidth(); i++) {
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

for(unsigned int c = 0; c < tmpImg->getNbChannels(); c++) {
  for(unsigned int i=0 ; i< tmpImg->getWidth() ; i+=idt) {
     for(unsigned int j=0 ; j<tmpImg->getHeight(); j+=idt)
     {
        for(int k=0 ; k<idt ; k++)
          res[k] = 0.;

        for(int k=0 ; k<idt ; k++)
        {
          for(int l=0 ; l<idt ; l++)
             for(int m=0 ; m<idt ; m++)
                if( i+m < tmpImg->getWidth() &&  j+k < tmpImg->getHeight())
                    res[l] += rmat[m * idt + l] * tmpImg->getPixelAt(i+m, j+k, c);

          for(int l=0 ; l<idt ; l++)
          { 
            if( i+l < tmpImg->getWidth() &&  j+k < tmpImg->getHeight())
                tmpImg->setPixelAt(i+l, j+k, c, res[l]);
            res[l] = 0.;
          }
        }

        for(int k=0 ; k<idt ; k++)
        {
          for(int l=0 ; l<idt ; l++)
             for(int m=0 ; m<idt ; m++)
                if( i+k < tmpImg->getWidth() &&  j+m < tmpImg->getHeight())
                res[l] += rmat[m * idt + l] * tmpImg->getPixelAt(i+k, j+m, c);

          for(int l=0 ; l<idt ; l++)
          {
            if(i+k < tmpImg->getWidth() &&  j+l < tmpImg->getHeight())
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
