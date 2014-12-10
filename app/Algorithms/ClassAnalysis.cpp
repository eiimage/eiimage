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

#include "ClassAnalysis.h"

#include <cstring>
#include <cstdio>
#include <cmath>
#include <cstdlib>

#include "GrayscaleImage.h"

using namespace ClassAnalysis;
using namespace imagein;
using namespace std;

void ClassAnalysis::analyse(int fen, uint8_t *imorig, double *param1, double *param2, int nbl, int nbc ) {

    // Param 1: Will now hold the means
    // Param 2: Will now hold the standard deviations
    if(!( fen < (nbl * 2) && fen < (nbc * 2) )) {
        char buffer[255];
        sprintf( buffer, "Error in ClassAnalysis::analyse:\nfen = %d, nbl = %d, nbc = %d\n", fen, nbl, nbc );
        throw buffer;
    }
//	char nom1[40],nom2[40];
    int fen2,i,j,k,l,nbp;
    long indexy;
    long indexx;
    double moy,sigma;

//	printf("taille de la fenetre locale pour le calcul des parametres :");
//	scanf("%d",&fen);
    if(fen < 3) fen=3;
    if(fen > 15) fen=15;
    fen2 = fen/2;
//	printf("nom de l'image du parametre 1 :");
//	scanf("%s",nom1);
//	printf("nom de l'image du parametre 2 :");
//	scanf("%s",nom2);

    nbp = fen*fen;
    for(i=0 ; i<nbl ; i++)
      for(j=0 ; j<nbc ; j++)
      {
         moy = sigma = 0;

         for(k=-fen2 ; k<fen2+1 ; k++)
            for(l=-fen2 ; l<fen2+1 ; l++)
     {
              indexy = abs((i+k));
                if( indexy >= nbl ) {
                  indexy = 2 * nbl - indexy - 2;
                }
                indexx = abs((j+l));
                if( indexx >= nbc ) {
                  indexx = 2 * nbc - indexx - 2;
                }
                moy = moy + *(imorig+(indexy)*nbc+indexx);
              sigma = sigma+(*(imorig+(indexy)*nbc+indexx)*(*(imorig+(indexy)*nbc+indexx)));
     }
         *(param1+i*nbc+j) = (moy / nbp);
         *(param2+i*nbc+j) = sqrt((double)(sigma/nbp-(moy*moy)/(nbp*nbp)));
      }


    /*
        *learning_result_1 = new ImageFloat;
    (*learning_result_1)->Create( nbc, nbl );
    *learning_result_2 = new ImageFloat;
    (*learning_result_2)->Create( nbc, nbl );
    (*learning_result_1)->setFloatMatrix( param1 );
    (*learning_result_2)->setFloatMatrix( param2 );
        */
    //ecrire_image(p1,nom1,nbc,nbl);
    //ecrire_image(p2,nom2,nbc,nbl);

}

void ClassAnalysis::estimateur(double *param1,double *param2,double moy1[],double var1[],double moy2[],double var2[],int fen,int nbl,int nbc, const vector<Rectangle>& rectangles) {
    long nbp;
    //long fen2;
    double m1,s1,m2,s2;
    int current_rectangle;
    Rectangle rect;


/* estimation des paramtres de chaque classe */
// Each class is a rectangle in the parameter rectangles
    vector<Rectangle>::const_iterator iter;
    current_rectangle = 0;
    for(iter=rectangles.begin();iter!=rectangles.end();++iter) {
        rect = *iter;
        NormalizeRectangle( rect );
        nbp = ((rect.bottom() - rect.top()) + 1) * ((rect.right() - rect.left()) + 1);
        m1 =  m2 = s1 = s2 = 0;
        for(unsigned int i=rect.top() ; i<=rect.bottom() ; i++) {
          for(unsigned int j=rect.left() ; j<=rect.right() ; j++) {
                m1 = m1 + *(param1+i*nbc+j);
                s1 = s1+(*(param1+i*nbc+j)*(*(param1+i*nbc+j)));
                m2 = m2 + *(param2+i*nbc+j);
                s2 = s2+(*(param2+i*nbc+j)*(*(param2+i*nbc+j)));
            }
        }
        moy1[current_rectangle] = (m1 / nbp);
        var1[current_rectangle] = ((double)(s1/nbp-(m1*m1)/(nbp*nbp)));
        if( var1[current_rectangle] > 0 ) {
          var1[current_rectangle] = sqrt( var1[current_rectangle] );
        }
        else {
          var1[current_rectangle] = 0;
        }
        moy2[current_rectangle] = (m2 / nbp);
        var2[current_rectangle] = ((double)(s2/nbp-(m2*m2)/(nbp*nbp)));
        if( var2[current_rectangle] > 0 ) {
          var2[current_rectangle] = sqrt( var2[current_rectangle] );
        }
        else {
          var2[current_rectangle] = 0;
        }
        current_rectangle++;
    }
}

