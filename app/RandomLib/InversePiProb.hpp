/**
 * \file InversePiProb.hpp
 * \brief Header for InversePiProb
 *
 * Return true with probabililty 1/\e pi.
 *
 * Copyright (c) Charles Karney (2012) <charles@karney.com> and licensed
 * under the MIT/X11 License.  For more information, see
 * http://randomlib.sourceforge.net/
 **********************************************************************/

#if !defined(RANDOMLIB_INVERSEPIPROB_HPP)
#define RANDOMLIB_INVERSEPIPROB_HPP \
  "$Id: 5394753ce030151afe8f8524f0ca5a80d560e98d $"

#include <cstdlib>              // for abs(int)
#include <RandomLib/Random.hpp>

namespace RandomLib {

  /**
   * \brief Return true with probability 1/\e pi.
   *
   * InverseEProb p; p(Random& r) returns true with prob 1/\e pi using the
   * method of Flajolet et al.  It consumes 9.6365 bits per call on average.
   *
   * The method is given in<br>

   * - P. Flajolet, M. Pelletier, and M. Soria,<br>
   *   On Buffon Machines and Numbers,<br> Proc. 22nd ACM-SIAM Symposium on
   *   Discrete Algorithms (SODA), Jan. 2011.<br>
   *   http://www.siam.org/proceedings/soda/2011/SODA11_015_flajoletp.pdf <br>
   * .
   * using the identity \verbatim
   1/pi = sum( binomial(2n,n)^3 (6n+1)/(2^(8n+2)), n = 0..infty)
\endverbatim
   *
   * It is based on an expression for 1/pi given by Eq. (28) of<br>
   * - S. Ramanujan,<br>
   *   Modular Equations and Approximations to pi,<br>
   *   Quart. J. Math. 45, 350-372 (1914);<br>
   *   In Collected Papers, edited by G. H. Hardy, P. V. Sechu Aiyar,
   *   B. M. Wilson (Cambridge Univ. Press, 1927; preprinted AMS, 2000).<br>
   *   http://books.google.com/books?id=oSioAM4wORMC&pg=PA36 <br>
   * .
   * \verbatim
   4/pi = 1 +  7/4  *(1    /(2)    )^3
            + 13/4^2*(1*3  /(2*4)  )^3 +
            + 19/4^3*(1*3*5/(2*4*6))^3 + ...
\endverbatim
   * Flajolet et al. write this as  \verbatim
   1/pi = sum( binomial(2*n,n)^3 * (6*n+1)/(2^(8*n+2)), n = 0..infty)
\endverbatim
   * (however, their paper misprints the final exponent of 2 as 8*n+4).
   *
   * Proof of the algorithm: \verbatim
   1/pi = sum( (binomial(2*n,n)/2^(2*n))^3 * (6*n+1)/2^(2*n+2), n, 0..infty)
\endverbatim
   * But \verbatim
   sum( (6*n+1)/2^(2*n+2), n, 0..infty) = 1
\endverbatim
   * so algorithm becomes:
   * -# pick n >= 0 with prob (6*n+1)/2^(2*n+2) (mean n = 11/9);
   * -# return true with prob (binomial(2*n,n)/2^(2*n))^3.
   *
   * Implement (1) as
   * - geom4(r) + geom4(r) returns n with prob 9*(n+1)/2^(2*n+4);
   * - geom4(r) + geom4(r) + 1 returns n with prob 36*n/2^(2*n+4);
   * - Combine these with probs [4/9, 5/9] to yield (6*n+1)/2^(2*n+2), as
   *   required.
   * .
   * Implement (2) as the outcome of 3 coin tossing experiments of 2*n tosses
   * with success defined as equal numbers heads and tails.
   *
   * This class illustrates how to return an exact result using coin tosses
   * only.  A more efficient implementation (which is still exact) would
   * replace prob59 by r.Prob(5,9) and geom4 by LeadingZeros z; z(r)/2.
   **********************************************************************/
  class InversePiProb {
  private:
    template<class Random> bool prob59(Random& r) {
      // true with prob 5/9 = 0.1 000 111 000 111 000 111 ... (binary expansion)
      if (r.Boolean()) return true;
      for (bool res = false; ; res = !res)
        for (int i = 3; i--; ) if (r.Boolean()) return res;
    }

    template<class Random> int geom4(Random& r) { // Geom(1/4)
      int sum = 0;
      while (r.Boolean() && r.Boolean()) ++sum;
      return sum;
    }

    template<class Random> bool binom(Random& r, int n) {
      // Probability of equal heads and tails on 2*n tosses
      // = binomial(2*n, n) / 2^(2*n)
      int d = 0;
      for (int k = n; k--; ) d += r.Boolean() ? 1 : -1;
      for (int k = n; k--; ) {
        d += r.Boolean() ? 1 : -1;
        // This optimization saves 0.1686 bit per call to operator() on average.
        if (std::abs(d) > k) return false;
      }
      return true;
    }

  public:
    /**
     * Return true with probability 1/\e pi.
     *
     * @tparam Random the type of the random generator.
     * @param[in,out] r a random generator.
     * @return true with probability 1/\e pi.
     **********************************************************************/
    template<class Random> bool operator()(Random& r) {
      // Return true with prob 1/pi.
      int n = geom4(r) + geom4(r) + (prob59(r) ? 1 : 0);
      for (int j = 3; j--; ) if (!binom(r, n)) return false;
      return true;
    }
  };

} // namespace RandomLib

#endif  // RANDOMLIB_INVERSEPIPROB_HPP
