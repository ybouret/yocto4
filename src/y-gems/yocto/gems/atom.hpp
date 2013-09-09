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
            typedef intrusive_ptr<word_t,atom>        ptr;
            typedef vector<ptr,allocator>             group;
            typedef residue<T>                        residue_t;
            typedef intrusive_ptr<word_t,residue_t>   residue_p;
            
            v3d<T>          r;
            v3d<T>          v;
            v3d<T>          a;
            const T         m; //!< mass
            const T         w; //!< 1/mass
            const residue_p parent;
            
            explicit atom(const residue_p &res, word_t u, word_t t) throw();
            virtual ~atom() throw();
            void     set_mass(T mass) throw();
            
            class properties : public gems::properties
            {
            public:
                typedef intrusive_ptr<word_t,properties> ptr;
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
