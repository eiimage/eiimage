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

#include "Pyramid.h"
#include <Algorithm/Filtering.h>
#include <cstdio>
#include <cstring>
using namespace std;
using namespace imagein;
using namespace Pyramid;

const uint8_t tp6_filter_file_data[] = {0x74, 0x72, 0x69, 0x61, 0x6e, 0x67, 0x75, 0x6c, 0x61, 0x69, 0x72, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x80, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x67, 0x61, 0x75, 0x73, 0x73, 0x69, 0x65, 0x6e, 0x00, 0x00, 0xf8, 0x45, 0x26, 0x00, 0xf8, 0x45, 0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcd, 0xcc, 0xcc, 0x3e, 0x00, 0x00, 0x80, 0x3e, 0xcd, 0xcc, 0x4c, 0x3d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x74, 0x72, 0x69, 0x6d, 0x6f, 0x64, 0x61, 0x6c, 0x00, 0x00, 0x00, 0x45, 0x26, 0x00, 0xf8, 0x45, 0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9a, 0x99, 0x19, 0x3f, 0x00, 0x00, 0x80, 0x3e, 0xcd, 0xcc, 0x4c, 0xbd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x72, 0x65, 0x63, 0x74, 0x61, 0x6e, 0x67, 0x75, 0x6c, 0x61, 0x69, 0x72, 0x65, 0x00, 0x00, 0x00, 0xf8, 0x45, 0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xae, 0x47, 0xe1, 0x3d, 0xae, 0x47, 0xe1, 0x3d, 0xae, 0x47, 0xe1, 0x3d, 0xae, 0x47, 0xe1, 0x3d, 0xae, 0x47, 0xe1, 0x3d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x71, 0x6d, 0x66, 0x00, 0x58, 0xec, 0x00, 0x00, 0xf8, 0x45, 0x26, 0x00, 0x00, 0x00, 0xf8, 0x45, 0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x51, 0x88, 0x10, 0x3f, 0x16, 0xde, 0x95, 0x3e, 0x9c, 0xf9, 0x55, 0xbd, 0xae, 0xf0, 0x2e, 0xbd, 0x2f, 0x6e, 0xa3, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00};

Pyramid::Filters::Filters()
{
    num_filters=(int)sizeof(tp6_filter_file_data)/sizeof(filtre);
    filters = reinterpret_cast<const filtre*>(tp6_filter_file_data);
}

Pyramid::Filters::~Filters()
{
    if( filters != NULL ) {
//        delete[] filters;
    }
}

bool Pyramid::Filters::getFromPos( int pos, filtre &to_fill ) {
  bool found = false;
    if( pos >= 0 && pos < num_filters ) {
      found = true;
        copy_filter( filters[pos], to_fill );
    }
    return found;
}

bool Filters::getFromName( const char *name, filtre &to_fill ) {
  bool found = false;
    if( strcmp( name, "default - gaussien" ) == 0 ) {
      found = true;
        getDefault( to_fill );
    }
    else {
        int counter = 0;
        for( counter=0; counter< num_filters && !found; counter++ ) {
            if( strcmp( name, filters[counter].nom_f ) == 0 ) {
                found = true;
                copy_filter( filters[counter], to_fill );
            }
        }
    }
    return found;
}

int Filters::size() {
  return num_filters;
}

void Filters::copy_filter(const filtre &source, filtre &dest ) {
    int counter;
    for( counter=0; counter< 30; counter++ ) {
      dest.nom_f[counter] = source.nom_f[counter];
    }
    for( counter=0; counter< 10; counter++ ) {
      dest.coeff_f[counter] = source.coeff_f[counter];
    }
    dest.taille_f = source.taille_f;
}

void Filters::getDefault( filtre &to_fill ) {
  strcpy( to_fill.nom_f, "default - gaussien" );
    to_fill.taille_f =  (float)2;
    to_fill.coeff_f[0] =  (float)0.4;
    to_fill.coeff_f[1] =  (float)0.25;
    to_fill.coeff_f[2] =  (float)0.05;
}