void ClassAnalysis::classif(double *param1,double *param2,double moy1[],double var1[],double moy2[],double var2[],int fen,int nbl,int nbc, Image **classify_result, int num_classes ) {
    //char nom[40];
    int i,j,k,l,numclass;
    double distmin,dist,d1,d2;
    short *resclass;
    double num_class_multiplier = 250.0 / ((double)(num_classes-1));

    //printf("nom de l'image resultat de classification :");
    //scanf("%s",nom);

    resclass = (short *)calloc(nbl*nbc,sizeof(short));

/* classification */
    for(i=0 ; i<nbl ; i++)
    {
      k=i*nbc;
      for(j=0 ; j<nbc ; j++)
      {
         distmin = 10000;
         for(l=0 ; l<num_classes ; l++)
         {
          d1 = (*(param1+k+j) - moy1[l])/var1[l];
            d2 = (*(param2+k+j) - moy2[l])/var2[l];
            dist = sqrt((double)(d1*d1+d2*d2));
            if(dist < distmin)
            {
              distmin = dist;
              numclass = l;
            }
         }
         *(resclass+k+j) = (short)(numclass * num_class_multiplier);
      }
    }

/* sauvegarde image rsultat de classification dans un fichier */
  *classify_result = new GrayscaleImage(nbc, nbl);
//    Pixel pixel;
    int wcounter, hcounter;
    for( hcounter=0; hcounter< nbl; hcounter++ ) {
        for( wcounter=0; wcounter< nbc; wcounter++ ) {
//          pixel.red = resclass[ hcounter * nbc + wcounter  ];
//            pixel.green = pixel.red;
//            pixel.blue = pixel.red;
            (*classify_result)->setPixelAt( wcounter, hcounter, resclass[hcounter * nbc + wcounter] );
        }
    }
//	ecrire_image(resclass,nom,nbc,nbl);

/* libration mmoire alloue */
  free(resclass);
}

void ClassAnalysis::write_to_file( GrayscaleImage *learning, const vector<Rectangle>& rectangles, FILE *f, int learning_fen, Image_t<double> **returnval_mean, Image_t<double> **returnval_stdev ) {
  char buffer[255];
    if(( learning == NULL )) {
        sprintf( buffer, "Error in ClassAnalysis::write_to_file:\nlearning = %p", learning);
        throw buffer;
    }
    if( f == NULL ) {
        throw "Error in ClassAnalysis::write_to_file:\nf = NULL";
    }
    if(!( ( learning_fen & 1 ) == 1 )) {
        sprintf( buffer, "Error in ClassAnalysis::write_to_file:\nlearning_fen = %d", learning_fen );
        throw buffer;
    }
    if( rectangles.empty() ) {
      throw "Error in ClassAnalysis::write_to_file:\nrectangles.empty() = TRUE";
    }

    //Image *learning_result_1;
    //Image *learning_result_2;
    uint8_t *im1;
  double *param1,*param2, *moy1, *var1, *moy2, *var2;
    long nbl,nbc,size,i;

    // entre des paramtres
    nbc = learning->getWidth();
    nbl = learning->getHeight();
    size = nbc * nbl;

    im1 = learning->begin();
    moy1 = new double[ rectangles.size() ];
    var1 = new double[ rectangles.size() ];
    moy2 = new double[ rectangles.size() ];
    var2 = new double[ rectangles.size() ];

    // allocation memoire pour les images des paramtres caractristiques
    param1 = new double[size];
    param2 = new double[size];

// Traitement
    //printf("\n\n- phase d'apprentissage  \n");
    //printf("\n\n- calcul des parametres caracteristiques sur l'image %s\n",nom1);
    analyse(learning_fen, im1,param1,param2,nbl,nbc );

//	printf("\n\n- estimation des parametres caracteristiques  \n");
    estimateur(param1,param2,moy1,var1,moy2,var2,learning_fen,nbl,nbc,rectangles);

    fprintf( f, "MEAN_STDEV\n" );
    fprintf( f, "%d\n", learning_fen );
    fprintf( f, "%lu\n", rectangles.size() );
    vector<Rectangle>::const_iterator iter;
    i = 0;
    for(iter=rectangles.begin();iter!=rectangles.end();++iter) {
      fprintf( f, "%f %f %f %f\n", moy1[i], var1[i], moy2[i], var2[i] );
        i++;
    }

    if( returnval_mean != NULL ) {
        (*returnval_mean) = new Image_t<double>(nbc, nbl, 1, param1);
    }
    if( returnval_stdev != NULL ) {
        (*returnval_stdev) = new Image_t<double>(nbc, nbl, 1, param2);
    }
    /*
    return_string = "valeur des parametres estimes :\n\n";
    for(i=0 ; i<4 ; i++)
    {
      sprintf(buffer, "Classe no %1d \n",i+1);
        return_string = return_string + buffer;
      sprintf(buffer, "parametre 1 ---> moyenne = %5.1f \n",moy1[i]);
      return_string = return_string + buffer;
        sprintf(buffer, "parametre 2 ---> ecart-type = %4.1f\n",var2[i]);
        return_string = return_string + buffer;
    }
*/
    // Erase memory allocated
    delete im1;
    delete moy1;
    delete var1;
    delete moy2;
    delete var2;
    delete param1;
    delete param2;
//	delete learning_result_1;
//	delete learning_result_2;
}

