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

#include <cstdio>
#include "DCT.h"
#include "../Tools.h"
#include <Converter.h>

using namespace std;
using namespace imagein;



void cosi(double* coef);
void inver(double* tab, double* coef);
void trans(double* tab, double* coef);
void idct(Image_t<double>* img, double* coefs);
void dct(Image_t<double>* img, double* coefs);
string reduce(Image_t<double>* img, int nBitInit, double slope);
string tronc(Image_t<double>*img, int limit);

std::string dct16x16(const imagein::Image *img, imagein::Image_t<double> **resImg, imagein::Image **invImg, bool truncMode, int truncLimit, int nBitInit, double slope)
{
    string returnval;
/*----------------------------------------------------------------------
*
*     OUVERTURE ET LECTURE DU FICHIER IMAGE ORIGINE
*
*----------------------------------------------------------------------*/
//    tab1 = im;
    Image_t<double>* tmpImg = Converter<Image_t<double> >::convert(*img);
//    tabim= new float[size];
//    choix = v->choice;
    //printf("\n\nMode de codage  ? :\n");
  //printf("   - par troncature des coefficients  (rep=1)\n");
  //printf("   - par matrice d'allocation de bits (rep=2)\n");
  //printf("Votre choix ? :");
  //scanf("%d",&choix);

  //PIXEL *p = tab1->get_image_data();

//    for(i=0 ; i<nbl ; i++) {
//        for(j=0 ; j<nbc ; j++) {
//          pixel = tab1->GetPixel( j, i );
//          *(tabim+i*nbc+j) = (float)pixel.red;
//        }
//    }

/*----------------------------------------------------------------------
*
*     CALCUL DES COEFFICIENTS DE LA MATRICE DE TRANSFORMATION
*
*----------------------------------------------------------------------*/
    double coef[16];
    cosi(coef);

/*----------------------------------------------------------------------
*
*     TRANSFORMATION
*
*----------------------------------------------------------------------*/
    dct(tmpImg, coef);

/*----------------------------------------------------------------------
*
*     CODAGE
*
*----------------------------------------------------------------------*/
//  printf("\ncodage ...\n");
    if(truncMode) {
        returnval = tronc(tmpImg, truncLimit);
    }
    else {
        returnval = reduce(tmpImg, nBitInit, slope);
    }

/*----------------------------------------------------------------------
*
*     STOCKAGE DE L'IMAGE TRANSFORMEE DANS UN FICHIER
*
*----------------------------------------------------------------------*/
//  max = 0.;
//  for(i=0 ; i<nbl ; i++)
//     for(j=0 ; j<nbc ; j++)
//    {
//        a = (float)log(fabs((double)(*(tabim+i*nbc+j))) + 1.);
//      if(a > max) max = a;
//    }

//    //q = (*result)->get_image_data();
//    for(i=0 ; i<nbl ; i++)
//     for(j=0 ; j<nbc ; j++)
//    {
//         if( (i%idt)==0 && (j%idt)==0 ) {
//           pixel.red = (byte)max;
//             pixel.green = (byte)max;
//             pixel.blue = (byte)max;
//             pixel.alpha = 0;
//             (*result)->SetPixel( j, i, pixel );
//         }
//         else {
//           pixel.red = (byte)(log(fabs((double)(*(tabim+i*nbc+j))) + 1.)*255/max + 0.5);
//       pixel.green = pixel.red;
//             pixel.blue = pixel.red;
//             pixel.alpha = 0;
//             (*result)->SetPixel( j, i, pixel );
//         }
//     }
    *resImg = new Image_t<double>(*tmpImg);

/*----------------------------------------------------------------------
*
*     TRANSFORMATION INVERSE
*
*----------------------------------------------------------------------*/
    idct(tmpImg, coef);

/*----------------------------------------------------------------------
*
*     STOCKAGE DE L'IMAGE RESULTAT DANS UN FICHIER
*
*----------------------------------------------------------------------*/

//    for(i=0 ; i<nbl ; i++)
//      for(j=0 ; j<nbc ; j++)
//      {
//         value = (short)(*(tabim+i*nbc+j) + 0.5);
//         if (value > 255) value = 255;
//         if (value < 0) value = 0;
//         pixel.red = (byte)value;
//         pixel.green = (byte)value;
//         pixel.blue = (byte)value;
//         pixel.alpha = 0;
//         (*result_inverse)->SetPixel( j, i, pixel );
//      }

    *invImg = Converter<Image>::convertAndRound(*tmpImg);

    return returnval;
}

