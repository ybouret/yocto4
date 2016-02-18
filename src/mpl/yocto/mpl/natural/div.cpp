#include "yocto/mpl/natural.hpp"
#include "yocto/exceptions.hpp"
#include <cerrno>

namespace yocto
{
    namespace mpl
    {
        natural natural:: div(const void *num, const size_t nn,
                              const void *den, const size_t nd)
        {
            if(nd<=0)
            {
                throw libc::exception(EINVAL,"mpl: division by zero");
            }

            const int cmp = natural::compare(num, nn, den, nd);
            if(cmp<0)
            {
                //______________________________________________________________
                //
                //
                // special case: num < den => 0
                //
                //______________________________________________________________
                return natural();
            }
            else
            {
                if(cmp>0)
                {
                    //__________________________________________________________
                    //
                    //
                    // generic case num > den
                    //
                    //__________________________________________________________


                    //__________________________________________________________
                    //
                    // bracket result
                    //__________________________________________________________
                    size_t p = 1;
                    {
                        natural probe = add(den,nd,den,nd); //! 2^(p=1) * den
                        while( compare(probe.byte,probe.size,num,nn) <= 0 )
                        {
                            ++p;
                            probe.shl();
                        }
                        assert(  compare(probe.byte,probe.size,num,nn) >= 0 );
                    }

                    natural hi = natural::exp2(   p );
                    natural lo = natural::exp2( --p );

                    //__________________________________________________________
                    //
                    // bissection
                    //__________________________________________________________
                    while( p-- > 0 )
                    {

                        natural       mid = lo+hi;
                        (void)mid.shr();
                        const natural tmp = mul( mid.byte, mid.size, den, nd);
                        const int     chk = natural::compare(tmp.byte,tmp.size,num,nn);
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
    }
}
