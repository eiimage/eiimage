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

#include "MICD.h"
#include <cstring>
#include <cstdio>

using namespace std;
using namespace imagein;

MICD::MICD()
{
    long counter;
    for( counter=0; counter< 128; counter++ ) {
      iloiqu[counter] = 0;
    }
    for( counter=0; counter< 2048*20; counter++ ) {
      ((int*)itcod)[counter] = 0;
        ((int*)itrco)[counter] = 0;
    }
    for( counter=0; counter< 40; counter++ ) {
      ((int*)ktab)[counter] = 0;
    }
    quantdef = NULL;
}

MICD::~MICD()
{

}

string MICD::execute( const GrayscaleImage *im, Prediction prediction_alg, imagein::ImageDouble **err_image, Image **recons_image, double Q ) {
    char buffer[255];
    if( quantdef == NULL ) {
        throw "Error in MICD::execute:\nquantdef = NULL";
    }
    string returnval;
    int nbl,nbc,size,rep,i,j,pred,ier,ireco,icode;
    float pi[512],nbpt = 0;
    double h = 0.;
    rep = prediction_alg;

    nbl = im->getHeight();
    nbc = im->getWidth();
    size = nbc * nbl;

    int po;
    int temp_p;
    /* initialisation de la loi de quantification */
    set_levels();
    codlq(0);
    /* allocation mmoire pour l'image d'erreur de prdiction */
    ImageDouble *error_prediction_image = new ImageDouble(nbc, nbl, 1);
    Image *reconstructed_image = new GrayscaleImage(*im);
    // File these images with all values of zero
    long wcounter, hcounter;
    for( hcounter=0; hcounter< nbl; hcounter++ ) {
        for( wcounter=0; wcounter< nbc; wcounter++ ) {
            error_prediction_image->setPixelAt(wcounter, hcounter, 0);
        }
    }

    /* mise  0 du tableau des probas servant pour le calcul de
    l'entropie de l'erreur de prdiction */
    for(i=0 ; i<512 ; i++) pi[i]= 0.;

    /* codage de l'image */
    for(i=1; i<nbl ; i++)
    {
         for(j=1; j<nbc ; j++)
         {
             if(rep == PX_EQ_A) {
                 temp_p = reconstructed_image->getPixelAt( j - 1, i );
                 pred = temp_p;
             }
             else if(rep == PX_EQ_B) {
                 temp_p = reconstructed_image->getPixelAt( j, i - 1);
                 pred = temp_p;
             }
             else if(rep == PX_EQ_APC) {
                 temp_p = reconstructed_image->getPixelAt( j - 1, i );
                 pred = (int)temp_p;
                 temp_p = reconstructed_image->getPixelAt( j, i - 1 );
                 pred = ( pred + (int)temp_p ) / 2;
         }
         else if(rep == PX_EQ_Q) {
                /*
             Modified Graham's Algorithm:
             if |B-C| - Q <= |B-A| <= |B-C| + Q
                        P(X) = (A+C)/2
             else
                    if |B-A| > |B-C|
                            P(X) = A
                    else
                            P(X) = C
             */
             float A = im->getPixelAt( j - 1, i );
             float B = im->getPixelAt( j - 1, i - 1 );
             float C = im->getPixelAt( j, i - 1 );
             if( ((fabs(B-C) - Q) <= fabs(B-A)) &&
                 (fabs(B-A) <= (fabs(B-C) + Q)) ) {
                 pred = (uint8_t)((A + C) / 2);
             }
             else {
                 if( fabs(B-A) > fabs(B-C) ) {
                   pred = (uint8_t)A;
                 }
                 else {
                   pred = (uint8_t)C;
                 }
             }
         }

         temp_p = reconstructed_image->getPixelAt( j, i );
         ier = (int)temp_p - pred;
         codec(0,ier,&icode,&ireco);
         pi[ier+255]++;      /* proba associe  l'erreur de prdiction */
         nbpt++;

         int tempvalue;
         tempvalue = ier + 128;
         po = tempvalue;
         error_prediction_image->setPixelAt( j, i, po );
         tempvalue = pred + ireco;
         if( tempvalue > 255 ) tempvalue = 255;
         if( tempvalue < 0 ) tempvalue = 0;
         po = tempvalue;
         reconstructed_image->setPixelAt( j, i, po );
     }
  }

    /* calcul de l'entropie de l'image d'erreur de prdiction */
    for(i=0 ; i<512 ; i++)
    {
        if(pi[i] != 0) {
            pi[i] /= nbpt;
            h -= (double)pi[i] * log((double)pi[i])/log((double)2.0);
        }
    }

    /* affichage des rsultats */
    sprintf(buffer, "\nL'entropie de l'image d'erreur de prediction vaut : %lf\n",h);
    returnval = returnval + buffer;
    returnval = returnval + "\n";
    returnval = returnval + print_iloiqu();


    /* libration de la mmoire alloue */
    *err_image = error_prediction_image;
    *recons_image = reconstructed_image;
    return returnval;
}