/*--------------------------------------------------------------------
*
*  SOUS-PROGRAMME DE SUPRESSION DES COEFFICIENTS DE HAUTE FREQUENCE
*
*--------------------------------------------------------------------*/
string tronc(Image_t<double>*img, int limit)
{
    double debit = 0.;
    char buffer[255];

    for(unsigned int c = 0; c < img->getNbChannels(); ++c) {
        for(unsigned int i = 0; i < img->getHeight(); i +=16) {
            for(unsigned int j = 0; j < img->getWidth(); j +=16) {

                for(int k = 0; k <16; ++k) {
                    for(int l = 0; l <16; ++l) {

                        if(k > limit || l > limit) {
                            img->setPixelAt(j+l, i+k, c, 0.);
                        }
                    }
                }
            }
        }
    }

    /* calcul du debit */
    for(int k = 0; k <16; ++k) {
        for(int l = 0; l <16; ++l) {
            if(k <= limit && l <= limit) ++debit;
        }
    }
    debit = debit * 8. / 256.;
    sprintf(buffer, QString(qApp->translate("Operations", "\nLe debit vaut : %5.2f\n\n")).toUtf8(), debit);
    return buffer;
}

/*---------------------------------------------------------------------
*
*  SOUS-PROGRAMME DE QUANTIFICATION ET CODAGE DES COEFFICIENTS
*
*---------------------------------------------------------------------*/
string reduce(Image_t<double>* img, int nBitInit, double slope)
{
   int matrice[16][16];

    int n0 = nBitInit;
    double a = slope;
    double debit = 0;

    char buffer[100];
    string cs = QString(qApp->translate("Operations","\n---------Matrice d'allocation de bits---------\n\n")).toStdString();

    for(unsigned int i = 0; i <16; ++i) {
        for(unsigned int j = 0; j <16; ++j) {
            int m;
            if(i==0 && j==0) {
                m = 8;
            }
            else
            {
                m = n0 - (int)(fabs( a * (i+j) ) + 0.5);
                if(m > 8) m = 8;
                if(m < 0) m = 0;
            }
            matrice[i][j] = m;
            debit += m;
            sprintf( buffer, "%1d  ",matrice[i][j]);
            cs = cs + buffer;
        }
        cs = cs + "\n";
    }
    debit /= (16 * 16);
    sprintf(buffer, QString(qApp->translate("Operations","\nLe debit vaut : %5.2f\n\n")).toUtf8(),debit);
    cs = cs + buffer;


    for(unsigned int c = 0; c < img->getNbChannels(); ++c) {
        for(unsigned int i = 0; i < img->getHeight(); i +=16) {
            for(unsigned int j = 0; j < img->getWidth(); j +=16) {

                img->setPixelAt(j, i, c, (int)(img->getPixelAt(j, i, c) + 0.5));

                for(int k = 1; k <= 2*(16 - 1); ++k) {
                    for(int l = max(0, k -16+1); l <= min(k,16-1); l++) {
                        int kx, ky;
                        if( k%2 == 0)
                        {
                           kx = l;
                           ky = k-l;
                        }
                        else
                        {
                           kx = k-l;
                           ky = l;
                        }

                        int nb = matrice[ky][kx] - 1;
                        if(nb >= 0)
                        {
                            double cmax = pow(2., nb) - 1.;
                            double co = img->getPixelAt(j + kx, i + ky, c);
                            double ca = fabs(co);
                            double cn = min(ca, cmax);
                            int cm = cn + 0.5;
                            img->setPixelAt(j+kx, i+ky, c, (co > 0) ? cm : -cm);
                        }
                        else {
                            img->setPixelAt(j+kx, i+ky, c, 0.);
                        }


            }
                }
            }
        }
    }
    return cs;
}

