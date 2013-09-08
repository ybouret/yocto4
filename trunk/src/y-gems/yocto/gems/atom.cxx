#include "yocto/math/ztype.hpp"
#include "yocto/gems/residue.hpp"

namespace yocto
{
    namespace gems
    {
        template <>
        atom<real_t>:: ~atom() throw()
        {
        }
        
        template <>
        atom<real_t>:: atom( const residue_p &res, word_t u, word_t t) throw() :
        identifier(u,t), r(),v(),a(),m(0),w(0), parent(res)
        {
        }
        
      
        
        template <>
        void atom<real_t>::set_mass( real_t mass ) throw()
        {
            (real_t &)w = 1/( (real_t &)m = mass );
        }
        
        
        
    }
    
}

