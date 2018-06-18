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

#ifndef PYRAMID_H
#define PYRAMID_H

#include <Image.h>
#include <GrayscaleImage.h>
#include <string>

namespace Pyramid
{
    inline bool isPowerOf2(int n) {
        return ( ( n > 0 ) && ( (n & (n - 1) ) == 0) );
    }

    typedef struct Filtre {
                char nom_f[30];
                float coeff_f[10];
          int taille_f;
    };

    class Filters
    {
    public:
        Filters();
        virtual ~Filters();
        bool getFromPos( int pos, Filtre &to_fill );
        bool getFromName( const char *name, Filtre &to_fill );
        void getDefault( Filtre &to_fill );
        int size();
    private:
        const Filtre *filters;
        int num_filters;
        void copy_filter( const Filtre &source, Filtre &dest );
    };

    void etage_suiv_g(const uint8_t *srcTab, uint8_t *dstTab, int srcWidth, int srcHeight, Filtre &utile);
    void pyram_g_n(uint8_t *rep, int nStage, long nbc, long nbl, const uint8_t *itab, Filtre &utile);
    void pyram_l_n (uint8_t *rep,int n, long nbc, long nbl, const uint8_t *itab, Filtre &utile);
    imagein::Image *pyram_g(const imagein::GrayscaleImage *im, int etage_f, Filtre &utile, std::string &to_print);
    imagein::Image *n_pyram_g(const imagein::Image *im, int etage_f, Filtre &utile );
    imagein::Image *pyram_l (const imagein::Image *im, int etage_f, Filtre &utile, std::string &to_print);
    imagein::Image *n_pyram_l(const imagein::Image *im, int etage_f, Filtre &utile);
    uint8_t filt_point_1d_lg(const uint8_t *tab,int cl, int x, int y, Filtre &utile);
    uint8_t filt_point_1d_cl(const uint8_t *tab,int lg,int cl, int x, int y, Filtre &utile);
    void filt_ligne(uint8_t *ligne,int taille_c, Filtre &utile);
    void filt_tab_cl(uint8_t *tab,int taille_c,int taille_l, Filtre &utile);
    void agrandir(uint8_t *petit,uint8_t *grand,int taille_c,int taille_l, Filtre &utile);
    void reconstruction(uint8_t *pyra,int n, long nbc, long nbl);
    int etage_max(const imagein::Image *im);
    float entropie2(const uint8_t *tab,int taille_c,int taille_l);
    std::string entropie_p(const uint8_t *pyra,int etage_f,int nbc,int nbl);
    imagein::Image* rebuild_interface(const imagein::Image *to_rebuild, int etage_f, int to_rebuild_to, Filtre &utile );
}

#endif // PYRAMID_H
