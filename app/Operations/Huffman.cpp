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

#include "Huffman.h"
#include <cstring>
#include <cstdio>
#include <cmath>

using namespace std;
using namespace imagein;

Huffman::Huffman() {
    size = 0;
    nbeff = 0;
    ilon = NULL;
    Pi = NULL;
    codPi = NULL;
    H = 0.0;
    nbbit = 0;
    indicePi = NULL;
    chain = NULL;
}

Huffman::~Huffman() {
    clearMem();
}

void Huffman::clearMem() {
    if( ilon != NULL ) {
        delete ilon;
    }
    if( Pi != NULL ) {
        delete Pi;
    }
    if( codPi != NULL ) {
        delete codPi;
    }
    if( indicePi != NULL ) {
        delete indicePi;
    }
    if( chain != NULL ) {
        delete chain;
    }
}

string Huffman::execute( const GrayscaleImage *im ) {
    int nbpt;
    long wcounter, hcounter;
    char buffer[255];
    std::string returnval;
    Image::depth_t p;
    int i;
    clearMem();
    size = im->getHeight() * im->getWidth();
    nbpt = 0;
    for( hcounter=0; hcounter< im->getHeight(); hcounter++ ) {
        for( wcounter=0; wcounter< im->getWidth(); wcounter++ ) {
            p = im->getPixel( wcounter, hcounter );
            if( p != 0 ) {
                nbpt++;
            }
        }
    }
    returnval = prob_Pi( im ,nbpt);
    sprintf(buffer, "\nH (theo.) = %.4f\n\n",H);
    returnval = returnval + buffer;

    codhuffman();

    for(i=0 ; i<nbeff ; i++)
    {
        sprintf(buffer, "%s",chain+i*nbeff);
        returnval = returnval + buffer;
        sprintf(buffer, "--->%2d bits      Pi[%3d] = %7.5f\n",*(ilon+i),*(indicePi+i),*(Pi+i));
        returnval = returnval + buffer;
    }
    sprintf(buffer, "\n debit(huffman) = %.4f\n",nbbit);
    returnval = returnval + buffer;

    return returnval;
}

string Huffman::prob_Pi(const GrayscaleImage *im, int nbpt )
{
    int ind,p[256],max,min;
    long hcounter;
    long wcounter;
    Image::depth_t pixel;

    string returnval;
    char buffer[50];

/********************************/
/*  calcul de la dynamique de	*/
/*  l'image : min et max 	*/
/*				*/
/********************************/

    min = 255;
    max = 0;

    for( hcounter=0; hcounter< im->getHeight(); hcounter++ ) {
        for( wcounter=0; wcounter< im->getWidth(); wcounter++ ) {
            pixel = im->getPixel( wcounter, hcounter );
            if((pixel < min) && (pixel != 0))
                min = pixel;
            if(pixel > max)
                max = pixel;
        }
    }

    sprintf(buffer, "\n nbpt = %d\n",nbpt);
    returnval = returnval + buffer;
    sprintf(buffer, " min = %d\n",min);
    returnval = returnval + buffer;
    sprintf(buffer, " max = %d\n",max);
    returnval = returnval + buffer;


    Pi = new double[max-min+1];
    indicePi = new short[max-min+1];

    for(ind=0 ; ind <= max-min ; ind++)
    {
        p[ind] = 0;
        *(Pi+ind) = 0.0;
    }

    for( hcounter=0; hcounter< im->getHeight(); hcounter++ ) {
        for( wcounter=0; wcounter< im->getWidth(); wcounter++ ) {
            pixel = im->getPixel( wcounter, hcounter );
            if( pixel != 0 ) {
                ind = pixel;
                p[ind-min]++;
            }
        }
    }


    /**********************************
     *  nbeff : nb effectif
     *  de luminace utile --> Pi != 0
     *  soit : la lum 'i' existe
     *
     **********************************/
    nbeff=0;
    for(ind=0 ; ind <= max-min ; ind++)
    {
        if(p[ind]!=0)
        {
            *(Pi+nbeff)=(double)p[ind]/(double)nbpt;
            H = - (*(Pi+nbeff)) * log(*(Pi+nbeff))/log((double)2.0)+H;
            *(indicePi+nbeff) = (short)(ind+1);
            nbeff++;
        }
    }

    return returnval;
}

void Huffman::codhuffman(void)
{
    int i,j;
    int indic,i0,it;
    double rp1,rp2;
    int i1=0,i2,*icod,iter;

/* codPi tableau intermdiaire de Pi pour le calcul du code de
    Huffman.Les valeurs de codPi sont modifies dans cette partie */

    chain = new char[nbeff*nbeff];
    codPi = new double[nbeff];
    ilon = new int[nbeff];
    icod = new int[nbeff*nbeff];
    //chain=(char *)calloc((unsigned)(nbeff*nbeff),sizeof(char));
  //codPi=(double *)calloc((unsigned)(nbeff),sizeof(double));
  //ilon=(int *)calloc((unsigned)(nbeff),sizeof(int));
  //icod=(int *)calloc((unsigned)(nbeff*nbeff),sizeof(int));

    for(i=0 ; i<nbeff ; i++)
        *(codPi+i) = *(Pi+i);

    for(i=0 ; i<nbeff ; i++)
    {
        *(ilon+i) = 0;
        for(j=0 ; j<nbeff ; j++)
        {
            *(icod+i*nbeff+j)= -1;
            *(chain+i*nbeff+j) = '\0';
        }
    }

    /* reprage des 2 mins de Pi et marquage des indices des Pi traites
       pour ensuite leur affecter un code */

    for(iter=0 ; iter<nbeff-1 ; iter++)
    {
        rp1=1.0;
        rp2=1.0;
        for(i=0 ; i<nbeff ; i++)
        {
            if(*(codPi+i) >= 0.0)
            {
                if(*(codPi+i)<rp1)
                {
                    rp2 = rp1;
                    i2 = i1;
                    rp1 = *(codPi+i);
                    i1 = i;
                }
                else if(*(codPi+i)<rp2)
                {
                    rp2 = *(codPi+i);
                    i2 = i;
                }
            }
        }

        *(icod+iter*nbeff+i2) = 0;
        *(icod+iter*nbeff+i1) = i2+1;
        *(codPi+i2) = *(codPi+i1) + *(codPi+i2);
        *(codPi+i1) = -1.0;
    }

    /* codage effectif (0/1) des != Pi result. sur chain */
    nbbit = 0.0;
    for(i=0 ; i<nbeff ; i++)
    {
        indic = 0;
        i0 = i;
        for(iter=0 ; iter<nbeff-1 ; iter++)
        {
            it = *(icod+iter*nbeff+i0);
            if(it>=0)
            {
                *(ilon+i) = *(ilon+i) +1;
                if(it==0)
                {
                    *(chain+i*nbeff+indic) = '0';
                    indic++;
                }
                else
                {
                    *(chain+i*nbeff+indic) = '1';
                    indic++;
                    i0 = it - 1;
                }
            }
        }
        nbbit = nbbit + (double)(*(ilon+i))*(*(Pi+i));
    }
    delete icod;
}