/*---------------------------------------------------------------------
*
*  SOUS-PROGRAMME DE TRANSFORMATION DCT SUR TOUTE UNE IMAGE
*
*---------------------------------------------------------------------*/
void dct(Image_t<double>* img, double* coefs)
{
    double coef = 2. / (16*16);
    double coe0 = sqrt(0.5);

    for(unsigned int c = 0; c < img->getNbChannels(); ++c) {
        for(unsigned int i = 0; i < img->getHeight(); i += 16) {
            for(unsigned int j = 0; j < img->getWidth(); j += 16) {

                for(int k = 0; k < 16; ++k) {

                    double tab[16];
                    for(int l = 0; l < 16; ++l) {
                        tab[l] = img->getPixelAt(j+l, i+k, c);
                    }

                    trans(tab, coefs);
                    tab[0] = tab[0] * coe0;

                    for(int l = 0; l <16; ++l) {
                        img->setPixelAt(j+l, i+k, c, tab[l]);
                    }
                }

                for(int l = 0; l < 16; ++l)
                {
                    double tab[16];
                    for(int k = 0; k < 16; ++k) {
                        tab[k] = img->getPixelAt(j+l, i+k, c);
                    }

                    trans(tab, coefs);
                    tab[0] = tab[0] * coe0;

                    for(int k=0; k < 16; ++k) {
                        img->setPixelAt(j+l, i+k, c, tab[k]*coef);
                    }
                }
            }
        }
    }
}

/*---------------------------------------------------------------------
*
*
* SOUS-PROGRAMME DE TRANSFORMATION DCT INVERSE SUR TOUTE UNE IMAGE
*
*---------------------------------------------------------------------*/
void idct(Image_t<double>* img, double* coefs)
{
    double coe0 = sqrt(0.5);
    double coef = 2.;

    for(unsigned int c = 0; c < img->getNbChannels(); ++c) {
        for(unsigned int i = 0 ; i < img->getHeight() ; i += 16) {
            for(unsigned int j = 0 ; j < img->getWidth() ; j += 16) {

                for(int k = 0 ; k < 16 ; ++k) {

                    double tab[16];
                    for(int l = 0; l < 16; ++l) {
                        tab[l] = img->getPixelAt(j+k, i+l, c) * coef;
                    }

                    tab[0] = tab[0] * coe0;
                    inver(tab, coefs);

                    for(int l = 0; l < 16; ++l) {
                        img->setPixelAt(j+k, i+l, c, tab[l]);
                    }
                }

                for(int l = 0; l < 16; ++l) {

                    double tab[16];
                    for(int k = 0; k < 16; ++k) {
                        tab[k] = img->getPixelAt(j+k, i+l, c);
                    }

                    tab[0] = tab[0] * coe0;
                    inver(tab, coefs);

                    for(int k = 0; k < 16; ++k) {
                        img->setPixelAt(j+k, i+l, c, tab[k]);
                    }
                }
            }
        }
    }
}

