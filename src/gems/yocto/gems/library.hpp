#ifndef YOCTO_GEMS_LIBRARY_INCLUDED
#define YOCTO_GEMS_LIBRARY_INCLUDED 1

#include "yocto/gems/properties.hpp"

namespace yocto
{
    namespace gems
    {


        class library
        {
        public:
            explicit library();
            virtual ~library() throw();

            properties & insert( const string &name, const double mass);

        private:
            properties::database db;
            bool                 locked;
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(library);
        };
        
        
    }
    
    
}

#endif
