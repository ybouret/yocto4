#include "yocto/mpl/natural.hpp"
#include "yocto/exceptions.hpp"
#include <cerrno>

namespace yocto
{
    namespace mpl
    {

        natural natural:: __mod(const void *num, const size_t nn, const void *den, const size_t nd)
        {
            assert(nd>0);
            const int cmp = natural::compare(num, nn, den, nd);
            if(cmp<0)
            {
                //______________________________________________________________
                //
                //
                // special case: num < den => num
                //
                //______________________________________________________________
                natural ans(nn,as_capacity);
                memcpy(ans.byte,num,nn);
                YOCTO_CHECK_MPN(ans);
                return ans;
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
                                return natural(); // special case, 0
                            }
                        }
                    }
                    // lo is the quotient
                    const natural qd = mul( lo.byte, lo.size, den, nd);
                    assert( compare(qd.byte,qd.size,num,nn) <= 0 );
                    return sub(num,nn,qd.byte,qd.size);
                }
                else
                {
                    //__________________________________________________________
                    //
                    //
                    // special case num == den
                    //
                    //__________________________________________________________
                    return natural();
                }
                
            }
        }

        natural natural::modulo(const void *num, const size_t nn, const void *den, const size_t nd)
        {
            if(nd<=0)
            {
                throw libc::exception(EINVAL,"mpl: modulo by zero");
            }
            return __mod(num, nn, den, nd);
        }


    }

}