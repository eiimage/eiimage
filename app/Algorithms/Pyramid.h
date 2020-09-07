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


/*!
 * \brief
 *
 * \param 
 * \return
*/

namespace Pyramid
{   
    /*!
    * \brief return if a number is a Powerof2 or not
    * \param n the number to test
    * \return true if n is a power of2 false otherwise
    */
    inline bool isPowerOf2(int n) {
        return ( ( n > 0 ) && ( (n & (n - 1) ) == 0) );
    }

    /*!
    * \struct Filtre 
    * \Brief A stucture reprensenting a filter 
    *   
    *   Coefficients are given to only one side of the filter, we use symetry for the others
    *   example : if coeff_f = {1, 0.5 , 0.25, ...},  the filter really applied is  {..., 0.25, 0.5, 1, 0.5, 0.25, ..}
    *   Size is the maximum index to search for coefficient in the coeff_f[] 
    */
    struct Filtre {
          char nom_f[30];         //!< Name of the filter
          float coeff_f[10];      //!< Coefficient of the filter
          int taille_f;                 //!< Size of the filter
    };

    /*!
    * \Class Filters
    * \Brief A class used for pyramid operation
    *   
    * This class regroup all the filters possible in a tab
    */
    class Filters
    {
    public:
        Filters();
        virtual ~Filters();
        /**
         * @brief Retrieve a filter in the filters array with its position
         * 
         * @param pos the position of the filter in the array
         * @param to_fill the pointer where the filter will be return 
         * @return true if the filter was found 
         * @return false if the filter was not found
         */
        bool getFromPos( int pos, Filtre &to_fill );
        
        /**
         * @brief Retrieve a filter in the filters array with its name
         * 
         * @param name the name of the filter 
         * @param to_fill the pointer to return the found filter in
         * @return true if the filter was found 
         * @return false if the filter was not found
         */
        bool getFromName( const char *name, Filtre &to_fill );
        /**
         * @brief Get the Default filter 
         * 
         * @param to_fill  the pointer to return the filter in
         */
        void getDefault( Filtre &to_fill );
        /**
         * @brief get the number of filter in the filters array
         * 
         * @return int the number of filter 
         */
        int size();
    private:
        const Filtre *filters; //!< Array keeping every filter available
        int num_filters;       //!< Number of available filter 
        /**
         * @brief Copy a filter
         * 
         * @param source filter to copy
         * @param dest destination filter 
         */
        void copy_filter( const Filtre &source, Filtre &dest ); 
    };

    /**
     * @brief Create the next level of a gaussian pyramid 
     * 
     * @param srcTab previous level data
     * @param dstTab new level data 
     * @param srcWidth Width of the previous level
     * @param srcHeight hegiht of the previous level 
     * @param utile the filter to use 
     */
    void etage_suiv_g(const uint8_t *srcTab, uint8_t *dstTab, int srcWidth, int srcHeight, Filtre &utile);
    
    /**
     * @brief Create a gaussian pyramid 
     * 
     * @param rep the created pyramid
     * @param nStage number of level wanted in the pyramid 
     * @param nbc width of the original image
     * @param nbl height of the original image
     * @param itab source image
     * @param utile filter to use
     */
    void pyram_g_n(uint8_t *rep, int nStage, long nbc, long nbl, const uint8_t *itab, Filtre &utile);
    
    /**
     * @brief Create a laplacian pyramid 
     * 
     * @param rep the created pyramid
     * @param nStage number of level wanted in the pyramid 
     * @param nbc width of the original image
     * @param nbl height of the original image
     * @param itab source image
     * @param utile filter to use
     */
    void pyram_l_n (uint8_t *rep,int n, long nbc, long nbl, const uint8_t *itab, Filtre &utile);

    /**
     * @brief Create a gaussian pyramid and return an image of it
     * 
     * @param im the to build pyramid from 
     * @param etage_f number of level in the pyramid
     * @param utile filter to use 
     * @param to_print text to print in the log window
     * @return imagein::Image* the image of the new pyramid
     */
    imagein::Image *pyram_g(const imagein::GrayscaleImage *im, int etage_f, Filtre &utile, std::string &to_print);
    
    /**
     * @brief Create a level of a gaussian pyramid and return an image of it
     * 
     * @param im the to build pyramid from 
     * @param etage_f number of the level to create
     * @param utile filter to use
     * @return imagein::Image* the image of the new pyramid
     */
    imagein::Image *n_pyram_g(const imagein::Image *im, int etage_f, Filtre &utile, std::string &to_print);
    