void MICD::codlq(int m) {
   int n,nar,nk,i,j;

    n=iloiqu[0];
    ktab[0][m]=iloiqu[1];
    nar=ktab[0][m] - 1;
    j= -1;
    for(i=0;i < n-1 ; i++)
    {
      nk=1+nar-iloiqu[2*i+1];
      do{j++;itcod[j][m]=i;nk++;} while(nk <= 0);
      nar=iloiqu[2*i+1];
      itrco[i][m]=iloiqu[2*i+2];
    }
    itcod[j+1][m]=i;
    itrco[i][m]=iloiqu[2*i+2];
    ktab[1][m]=iloiqu[2*i+1];
}

void MICD::codec(int nlq,int ier,int *icode,int *ireco) {
   int m,ip,iep,ierp,n,l;
    m=nlq;
    ip=ktab[0][m];
    ierp=ier-ip;
    if(ierp > 0)
    {
      ip=ktab[1][m];
      iep=ier-ip;
      if(iep < 0)
        n=ier - ktab[0][m];
      else
        n=ktab[1][m] - ktab[0][m];
      *icode=itcod[n][m];
      l= *icode;
      *ireco=itrco[l][m];
    }
    else
    {
      *icode=itcod[0][m];
      *ireco=itrco[0][m];
    }
}

void MICD::set_levels() {
  // Fills in iloiqu with the specified values
    if( quantdef->size - 1 > 32 || quantdef->size - 1 < 1 ) {
        char buffer[255];
        sprintf( buffer, "Error in MICD::set_levels:\nquantdef->GetNumThresholds() = %d", quantdef->size - 1 );
        throw buffer;
    }
    int counter;
    iloiqu[0] = quantdef->size;
    for( counter=0; counter< quantdef->size - 1; counter++ ) {
        iloiqu[ counter * 2 + 1 ] = quantdef->threshold[ counter ];
        iloiqu[ counter * 2 + 2 ] = quantdef->values[ counter ];
    }
    iloiqu[ (quantdef->size - 1) * 2 + 1 ] = iloiqu[ (quantdef->size - 1) * 2 - 1 ] + 1;
    iloiqu[ (quantdef->size - 1) * 2 + 2 ] = quantdef->values[ quantdef->size - 1 ];
}

string MICD::print_iloiqu() {
  string returnval;
    returnval = "seuils de dcision --------------- niveaux de reconstruction\n";
  int counter;
    char buffer[100];
    for( counter=1; counter<= iloiqu[0]*2-1; counter++ ) {
        if( !(counter & 1 == 1) ) {
          sprintf( buffer, "                                                 %3d     \n", iloiqu[counter] );
            returnval = returnval + buffer;
            sprintf( buffer, "      %3d ---------------------------------------------\n", iloiqu[counter-1] );
            returnval = returnval + buffer;
        }
    }
    sprintf( buffer, "                                                 %3d     \n", iloiqu[counter] );
    returnval = returnval + buffer;

    return returnval;
}

void MICD::setQuantification( Quantification *tquantdef ) {
    if( tquantdef == NULL ) {
        throw "Error in MICD::setQuantDef:\ntquantdef = NULL";
    }
    if( tquantdef->size - 1 > 32 || tquantdef->size - 1 < 1 ) {
        char buffer[255];
        sprintf( buffer, "Error in MICD::setQuantDef:\ntquantdef->GetNumThresholds() = %d", tquantdef->size - 1 );
        throw buffer;
    }
    quantdef = tquantdef;
}
