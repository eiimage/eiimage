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

#include "FFT.h"

using namespace std;


typedef struct
        {
        double real;
        double imag;
        }
        COMPLEX;         //  complex

//  FFT & 2DFFT code written by Paul Bourke  July 1998
/*----------------------------------------------------------------------
---
   Calculate the closest but lower power of two of a number
   twopm = 2**m <= n
   Return TRUE if 2**m == n
*/
bool Powerof2( int n, int *m, int *twopm ) {
   if ( n <= 1 )
   {
      *m     = 0;
      *twopm = 1;
      return false;
   }

   *m     = 1;
   *twopm = 2;

   do
   {
      (*m)++;
      (*twopm) *= 2;
   } while ( 2 * (*twopm) <= n );

   if ( *twopm != n )
   {
      return false;
   }
   else
   {
      return true;
   }
}

int nearestUpPower2(int n) {
    int i;
    for(i = 1; i < n; i *= 2);
    return i;
}
/*-------------------------------------------------------------------------
   Perform a 2D FFT inplace given a complex 2D array
   The direction dir, 1 for forward, -1 for reverse
   The size of the array (nx,ny)
   Return false if there are memory problems or
      the dimensions are not powers of 2
*/
bool FFT2D(complex<double> **c,int nx,int ny,int dir)
{
   int m,twopm;
   double *real, *imag;

   /* Transform the rows */
   real = new double[nx];
   imag = new double[nx];
   if (real == NULL || imag == NULL)
      return false;
   if (!Powerof2(nx,&m,&twopm) || twopm != nx)
      return false;
   for (int j=0;j<ny;j++) {
      for (int i=0;i<nx;i++) {
         real[i] = c[i][j].real();
         imag[i] = c[i][j].imag();
      }
      FFT(dir,m,real,imag);
      for (int i=0;i<nx;i++) {
         c[i][j] = complex<double>(real[i], imag[i]);
      }
   }
   delete[] real;
   delete[] imag;

   /* Transform the columns */
   real = new double[ny];
   imag = new double[ny];
   if (real == NULL || imag == NULL)
      return false;
   if (!Powerof2(ny,&m,&twopm) || twopm != ny)
      return false;
   for (int i=0;i<nx;i++) {
      for (int j=0;j<ny;j++) {
         real[j] = c[i][j].real();
         imag[j] = c[i][j].imag();
      }
      FFT(dir,m,real,imag);
      for (int j=0;j<ny;j++) {
         c[i][j] = complex<double>(real[j], imag[j]);
      }
   }
   delete[] real;
   delete[] imag;

   return true;
}

/*-------------------------------------------------------------------------
   This computes an in-place complex-to-complex FFT
   x and y are the real and imaginary arrays of 2^m points.
   dir =  1 gives forward transform
   dir = -1 gives reverse transform

     Formula: forward
                  N-1
                  ---
              1   \          - j k 2 pi n / N
      X(n) = ---   >   x(k) e                    = forward transform
              N   /                                n=0..N-1
                  ---
                  k=0

      Formula: reverse
                  N-1
                  ---
                  \          j k 2 pi n / N
      X(n) =       >   x(k) e                    = forward transform
                  /                                n=0..N-1
                  ---
                  k=0
*/
void FFT(int dir,int m,double *x,double *y)
{
   long nn,i,i1,j,k,i2,l,l1,l2;
   double c1,c2,tx,ty,t1,t2,u1,u2,z;

   /* Calculate the number of points */
   nn = 1;
   for (i=0;i<m;i++)
      nn *= 2;

   /* Do the bit reversal */
   i2 = nn >> 1;
   j = 0;
   for (i=0;i<nn-1;i++) {
      if (i < j) {
         tx = x[i];
         ty = y[i];
         x[i] = x[j];
         y[i] = y[j];
         x[j] = tx;
         y[j] = ty;
      }
      k = i2;
      while (k <= j) {
         j -= k;
         k >>= 1;
      }
      j += k;
   }

   /* Compute the FFT */
   c1 = -1.0;
   c2 = 0.0;
   l2 = 1;
   for (l=0;l<m;l++) {
      l1 = l2;
      l2 <<= 1;
      u1 = 1.0;
      u2 = 0.0;
      for (j=0;j<l1;j++) {
         for (i=j;i<nn;i+=l2) {
            i1 = i + l1;
            t1 = u1 * x[i1] - u2 * y[i1];
            t2 = u1 * y[i1] + u2 * x[i1];
            x[i1] = x[i] - t1;
            y[i1] = y[i] - t2;
            x[i] += t1;
            y[i] += t2;
         }
         z =  u1 * c1 - u2 * c2;
         u2 = u1 * c2 + u2 * c1;
         u1 = z;
      }
      c2 = sqrt((1.0 - c1) / 2.0);
      if (dir == 1)
         c2 = -c2;
      c1 = sqrt((1.0 + c1) / 2.0);
   }

   /* Scaling for forward transform */
   if (dir == 1) {
      for (i=0;i<nn;i++) {
         x[i] /= (double)nn;
         y[i] /= (double)nn;
      }
   }

   return;
}
