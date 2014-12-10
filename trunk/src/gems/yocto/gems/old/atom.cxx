#include "yocto/math/ztype.hpp"
#include "yocto/gems/atom.hpp"
#include "yocto/exceptions.hpp"
#include <cerrno>

namespace yocto
{
    namespace gems
    {
        template <>
        atom<real_t>:: ~atom() throw()
        {
        }
       
        template <>
        atom<real_t>:: atom( const residue<real_t> &from, word_t u, word_t t) throw() :
        identifier(u,t), r(),v(),a(),m(0),w(0), pp(&from)
        {
        }
        
        
        template <>
        void atom<real_t>::set_mass( real_t mass ) throw()
        {
            (real_t &)w = 1/( (real_t &)m = mass );
        }
        
        template <>
        atom<real_t>::properties:: ~properties() throw() {}
        
        template <>
        atom<real_t>::properties:: properties(word_t        t,
                                              const string &n,
                                              real_t        m ) :
        gems::properties(t,n),
        mass(m)
        {
            if(mass<=0)
                throw libc::exception( ERANGE, "negative mass for '%s'", name.c_str() );
        }
        
        template <>
        const char atom<real_t>::properties::table::Label[] = "atom";
        
        
    }
    
}