/*---------------------------------------------------------------------------
    Cration de l'tage suivant de la pyramide Gaussienne
---------------------------------------------------------------------------*/
void Pyramid::etage_suiv_g(const uint8_t *srcTab, uint8_t *dstTab, int srcWidth, int srcHeight, filtre &utile)
{
    cout << "etage_suiv_g " << srcWidth << " " << srcHeight << " " << endl;
    cout << "src = " << (uintptr_t)srcTab << " dst = " << (uintptr_t)dstTab << endl;
    int dstWidth = srcWidth / 2, dstHeight = srcHeight / 2;
    uint8_t* intermed = new uint8_t[dstWidth * srcHeight];
    for(int j = 0; j < srcHeight; ++j)
    {
        for(int i = 0; i < dstWidth; ++i)
        {
            intermed[i + j * dstWidth] = filt_point_1d_lg(srcTab, srcWidth, 2 * i, j, utile);
//            intermed[i + j * dstWidth] = srcTab[2*i + j * srcWidth];
        }
    }
    for(int i = 0; i < dstWidth; ++i)
    {
        for(int j = 0; j < dstHeight; ++j)
        {
            dstTab[i + j * dstWidth] = filt_point_1d_cl(intermed, srcHeight, dstWidth, i, j * 2, utile);
//            dstTab[i + j * dstWidth] = intermed[i + 2 * j * dstWidth];
        }
    }
    delete intermed;
}

/*---------------------------------------------------------------------------
    Cration d'une pyramide Gaussienne jusqu'au nime tage
---------------------------------------------------------------------------*/
void Pyramid::pyram_g_n(uint8_t *rep, int nStage, long nbc, long nbl, const uint8_t *itab, filtre &utile)
{
    cout << "pyram_g_n " << nStage << " " << nbc << " " << nbl << endl;
    int taille_c=nbc;
    int taille_l=nbl;
    const int size = nbc * nbl;
    for(int i = 0; i < size; ++i)
    {
        rep[i] = itab[i];
    }

    int j = 0;
    for(int fin = nStage; fin > 0 ; --fin)
    {
        etage_suiv_g((rep+j),(rep+j+taille_c*taille_l),taille_c,taille_l,utile);
        j=j+taille_c*taille_l;
        taille_l=taille_l/2;
        taille_c=taille_c/2;
    }
}

/*---------------------------------------------------------------------------
    Cration d'une pyramide Laplacienne jusqu'au nime tage
---------------------------------------------------------------------------*/
void Pyramid::pyram_l_n (uint8_t *rep,int n, long nbc, long nbl, const uint8_t *itab, filtre &utile)
{
    int i;
    int j=0;
    int fin=n;
    int taille_c=nbc;
    int taille_l=nbl;
    uint8_t* pyra1 = new uint8_t[nbc*nbc*2];

    pyram_g_n(pyra1,fin, nbc, nbl, itab, utile);
    do
    {
        agrandir((pyra1+j+taille_c*taille_l),(rep+j),taille_c/2,taille_l/2,utile);

        for(i=j;i<j+taille_c*taille_l;i++)
        {
            /* this overflow is important ! */
            int n = 0;
            n = n + pyra1[i];
            n = n - rep[i];
            if(n < -127) n = -127;
            if(n > 128) n = 128;
//            if(n > 255) n = 255;
//            if(n < 0) n = 0;
            rep[i] = n;
        }

        j=j+taille_c*taille_l;
        taille_c=taille_c/2;
        taille_l=taille_l/2;
        fin--;
    }while(fin>0);
    for(i=j;i<j+taille_c*taille_l;i++)
    {
        *(rep+i)=*(pyra1+i);
    }
    delete[] pyra1;
}