/*--------------------------------------------------------------------
*
*  SOUS-PROGRAMME DE TRANSFORMATION DCT D'UN VECTEUR A 16 ELEMENTS
*  SELON L'ALGORITME RAPIDE DE B.G.LEE.
*
*--------------------------------------------------------------------*/
void trans(double* tab, double* coef)
{
    double x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15;
    double y0,y1,y2,y3,y4,y5,y6,y7,y8,y9,y10,y11,y12,y13,y14,y15;

    x0=tab[0];
    x1=tab[1];
    x2=tab[2];
    x3=tab[3];
    x4=tab[4];
    x5=tab[5];
    x6=tab[6];
    x7=tab[7];
    x8=tab[8];
    x9=tab[9];
    x10=tab[10];
    x11=tab[11];
    x12=tab[12];
    x13=tab[13];
    x14=tab[14];
    x15=tab[15];
    y0=x0+x15;
    y15=(x0-x15)*coef[1];
    y1=x1+x14;
    y14=(x1-x14)*coef[3];
    y2=x2+x13 ;
    y13=(x2-x13)*coef[5];
    y3=x3+x12;
    y12=(x3-x12)*coef[7];
    y4=x4+x11;
    y11=(x4-x11)*coef[9];
    y5=x5+x10;
    y10=(x5-x10)*coef[11];
    y6=x6+x9;
    y9=(x6-x9)*coef[13];
    y7=x7+x8;
    y8=(x7-x8)*coef[15];
    x0=y0+y7;
    x7=(y0-y7)*coef[2];
    x1=y1+y6;
    x6=(y1-y6)*coef[6];
    x2=y2+y5;
    x5=(y2-y5)*coef[10];
    x3=y3+y4;
    x4=(y3-y4)*coef[14];
    x15=y15+y8;
    x8=(y15-y8)*coef[2];
    x14=y14+y9;
    x9=(y14-y9)*coef[6];
    x13=y13+y10 ;
    x10=(y13-y10)*coef[10];
    x12=y12+y11;
    x11=(y12-y11)*coef[14];
    y0=x0+x3;
    y3=(x0-x3)*coef[4];
    y1=x1+x2;
    y2=(x1-x2)*coef[12];
    y7=x7+x4;
    y4=(x7-x4)*coef[4];
    y6=x6+x5;
    y5=(x6-x5)*coef[12];
    y15=x15+x12;
    y12=(x15-x12)*coef[4];
    y14=x14+x13;
    y13=(x14-x13)*coef[12];
    y8=x8+x11;
    y11=(x8-x11)*coef[4];
    y9=x9+x10;
    y10=(x9-x10)*coef[12];
    x0=y0+y1;
    x1=(y0-y1)*coef[8];
    x3=y3+y2;
    x2=(y3-y2)*coef[8];
    x7=y7+y6;
    x6=(y7-y6)*coef[8];
    x4=y4+y5;
    x5=(y4-y5)*coef[8];
    x15=y15+y14;
    x14=(y15-y14)*coef[8];
    x12=y12+y13;
    x13=(y12-y13)*coef[8];
    x8=y8+y9;
    x9=(y8-y9)*coef[8];
    x11=y11+y10;
    x10=(y11-y10)*coef[8];
    x3=x3+x2;
    x4=x4+x5;
    x12=x12+x13 ;
    x11=x11+x10;
    x7=x7+x4;
    x4=x4+x6;
    x6=x6+x5;
    x8=x8+x11;
    x11=x11+x9;
    x9=x9+x10;
    x15=x15+x8;
    x8=x8+x12;
    x12=x12+x11;
    x11=x11+x14;
    x14=x14+x9;
    x9=x9+x13;
    x13=x13+x10;
    tab[0]=x0;
    tab[1]=x15;
    tab[2]=x7;
    tab[3]=x8;
    tab[4]=x3;
    tab[5]=x12;
    tab[6]=x4;
    tab[7]=x11;
    tab[8]=x1;
    tab[9]=x14;
    tab[10]=x6;
    tab[11]=x9;
    tab[12]=x2;
    tab[13]=x13;
    tab[14]=x5;
    tab[15]=x10;
}

