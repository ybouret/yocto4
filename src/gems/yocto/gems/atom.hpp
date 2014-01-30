#ifndef YOCTO_GEMS_ATOM_INCLUDED
#define YOCTO_GEMS_ATOM_INCLUDED 1

#include "yocto/math/v3d.hpp"
#include "yocto/gems/identifier.hpp"
#include "yocto/gems/properties.hpp"
#include "yocto/ordered/sorted-vector.hpp"
#include "yocto/ptr/intr.hpp"
#include "yocto/ptr/nosy.hpp"

namespace yocto
{
    namespace gems
    {
        template <typename>
        class residue;
        
        using namespace math;
        
        //! a shared atom
        template <typename T>
        class atom : public identifier
        {
        public:
            typedef intr_ptr<word_t,atom> pointer;
            
            class pointer_comparator
            {
            public:
                inline  pointer_comparator() throw() {}
                inline ~pointer_comparator() throw() {}
                inline  int operator()( const pointer &lhs, const pointer &rhs) const throw()
                {
                    return __compare<word_t>(lhs->uuid,rhs->uuid);
                }
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(pointer_comparator);
            };

            typedef sorted_vector<pointer,pointer_comparator,allocator>  group;
            
            v3d<T>                r;
            v3d<T>                v;
            v3d<T>                a;
            const T               m; //!< mass
            const T               w; //!< 1/mass
            
            explicit atom( const residue<T> &from, word_t u, word_t t) throw();
            virtual ~atom() throw();
            void     set_mass(T mass) throw();
            
            //! atom properties for library
            class properties : public gems::properties
            {
            public:
                const T mass;
                
                typedef nosy_ptr<word_t,string,properties>  pointer;
                typedef gems::properties::table<properties> table;
                
                explicit properties( word_t t, const string &n, T m );
                virtual ~properties() throw();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(properties);
            };
            
                       
        private:
            const residue<T> *pp;
            
            atom& operator=(const atom &);
            atom(const atom &other) throw();
            
        };
        
        
        
    }
    
}

#endif
