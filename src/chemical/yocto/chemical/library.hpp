#ifndef YOCTO_CHEMICAL_LIBRARY_INCLUDED
#define YOCTO_CHEMICAL_LIBRARY_INCLUDED 1


#include "yocto/chemical/group.hpp"
#include "yocto/lang/parser.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace chemical
    {
        
        typedef lang::syntax::xnode xnode;
        
        
        
        class library : public object
        {
        public:
            explicit library();
            virtual ~library() throw();
            
            const size_t max_name_length;
            
            xnode *parse_species(ios::istream &fp);
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(library);
            auto_ptr<lang::parser> species_parser;
        };
    }
}

#endif

