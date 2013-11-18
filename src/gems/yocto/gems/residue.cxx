#include "yocto/math/ztype.hpp"
#include "yocto/gems/residue.hpp"

namespace yocto
{
    namespace gems
    {
        
        template <>
        residue<real_t>:: ~residue() throw()
        {
            
        }
        
        template <>
        residue<real_t>:: residue( word_t u, word_t t) throw() :
        identifier(u,t)
        {
        }
        
        template <>
        residue<real_t>:: properties:: ~properties() throw() {}
        
        template <>
        residue<real_t>:: properties:: properties(word_t        t,
                                                  const string &n) :
        gems::properties(t,n)
        {
        }
        
        template <>
        const char residue<real_t>::properties::table::Label[] = "residue";
        
    }
    
}
