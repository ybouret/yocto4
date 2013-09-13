#ifndef YOCTO_GEMS_ATOM_INCLUDED
#define YOCTO_GEMS_ATOM_INCLUDED 1

#include "yocto/math/v3d.hpp"
#include "yocto/gems/identifier.hpp"
#include "yocto/gems/properties.hpp"
#include "yocto/sequence/vector.hpp"

namespace yocto
{
    namespace gems
    {
        template <typename>
        class residue;
        
        using namespace math;
        
        template <typename T>
        class atom : public identifier
        {
        public:
            typedef residue<T>                         residue_type;
            
            v3d<T>            r;
            v3d<T>            v;
            v3d<T>            a;
            const T           m; //!< mass
            const T           w; //!< 1/mass
            
            explicit atom( word_t u, word_t t) throw();
            virtual ~atom() throw();
            void     set_mass(T mass) throw();
            
            
        private:
            atom& operator=(const atom &);
            atom(const atom &other) throw();
            
        };
        
        
        
    }
    
}

#endif