/*---------------------------------------------------------------------------
    Cration d'une pyramide Gaussienne avec entre en conversationnel
    des diffrentes proprits de cette pyramide
---------------------------------------------------------------------------*/
Image *Pyramid::pyram_g(const GrayscaleImage *im, int etage_f, filtre &utile, string &to_print)
{
    if(!( im != NULL )) {
        throw "Error in Pyramid::pyram_g:\nim = NULL";
    }
    if(!( im->getWidth() == im->getHeight() )) {
        throw  "Error in Pyramid::pyram_g:\nim->getWidth() != im->getHeight()";
    }
    if( !isPowerOf2(im->getWidth()) ) {
        throw  "Error in Pyramid::pyram_g:\nimage dimensions not a power of 2";
    }
    long nbl = im->getHeight();
    long nbc = im->getWidth();
    const uint8_t* itab = im->begin();

//    uint8_t* rep = new uint8_t[nbc * nbl * 2];
    GrayscaleImage* resImg = new GrayscaleImage(im->getWidth(), im->getHeight() * 2);
    uint8_t* rep = resImg->begin();
    cout << "rep = " << (uintptr_t)rep << endl;
    int temp_etage_max = etage_max( im );
    if( etage_f > temp_etage_max ) etage_f = temp_etage_max;
    if( etage_f < 1 ) etage_f = 1;

    pyram_g_n(rep,etage_f,nbc,nbl,itab,utile);
    to_print = entropie_p(rep,etage_f,nbc,nbl);
    reconstruction(rep,etage_f,nbc,nbl);

    return resImg;
}
/*---------------------------------------------------------------------------
    Cration d'un tage de la  pyramide Gaussienne
---------------------------------------------------------------------------*/
Image *Pyramid::n_pyram_g(const Image *im, int etage_f, filtre &utile )
{
    if(!( im != NULL )) {
        throw "Error in Pyramid::pyram_g:\nim = NULL";
    }
    if(!( im->getWidth() == im->getHeight() )) {
        throw  "Error in Pyramid::pyram_g:\nim->getWidth() != im->getHeight()";
    }
    if( !isPowerOf2(im->getWidth()) ) {
        throw  "Error in Pyramid::pyram_g:\nimage dimensions not a power of 2";
    }

    int i;
    int k=0;
    int taille_c,taille_l;
    long nbl = im->getHeight();
    long nbc = im->getWidth();
    const uint8_t* itab = im->begin();

    taille_c=nbc;
    taille_l=nbl;
    int temp_etage_max = etage_max(im);
    if( etage_f > temp_etage_max ) etage_f = temp_etage_max;
    if( etage_f < 1 ) etage_f = 1;

    for(i=0;i<etage_f;i++)
    {
        k=k+taille_c*taille_l;
        taille_c=taille_c/2;
        taille_l=taille_l/2;
    }
    GrayscaleImage* resImg = new GrayscaleImage(taille_c, taille_l);
    uint8_t* rep = resImg->begin();
    uint8_t* tab = new uint8_t[(nbc * nbc) / 2 * 3];
    pyram_g_n(tab,etage_f,nbc,nbl,itab,utile);
    for(i=0;i<taille_c*taille_l;i++)
    {
        rep[i] = tab[i+k];
    }
    delete[] tab;
    return resImg;
}
/*---------------------------------------------------------------------------
    Cration d'une pyramide Laplacienne avec entre en conversationnel
    des diffrentes proprits de cette pyramide
---------------------------------------------------------------------------*/
Image *Pyramid::pyram_l (const Image *im, int etage_f, filtre &utile, string &to_print)
{
    if(!( im != NULL )) {
        throw "Error in Pyramid::pyram_g:\nim = NULL";
    }
    if(!( im->getWidth() == im->getHeight() )) {
        throw  "Error in Pyramid::pyram_g:\nim->getWidth() != im->getHeight()";
    }
    if( !isPowerOf2(im->getWidth()) ) {
        throw  "Error in Pyramid::pyram_g:\nimage dimensions not a power of 2";
    }
    long nbl = im->getHeight();
    long nbc = im->getWidth();
    const uint8_t* itab = im->begin();

//    uint8_t* rep = new uint8_t[nbc * nbl * 2];
    GrayscaleImage* resImg = new GrayscaleImage(im->getWidth(), im->getHeight() * 2);
    uint8_t* rep = resImg->begin();
    int temp_etage_max = etage_max( im );
    if( etage_f > temp_etage_max ) etage_f = temp_etage_max;
    if( etage_f < 1 ) etage_f = 1;

    pyram_l_n(rep,etage_f,nbc,nbl,itab,utile);
    to_print = entropie_p(rep,etage_f,nbc,nbl);
    reconstruction(rep,etage_f,nbc,nbl);

    return resImg;

}
/*---------------------------------------------------------------------------
    Cration d'un tage de la  pyramide Laplacienne
---------------------------------------------------------------------------*/
Image *Pyramid::n_pyram_l(const Image *im, int etage_f, filtre &utile)
{
    if(!( im != NULL )) {
        throw "Error in Pyramid::pyram_g:\nim = NULL";
    }
    if(!( im->getWidth() == im->getHeight() )) {
        throw  "Error in Pyramid::pyram_g:\nim->getWidth() != im->getHeight()";
    }
    if( !isPowerOf2(im->getWidth()) ) {
        throw  "Error in Pyramid::pyram_g:\nimage dimensions not a power of 2";
    }

    int i;
    int k=0;
    int taille_c,taille_l;
    long nbl = im->getHeight();
    long nbc = im->getWidth();
    const uint8_t* itab = im->begin();

    taille_c=nbc;
    taille_l=nbl;
    int temp_etage_max = etage_max(im) - 1;
    if( etage_f > temp_etage_max ) etage_f = temp_etage_max;
    if( etage_f < 0 ) etage_f = 0;

    for(i=0;i<etage_f;i++)
    {
        k=k+taille_c*taille_l;
        taille_c=taille_c/2;
        taille_l=taille_l/2;
    }
    GrayscaleImage* resImg = new GrayscaleImage(taille_c, taille_l);
    uint8_t* rep = resImg->begin();
    uint8_t* tab = new uint8_t[(nbc * nbc) / 2 * 3];
    pyram_g_n(tab,etage_f,nbc,nbl,itab,utile);
    pyram_l_n(tab,etage_f+1,nbc,nbl,itab,utile);
    for(i=0;i<taille_c*taille_l;i++)
    {
        rep[i] = tab[i+k];
    }
    delete[] tab;
    return resImg;
}