void ClassAnalysis::NormalizeRectangle( Rectangle &rect ) {
  int top, bottom, left, right;
    top = min( rect.top(), rect.bottom() );
    bottom = max( rect.top(), rect.bottom() );
    left = min( rect.left(), rect.right() );
    right = max( rect.left(), rect.right() );
    rect.y = top;
    rect.h = bottom - top;
    rect.x = left;
    rect.w = right - left;
}

Image *ClassAnalysis::classify_from_file( GrayscaleImage *to_classify, FILE *f, int classify_fen, Image_t<double> **returnval_mean, Image_t<double> **returnval_stdev ) {
    if(!( to_classify != NULL )) {
        throw "Error in ClassAnalysis::classify_from_file:\nto_classify = NULL";
    }
    if(!( f != NULL )) {
      throw "Error in ClassAnalysis::classify_from_file:\nf = NULL";
    }
    if(!( ( classify_fen & 1 ) == 1 )) {
        char buffer[255];
        sprintf( buffer, "Error in ClassAnalysis::classify_from_file:\nclassify_fen = %d", classify_fen );
        throw buffer;
    }

    Image *returnval = NULL;
    char buffer[255];
    int num_classes;
    int counter;
    double *param1,*param2, *moy1, *var1, *moy2, *var2;
    long nbl,nbc;
    //long size,i;
    nbl = to_classify->getHeight();
    nbc = to_classify->getWidth();

    fscanf( f, "%s", buffer );
    if( strcmp( buffer, "MEAN_STDEV" ) == 0 ) {
      fscanf( f, "%d", &num_classes ); // Old learning window size
        fscanf( f, "%d", &num_classes );
        if( num_classes <= 0 ) {
          // Invalid number of classes
            return NULL;
        }
        else {
            moy1 = new double[ num_classes ];
            var1 = new double[ num_classes ];
            moy2 = new double[ num_classes ];
            var2 = new double[ num_classes ];
            for( counter=0; counter< num_classes; counter++ ) {
              fscanf( f, "%lf", &moy1[counter] );
                fscanf( f, "%lf", &var1[counter] );
                fscanf( f, "%lf", &moy2[counter] );
                fscanf( f, "%lf", &var2[counter] );
            }
        }
    }
    else {
      // Invalid file type
        return NULL;
    }

    param1 = new double[nbl * nbc ];
    param2 = new double[nbl * nbc ];
    uint8_t *im1 = to_classify->begin();

    analyse(classify_fen, im1,param1,param2,nbl,nbc );
    classif(param1,param2,moy1,var1,moy2,var2,classify_fen,nbl,nbc, &returnval, num_classes );

    if( returnval_mean != NULL ) {
        (*returnval_mean) = new Image_t<double>(nbc, nbl, 1, param1);
    }
    if( returnval_stdev != NULL ) {
        (*returnval_stdev) = new Image_t<double>(nbc, nbl, 1, param2);
    }

    delete im1;
    delete param1;
    delete param2;
    delete moy1;
    delete moy2;
    delete var1;
    delete var2;
    return returnval;
}

string ClassAnalysis::print_file_info( FILE *f ) {
  if(!( f != NULL )) {
        throw "Error in ClassAnalysis::print_file_info:\nf = NULL";
    }
    string returnval;

    int wsize;
    int num_classes;
    char buffer[255];
    double moy1, moy2, var1, var2;
    int counter;
    fscanf( f, "%s", buffer );
    if( strcmp( buffer, "MEAN_STDEV" ) == 0 ) {
      fscanf( f, "%d", &wsize ); // Old learning window size
        sprintf( buffer, "Learning Window Size: %d\n", wsize );
        returnval = returnval + buffer;
        fscanf( f, "%d", &num_classes );
        sprintf( buffer, "Number of Classes: %d\n", num_classes );
        returnval = returnval + buffer;

        if( num_classes <= 0 ) {
          // Invalid number of classes
            returnval = "\n      Invalid number of classes";
        }
        else {
            for( counter=0; counter< num_classes; counter++ ) {
              fscanf( f, "%lf", &moy1 );
                fscanf( f, "%lf", &var1 );
                fscanf( f, "%lf", &moy2 );
                fscanf( f, "%lf", &var2 );
              sprintf( buffer, "---- Class %d:\n", (counter+1) );
                returnval = returnval + buffer;
                sprintf( buffer, "     Mean of means: %f\n", moy1 );
                returnval = returnval + buffer;
                //sprintf( buffer, "     Stdev of means: %f\n", var1 );
                //returnval = returnval + buffer;
                sprintf( buffer, "     Mean of stdevs: %f\n", moy2 );
                returnval = returnval + buffer;
                //sprintf( buffer, "     Stdev of stdevs: %f\n", var2 );
                //returnval = returnval + buffer;
                returnval = returnval + "\n";
            }
        }
    }
    else {
      returnval = "Not a valid classification file\n";
    }

    return returnval;
}
