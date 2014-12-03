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
            
            size_t count() const throw();
            void append(const string &);
            void append(const char   *);
            
            size_t operator[](const string &) const;
            size_t operator[](const char   *) const;

            
        private:
            const library &lib;
            YOCTO_DISABLE_COPY_AND_ASSIGN(parameters);
            map<string,size_t> db;
        };
        
    }
}

#endif
