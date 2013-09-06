#include "yocto/math/ztype.hpp"
#include "yocto/gems/atom.hpp"

namespace yocto
{
    namespace gems
    {
        template <>
        atom<real_t>:: ~atom() throw()
        {
        }
        
        template <>
        atom<real_t>:: atom( word_t u, word_t t) throw() :
        uuid(u), type(t), r(),v(),a(),m(0),w(0)
        {
        }
        
      
        
        template <>
        void atom<real_t>::set_mass( real_t mass ) throw()
        {
            (real_t &)w = 1/( (real_t &)m = mass );
        }
        
        template <>
        const word_t & atom<real_t>:: key() const throw()
        {
            return uuid;
        }

        
    }
    
}

