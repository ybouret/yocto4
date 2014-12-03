#ifndef YOCTO_CHEMICAL_PARAMETERS_INCLUDED
#define YOCTO_CHEMICAL_PARAMETERS_INCLUDED 1

#include "yocto/chemical/library.hpp"
#include "yocto/associative/map.hpp"

namespace yocto
{
    namespace chemical
    {
        
        
        class parameters
        {
        public:
            explicit parameters(const library &user_lib);
            virtual ~parameters() throw();
            
            const  size_t  nvar;
            
            size_t count() const throw();
            
        private:
            const library &lib;
            YOCTO_DISABLE_COPY_AND_ASSIGN(parameters);
            map<string,size_t> db;
        };
        
    }
}

#endif