/*--------------------------------------------------------------------
*
*  SOUS-PROGRAMME DE TRANSFORMATION DCT INVERSE D'UN VECTEUR A 16 ELE-
*  MENTS SELON L'ALGORITME RAPIDE DE B.G.LEE.
*
*--------------------------------------------------------------------*/
void inver(double* tab, double* coef)
{
    double x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15;
    double y0,y1,y2,y3,y4,y5,y6,y7,y8,y9,y10,y11,y12,y13,y14,y15;

    x0=tab[0];
    x1=tab[1];
    x2=tab[2];
    x3=tab[3];
    x4=tab[4];
    x5=tab[5];
    x6=tab[6];
    x7=tab[7];
    x8=tab[8];
    x9=tab[9];
    x10=tab[10];
    x11=tab[11];
    x12=tab[12];
    x13=tab[13];
    x14=tab[14];
    x15=tab[15];
    x15=x15+x13;
    x13=x13+x11;
    x11=x11+x9;
    x9=x9+x7;
    x7=x7+x5;
    x5=x5+x3;
    x3=x3+x1;
    x14=x14+x10;
    x10=x10+x6;
    x6=x6+x2;
    x15=x15+x11;
    x11=x11+x7;
    x7=x7+x3;
    x8=x8*coef[8];
    x10=x10*coef[8];
    x9=x9*coef[8];
    x11=x11*coef[8];
    x12=(x12+x4)*coef[8];
    x14=(x14+x6)*coef[8];
    x13=(x13+x5)*coef[8];
    x15=(x15+x7)*coef[8];
    y0=x0+x8;
    y8=(x0-x8);
    y4=(x4+x12)*coef[4];
    y12=(x4-x12)*coef[12];
    y2=x2+x10;
    y10=x2-x10;
    y6=(x6+x14)*coef[4];
    y14=(x6-x14)*coef[12];
    y1=x1+x9;
    y9=x1-x9;
    y5=(x5+x13)*coef[4];
    y13=(x5-x13)*coef[12];
    y3=x3+x11;
    y11=x3-x11;
    y7=(x7+x15)*coef[4];
    y15=(x7-x15)*coef[12];
    x0=y0+y4;
    x4=y0-y4;
    x8=y8+y12;
    x12=y8-y12;
    x2=(y2+y6)*coef[2];
    x6=(y2-y6)*coef[14];
    x10=(y10+y14)*coef[6];
    x14=(y10-y14)*coef[10];
    x1=y1+y5;
    x5=y1-y5;
    x9=y9+y13;
    x13=y9-y13;
    x3=(y3+y7)*coef[2];
    x7=(y3-y7)*coef[14];
    x11=(y11+y15)*coef[6];
    x15=(y11-y15)*coef[10];
    y0=x0+x2;
    y2=x0-x2;
    y8=x8+x10;
    y10=x8-x10;
    y4=x4+x6;
    y6=x4-x6;
    y12=x12+x14;
    y14=x12-x14;
    y1=(x1+x3)*coef[1];
    y3=(x1-x3)*coef[15];
    y9=(x9+x11)*coef[3];
    y11=(x9-x11)*coef[13];
    y5=(x5+x7)*coef[7];
    y7=(x5-x7)*coef[9];
    y13=(x13+x15)*coef[5];
    y15=(x13-x15)*coef[11];
    x0=y0+y1;
    x1=y0-y1;
    x2=y2+y3;
    x3=y2-y3;
    x4=y4+y5;
    x5=y4-y5;
    x6=y6+y7;
    x7=y6-y7;
    x8=y8+y9;
    x9=y8-y9;
    x10=y10+y11;
    x11=y10-y11;
    x12=y12+y13;
    x13=y12-y13;
    x14=y14+y15;
    x15=y14-y15;
    tab[0]=x0;
    tab[1]=x8;
    tab[2]=x12;
    tab[3]=x4;
    tab[4]=x6;
    tab[5]=x14;
    tab[6]=x10;
    tab[7]=x2;
    tab[8]=x3;
    tab[9]=x11;
    tab[10]=x15;
    tab[11]=x7;
    tab[12]=x5;
    tab[13]=x13;
    tab[14]=x9;
    tab[15]=x1;
}

/*--------------------------------------------------------------------
*
*  CALCUL DES COEFFICIENTS DE LA MATRICE DE TRANSFORMATION DCT.
*
*-------------------------------------------------------------------- */
void cosi(double* coef)
{
    const double pi = 3.1415926535;
    for(int k = 1; k < 16; ++k) {
        coef[k] = 1. / ( 2. * cos( k * pi / 32.) );
    }
}