    /**
     * @brief Create a Laplacian pyramid and return an image of it
     * 
     * @param im the to build pyramid from 
     * @param etage_f number of level in the pyramid
     * @param utile filter to use 
     * @param to_print text to print in the log window
     * @return imagein::Image* the image of the new pyramid
     */
    imagein::Image *pyram_l (const imagein::Image *im, int etage_f, Filtre &utile, std::string &to_print);
    
    /**
     * @brief Create a level of a gaussian pyramid and return an image of it
     * 
     * @param im the to build pyramid from 
     * @param etage_f number of the level to create
     * @param utile filter to use
     * @return imagein::Image* the image of the new pyramid
     */
    imagein::Image *n_pyram_l(const imagein::Image *im, int etage_f, Filtre &utile, std::string &to_print);
    
    /**
     * @brief Proccess the value by applying filter on lign
     * 
     * @param tab data of the lign 
     * @param cl number of column
     * @param x position of the pixel
     * @param y position of the pixel
     * @param utile filter to apply
     * @return uint8_t the value of the pixel
     */
    uint8_t filt_point_1d_lg(const uint8_t *tab,int cl, int x, int y, Filtre &utile);
        /**
     * @brief Proccess the value by applying filter on column
     * 
     * @param tab data of the column 
     * @param cl number of lign
     * @param x position of the pixel
     * @param y position of the pixel
     * @param utile filter to apply
     * @return uint8_t the value of the pixel
     */
    uint8_t filt_point_1d_cl(const uint8_t *tab,int lg,int cl, int x, int y, Filtre &utile);

    /**
     * @brief apply filtering on a lign
     * 
     * @param ligne the data of the lign
     * @param taille_c number of column
     * @param utile filter to apply
     */
    void filt_ligne(uint8_t *ligne,int taille_c, Filtre &utile);
    
    /**
     * @brief apply filtering on a c
     * 
     * @param tab the data of the column
     * @param taille_c number of column
     * @param taille_l number of lign
     * @param utile filter to apply
     */
    void filt_tab_cl(uint8_t *tab,int taille_c,int taille_l, Filtre &utile);
    
    /**
     * @brief Enlarging an image (with filtering)
     * 
     * @param petit the input image
     * @param grand the output image
     * @param taille_c number of column
     * @param taille_l number of lign
     * @param utile filter to apply
     */
    void agrandir(uint8_t *petit,uint8_t *grand,int taille_c,int taille_l, Filtre &utile);
    /**
     * @brief 
     * 
     * @param pyra the pyramide 
     * @param n the number of level 
     * @param nbc number of column
     * @param nbl number of lign
     */
    void reconstruction(uint8_t *pyra,int n, long nbc, long nbl);

    /**
     * @brief process the maximum number of level for an image
     * 
     * @param im input image
     * @return int the maximum number of level 
     */
    int etage_max(const imagein::Image *im);

    /**
     * @brief proccess the entropy of an image
     * 
     * @param tab data from the image
     * @param taille_c number of column
     * @param taille_l number of lign
     * @return float the entropy
     */
    float entropie2(const uint8_t *tab,int taille_c,int taille_l);

    /**
     * @brief Return a directly printable string of the entropy 
     * 
     * @param pyra data to proccess entropy 
     * @param etage_f the level of entropy 
     * @param nbc number of column
     * @param nbl number of lign
     * @return std::string the printable entropy 
     */
    std::string entropie_p(const uint8_t *pyra,int etage_f, int nbc, int nbl);

     /**
     * @brief Return a directly printable string of the entropy for only one level
     * 
     * @param pyra data to proccess entropy 
     * @param etage_f the level of entropy 
     * @param nbc number of column
     * @param nbl number of lign
     * @return std::string the printable entropy 
     */
    std::string n_entropie_p(const uint8_t *pyra, int etage_f,int nbc,int nbl);

    /**
     * @brief Return the image reconstructed from a single layer in pyramide
     *
     * @param to_rebuild pyramid layer to rebuild
     * @param etage_f number of level in the pyramid
     * @param to_rebuild_to level to reconstruct
     * @param utile filter to use
     * @return imagein::Image* the output reconstructed image
     */
    imagein::Image* rebuild_basic(const imagein::Image *layer, int etage_f, int to_rebuild_to, Filtre &utile );

    /**
     * @brief Return the image reconstructed from a pyramide
     * 
     * @param to_rebuild pyramid to rebuild
     * @param etage_f number of level in the pyramid
     * @param to_rebuild_to level to reconstruct 
     * @param utile filter to use
     * @return imagein::Image* the output reconstructed image
     */
    imagein::Image* rebuild_interface(const imagein::Image *to_rebuild, int etage_f, int to_rebuild_to, Filtre &utile );
}

#endif // PYRAMID_H
