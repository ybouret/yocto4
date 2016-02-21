#ifndef YOCTO_MPL_RATIONAL_INCLUDED
#define YOCTO_MPL_RATIONAL_INCLUDED 1

#include "yocto/mpl/integer.hpp"

namespace yocto
{
    namespace mpl
    {
        
        class rational : public object
        {
        public:
            const integer num;
            const integer den;
            
            virtual ~rational() throw() {}
            inline   rational() : num(0), den(1) {}
            inline   rational(const rational &r) : num(r.num), den(r.den) {}
            
            inline void xch(rational &r) throw()
            {
                ( (integer &)num ).xch( (integer &)(r.num) );
                ( (integer &)den ).xch( (integer &)(r.den) );
            }
            
            inline rational & operator=(const rational &r)
            {
                rational q(r);
                xch(q);
                return *this;
            }
            
            
        };
    }
}

#endif