/*---------------------------------------------------------------------------
    Filtrage d'un point de l'image suivant une ligne
---------------------------------------------------------------------------*/
uint8_t Pyramid::filt_point_1d_lg(const uint8_t *tab,int cl, int x, int y, filtre &utile)
{
    float partiel=0.;
    int i;
    int j;
    j=utile.taille_f;
    if(x-j<0)
    {
        for(i=-j;i<=j;i++)
        {
            if(x+i<0)
                partiel += *(tab+y*cl+x-i)*utile.coeff_f[-i];
            else if(i<0)
                partiel += *(tab+y*cl+x+i)*utile.coeff_f[-i];
            else
                partiel += *(tab+y*cl+x+i)*utile.coeff_f[i];
        }
    }else if(x+j<cl)
    {
        for(i=-j;i<=j;i++)
        {
            if(i<0)
                partiel += *(tab+y*cl+x+i)*utile.coeff_f[-i];
            else
                partiel += *(tab+y*cl+x+i)*utile.coeff_f[i];
        }
    }
    else
    {
        for(i=-j;i<=j;i++)
        {
            if(i<0)
                partiel += *(tab+y*cl+x+i)*utile.coeff_f[-i];
            else if(i+x<cl)
                partiel += *(tab+y*cl+x+i)*utile.coeff_f[i];
            else
                partiel += *(tab+y*cl+x-i)*utile.coeff_f[i];
        }
    }
    return((uint8_t)partiel);
}

/*---------------------------------------------------------------------------
    Filtrage d'un point de l'image suivant une colonne
---------------------------------------------------------------------------*/
uint8_t Pyramid::filt_point_1d_cl(const uint8_t *tab,int lg,int cl, int x, int y, filtre &utile)
{
    float partiel=0.;
    int i;
    int j=utile.taille_f;
    if(y-j<0)
    {
        for(i=-j;i<=j;i++)
        {
            if(y+i<0)
                partiel += *(tab+x+(y-i)*cl)*utile.coeff_f[-i];
            else if(i<0)
                partiel += *(tab+x+(y+i)*cl)*utile.coeff_f[-i];
            else
                partiel += *(tab+x+(y+i)*cl)*utile.coeff_f[i];
        }
    }else if(y+j<lg)
    {
        for(i=-j;i<=j;i++)
        {
            if(i<0)
                partiel += *(tab+x+(y+i)*cl)*utile.coeff_f[-i];
            else
                partiel += *(tab+x+(y+i)*cl)*utile.coeff_f[i];
        }
    }
    else
    {
        for(i=-j;i<=j;i++)
        {
            if(i<0)
                partiel += *(tab+x+(y+i)*cl)*utile.coeff_f[-i];
            else if(i+y<cl)
                partiel += *(tab+x+(y+i)*cl)*utile.coeff_f[i];
            else
                partiel += *(tab+x+(y-i)*cl)*utile.coeff_f[i];
        }
    }
    return((uint8_t)partiel);
}
/*---------------------------------------------------------------------------
    Filtrage d'une ligne
---------------------------------------------------------------------------*/
void Pyramid::filt_ligne(uint8_t *ligne,int taille_c, filtre &utile)
{
    int i;
    uint8_t *intermed;
    intermed=(uint8_t*)calloc(taille_c,sizeof(uint8_t));
    for(i=0;i<taille_c;i++)
    {
        *(intermed+i)=filt_point_1d_lg(ligne,taille_c,i,0,utile);
    }
    for(i=0;i<taille_c;i++)
    {
        *(ligne+i)=*(intermed+i);
    }
    free(intermed);
}
/*---------------------------------------------------------------------------
    Filtrage d'une image suivant les colonnes
---------------------------------------------------------------------------*/
void Pyramid::filt_tab_cl(uint8_t *tab,int taille_c,int taille_l, filtre &utile)
{
    int i,j;
    uint8_t *intermed;
    intermed=(uint8_t*)calloc(taille_c*taille_l,sizeof(uint8_t));
    for(j=0;j<taille_l;j++)
    {
        for(i=0;i<taille_c;i++)
        {
            *(intermed+i+j*taille_c)=filt_point_1d_cl(tab,taille_l,taille_c,i,j,utile);
        }
    }
    for(j=0;j<taille_l;j++)
    {
        for(i=0;i<taille_c;i++)
        {
            *(tab+i+j*taille_c)= *(intermed+i+j*taille_c) * 4;
        }
    }
    free(intermed);
}

