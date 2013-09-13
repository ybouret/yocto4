#ifndef YOCTO_GEMS_PROPERTIES_INCLUDED
#define YOCTO_GEMS_PROPERTIES_INCLUDED 1

#include "yocto/gems/types.hpp"
#include "yocto/container/vslot.hpp"
#include "yocto/string.hpp"

#include "yocto/associative/dual-set.hpp"
#include "yocto/counted.hpp"

namespace yocto
{
    namespace gems
    {
        
        class properties : public object, public counted, public vslot
        {
        public:
            const word_t type;
            const string name;
            
            virtual ~properties() throw();
            explicit properties(word_t t, const string &n);
            
            const word_t & key() const throw();
            const string & subkey() const throw();
            
            template <typename T>
            class table : public dual_set<word_t, string, typename T::pointer,hashing::sfh,allocator>
            {
            public:
                typedef dual_set<word_t, string,typename T::pointer,hashing::sfh,allocator> base_type;
                explicit table() throw() : base_type() {}
                virtual ~table() throw() {}
                
                
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(table);
            };

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(properties);
        };
     
        
        
        
    }
    
}



#endif
