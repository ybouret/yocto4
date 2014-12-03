#include "yocto/chemical/library.hpp"
#include "yocto/exception.hpp"

#include <cmath>

namespace yocto
{
    namespace chemical
    {
        
        double library:: osmolarity( const array<double> &C ) const
        {
            assert(C.size()>=db.size());
            double ans = 0;
            for(size_t i=db.size();i>0;--i)
            {
                ans += C[i];
            }
            return ans;
        }
        
        double library:: charge(const array<double> &C) const
        {
            assert(C.size()>=db.size());
            const size_t   n = db.size();
            const_iterator j = db.begin();
            double         ans = 0;
            for(size_t i=1;i<=n;++i,++j)
            {
                const int z = (**j).z;
                ans += z*C[i];
            }
            return ans;
        }

        double library:: ionic_strength(const array<double> &C) const
        {
            assert(C.size()>=db.size());
            const size_t   n = db.size();
            const_iterator j = db.begin();
            double         ans = 0;
            for(size_t i=1;i<=n;++i,++j)
            {
                const int z = (**j).z;
                ans += (z*z)*C[i];
            }
            return 0.5*ans;
        }

        
        double library:: pH(const array<double> &C) const
        {
            assert(C.size()>=db.size());
            const species::pointer *p = db.search(_proton);
            if(!p) throw exception("pH: no proton registered in library");
            const size_t ip = (**p).indx;
            assert(ip>0);
            assert(ip<=C.size());
            return -log10( C[ip] );
        }

        
    }
    
}