/*---------------------------------------------------------------------------
    Agrandissement d'une image (avec filtrage)
---------------------------------------------------------------------------*/
void Pyramid::agrandir(uint8_t *petit,uint8_t *grand,int taille_c,int taille_l, filtre &utile)
{
    int i,j;
    uint8_t *intermed;
    intermed=(uint8_t*)calloc(taille_c*taille_l*2,sizeof(uint8_t));
    for(j=0;j<taille_l;j++)
    {
        for(i=0;i<taille_c;i++)
        {
            *(intermed+i*2+j*taille_c*2)=*(petit+i+j*taille_c);
            *(intermed+i*2+j*taille_c*2+1)=0;
        }
    }
    for(j=0;j<taille_l;j++)
    {
        filt_ligne((intermed+j*taille_c*2),taille_c*2,utile);
    }
    for(j=0;j<taille_l;j++)
    {
        for(i=0;i<taille_c*2;i++)
        {
            *(grand+i+(j*2)*taille_c*2)=*(intermed+i+j*taille_c*2);
            *(grand+i+(j*2+1)*taille_c*2)=0;
        }
    }
    filt_tab_cl(grand,taille_c*2,taille_l*2,utile);
    free(intermed);
}

/*---------------------------------------------------------------------------
    Rarangement de la pyramide
---------------------------------------------------------------------------*/

void Pyramid::reconstruction(uint8_t *pyra,int n, long nbc, long nbl)
{
    int i,j,k;
    int q=0;
    int p=0;
    int fin=n;
    int taille_c=nbc;
    int taille_l=nbl;
    uint8_t *intermed;
    intermed=(uint8_t*)calloc(nbc*nbl,sizeof(uint8_t));
    do
    {
        q=q+taille_c*taille_l;
        taille_c=taille_c/2;
        taille_l=taille_l/2;
        k=0;
        for(j=p;j<p+taille_l;j++)
        {
            for(i=0;i<taille_c;i++)
            {
                *(intermed+j*nbc+i)=*(pyra+k+q);
                k++;
            }
        }
        p=p+taille_l;
        fin--;
    }while(fin>0);
    for(i=0;i<nbc*nbl;i++)
    {
        *(pyra+nbc*nbl+i)=*(intermed+i);
    }
   free(intermed);
}

int Pyramid::etage_max(const Image *im)
{
    int i;
    int taille_c=im->getWidth();
    int taille_l=im->getHeight();
    for(i=0;taille_c||taille_l>1;i++)
    {
        taille_c=taille_c/2;
        taille_l=taille_l/2;
    }
    i--;
    return(i);
}

/*----------------------------------------------------------------------
    Calcul de l'entropie d'une image donne en paramtre,
    la fonction retourne la valeur de l'entropie.
----------------------------------------------------------------------*/
float Pyramid::entropie2(const uint8_t *tab,int taille_c,int taille_l)
{
    int i,j;
    float pi[256],h=0;
    int size;

    size=taille_c*taille_l;

/**********************************************************************/
/*                    module de traitement                            */
/**********************************************************************/

/* initialisation  0 du tableau pi contenant l'histogramme */
    for(i=0 ; i<256 ; i++)
      pi[i] = 0;

/* calcul de l'histogramme de l'image */
    for(i=0 ; i<taille_l ; i++)
      for(j=0 ; j<taille_c ; j++)
         pi[*(tab+ i*taille_c +j)]++;

/* calcul de l'entropie de l'image */
    for(i=0 ; i<256 ; i++)
    {
      if(pi[i] != 0)
      {
         pi[i] /= size;
         h -= (double)pi[i] * log((double)pi[i])/log((double)2.0);
      }
    }
    return(h);
}

