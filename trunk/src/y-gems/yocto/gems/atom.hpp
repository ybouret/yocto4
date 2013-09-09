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
            typedef intrusive_ptr<word_t,atom>         pointer;
            typedef vector<pointer,allocator>          group;
            typedef residue<T>                         residue_type;
            typedef intrusive_ptr<word_t,residue_type> residue_ptr;
            
            v3d<T>            r;
            v3d<T>            v;
            v3d<T>            a;
            const T           m; //!< mass
            const T           w; //!< 1/mass
            const residue_ptr parent;
            
            explicit atom(const residue_ptr &res, word_t u, word_t t) throw();
            virtual ~atom() throw();
            void     set_mass(T mass) throw();
            
            class properties : public gems::properties
            {
            public:
                typedef intrusive_ptr<word_t,properties> pointer;
                const T mass;
                explicit properties(word_t t, const string &n, T m );
                virtual ~properties() throw();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(properties);
            };
            
        private:
            atom& operator=(const atom &);
            atom(const atom &other) throw();
            
        };
        
        
        
    }
    
}

#endif
