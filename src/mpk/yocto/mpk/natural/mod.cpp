#include "yocto/mpk/word2mpn.hpp"
#include "yocto/exceptions.hpp"
#include <cerrno>

namespace yocto
{
    namespace mpk
    {


        
        YOCTO_MPN_IMPL2(natural,operator%,natural::modulo)

        natural natural::modulo(const natural &num, const natural &den)
        {
            if(den.size<=0)
            {
                throw libc::exception(EDOM,"mpk: modulo division by zero");
            }
            const int cmp = natural::compare(num,den);
            if(cmp<0)
            {
                //______________________________________________________________
                //
                // trivial case
                //______________________________________________________________
                return num;
            }
            else
            {
                if(cmp>0)
                {
                    //__________________________________________________________
                    //
                    // generic case
                    //__________________________________________________________

                    //__________________________________________________________
                    //
                    // bracket quotient
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
                    // find quotient
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
                                lo.xch(mid);
                                break;
                            }
                        }
                    }

                    //__________________________________________________________
                    //
                    // lo is the quotient
                    //__________________________________________________________
                    const natural qd = mul(lo,den); assert(qd<=num);
                    return sub(num,qd);
                }
                else
                {
                    //__________________________________________________________
                    //
                    // special case
                    //__________________________________________________________
                    return natural(0);
                }
            }
        }


        natural & natural::operator%=(const word_t den)
        {
            const word2mpn DEN(den);
            natural ans = modulo(*this,DEN.n);
            xch(ans);
            return *this;
        }

        void natural:: split(natural &q, natural &r,const natural &num, const natural &den)
        {
            if(den.size<=0)
            {
                throw libc::exception(EDOM,"mpk: modulo division by zero");
            }
            const int cmp = natural::compare(num,den);
            if(cmp<0)
            {
                // trivial case
                q = 0;
                r = num;
                assert(num == q*den+r);
            }
            else
            {
                if(cmp>0)
                {
                    // generic case
                    //__________________________________________________________
                    //
                    // bracket quotient
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
                    // find quotient
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
                                lo.xch(mid);
                                break;
                            }
                        }
                    }

                    //__________________________________________________________
                    //
                    // lo is the quotient
                    //__________________________________________________________
                    lo.xch(q);
                    const natural qd = mul(q,den); assert(qd<=num);
                    r = sub(num,qd);
                    assert(num == q*den+r);
                }
                else
                {
                    // special case
                    q = 1;
                    r = 0;
                    assert(num == q*den+r);
                }
            }
        }


    }
}
