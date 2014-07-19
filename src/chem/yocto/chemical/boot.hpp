#ifndef YOCTO_CHEMICAL_BOOT_INCLUDED
#define YOCTO_CHEMICAL_BOOT_INCLUDED 1

#include "yocto/chemical/equilibria.hpp"

namespace yocto
{
    namespace chemical
    {
        
        class boot
        {
        public:
            
            //! a species and its coefficient
            class item
            {
            public:
                const species::pointer spec;
                const double           coef;
                explicit item( const species::pointer &s, const double w) throw();
                virtual ~item() throw();
                item(const item &other) throw();
                const string &key() const throw();
                
            private:
                YOCTO_DISABLE_ASSIGN(item);
            };
            
            
            
            explicit boot();
            virtual ~boot() throw();
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(boot);
            
        };
        
    }
}

#endif
