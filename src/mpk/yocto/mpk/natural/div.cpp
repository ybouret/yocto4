#include "yocto/mpk/word2mpn.hpp"
#include "yocto/exceptions.hpp"
#include <cerrno>

namespace yocto
{

    namespace mpk
    {
        natural  natural::div( const natural &num, const natural &den )
        {
            //__________________________________________________________________
            //
            // check
            //__________________________________________________________________
            if(den.size<=0)
            {
                throw libc::exception( EDOM, "mpk: division by zero");
            }

            const int cmp = natural::compare(num,den);
            if(cmp<0)
            {
                //______________________________________________________________
                //
                //
                // num < den
                //
                //______________________________________________________________
                return natural(); // zero
            }
            else
            {
                if(cmp>0)
                {
                    //__________________________________________________________
                    //
                    //
                    // generic num > den (!=0)
                    //
                    //__________________________________________________________


                    //__________________________________________________________
                    //
                    // bracket result
                    //__________________________________________________________
                    size_t p = 1;
                    {
                        natural probe = add(den,den); //! 2^(p=1) * den
                        while( compare(probe,num) <= 0 )
                        {
                            ++p;
                            probe.shl();
                        }
                        assert( probe > num );
                    }

                    natural hi = natural::exp2(   p );
                    natural lo = natural::exp2( --p );

                    //__________________________________________________________
                    //
                    // bissection
                    //__________________________________________________________
                    while( p-- > 0 )
                    {

                        natural       mid = add( lo, hi );
                        const natural tmp = mul( mid.shr(), den );
                        const int     chk = natural::compare(tmp,num);
                        if(chk<0)
                        {
                            lo.xch(mid);
                        }
                        else
                        {
                            if(chk>0)
                            {
                                hi.xch(mid);
                            }
                            else
                            {
                                return mid; //!< special case
                            }
                        }
                    }
                    return lo;
                }
                else
                {
                    //__________________________________________________________
                    //
                    //
                    // special case num == den
                    //
                    //__________________________________________________________
                    return natural(1);
                }
            }

        }
        
        YOCTO_MPN_IMPL2(natural,operator/,natural::div)
        natural & natural::operator/=(const word_t den)
        {
            const word2mpn DEN(den);
            natural ans = div(*this,DEN.n);
            xch(ans);
            return *this;
        }

    }

}