/*----------------------------------------------------------------------
 Calcul et affichage de l'entropie des diffrents tages d'une pyramide
----------------------------------------------------------------------*/
string Pyramid::entropie_p(const uint8_t *pyra,int etage_f,int nbc,int nbl)
{
    int i;
    int j=0;
    float h;
    int taille_c=nbc;
    int taille_l=nbl;
    char buffer[255];
    string returnval;
    for(i=0;i<=etage_f;i++)
    {
        h=entropie2((pyra+j),taille_c,taille_l);
        j=j+taille_c*taille_l;
        taille_c=taille_c/2;
        taille_l=taille_l/2;
        sprintf(buffer, "L'entropie de l'tage %d est %1f\n",i,h);
        returnval = returnval + buffer;
    }
   return returnval;
}


Image *Pyramid::rebuild_interface( const Image *pyramid, int etage_f, int pyramid_to, filtre &utile ) {
    // rebuilds from an image that was saved
    // etage_f = stage that the original image was built to
    char buffer[255];
    if(!pyramid) {
        throw "Error in TP6Pyramid::rebuild_interface:\npyramid = NULL";
    }
    if(pyramid->getWidth() * 2 != pyramid->getHeight()) {
        sprintf( buffer, "Error in TP6Pyramid::rebuild_interface:\npyramid->getWidth = %d, pyramid->getHeight = %d", pyramid->getWidth(), pyramid->getHeight() );
        throw buffer;
    }
    if(!(isPowerOf2(pyramid->getWidth()))) {
        throw "Error in TP6Pyramid::rebuild_interface:\npyramid->getWidth not a power of 2";
    }
    if( etage_f <= 0 || (1 << etage_f) > pyramid->getWidth() ) {
        throw "Error in TP6Pyramid::rebuild_interface:\nInvalid etage_f specified";
    }
    if(pyramid_to < 0 || pyramid_to >= etage_f) {
        throw "Error in TP6Pyramid::rebuild_interface:\nInvalid pyramid_to specified";
    }
    long nbc = pyramid->getWidth();
    long nbl = pyramid->getWidth();
    int i;
    int j=0;
    int taille_c=nbc;
    int taille_l=nbl;
    Image *returnval = NULL;
    int taille;
    int q=0;
    int etage_rec;
    const uint8_t *pyra1 = pyramid->begin();
    uint8_t* intermed = new uint8_t[nbc * nbc / 2 * 3];
    uint8_t* rep = new uint8_t[nbc * nbc / 2 * 3];

    // Copy info into rep in a packed format
    long current_offset = 0;
    long current_rep_offset = 0;
    for(int n = 0; n <= etage_f; ++n) {
        for(int j=0; j< taille_l; j++ ) {
            for(int i=0; i< taille_c; i++ ) {
                rep[current_rep_offset + j * taille_c + i] = pyra1[current_offset + j * pyramid->getWidth() + i];
            }
        }
        current_offset = current_offset + taille_l * nbc;
        current_rep_offset = current_rep_offset + taille_l * taille_c;
        taille_l = taille_l / 2;
        taille_c = taille_c / 2;
    }

/* reconstruction de l'image  partir de sa pyramide laplacienne en s'arretant
     un niveau etage_rec choisi par l'utilisateur
    La pyramide est contenue dans la zone mmoire pointe par rep */
    etage_rec = pyramid_to;
    taille=nbc;

    for(i=0;i<etage_f;i++)
    {
        q=q+taille*taille;
        taille=taille/2;
    }
    for(i=q;i<q+taille*taille;i++)
    {
        *(intermed+i)=*(rep+i);
    }
    for(i=etage_f;i>etage_rec;i--)
    {
        agrandir((intermed+q),(intermed+q-taille*taille*4),taille,taille,utile);
        taille=taille*2;
        q=q-taille*taille;
        for(j=q;j<q+taille*taille;j++)
        {
            int value = rep[j];
            if(value > 128) value = value - 256;
            value = value + intermed[j];
            if(value < 0) value = 0;
            if(value > 255) value = 255;
            intermed[j] = value;
        }
    }
    //printf("Entrez le nom du fichier de l'image reconstruite :");
    //scanf("%s",nom);
    //ecrire_image((intermed+q),nom,taille,taille);
    returnval = new GrayscaleImage(taille, taille, intermed+q);
    free(intermed);
    free(rep);
    return returnval;
}
