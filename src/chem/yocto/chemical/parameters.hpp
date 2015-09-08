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
            typedef map<string,size_t>       index_db;
            typedef index_db::const_iterator iterator;
            
            explicit parameters(const library &user_lib,
                                const char   *names[],
                                const size_t  num_names);
            explicit parameters(const library &user_lib,const array<string> &names);
            virtual ~parameters() throw();
            
            const size_t   count;
            const library &lib;
            const size_t   nvar;
            
            size_t operator[](const string &) const;
            size_t operator[](const char   *) const;
            
            iterator begin() const throw();
            iterator end()   const throw();
            
            friend  std::ostream & operator<<( std::ostream &os, const parameters &);
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(parameters);
            index_db db;

            void __create(const string &id,const size_t ii);
            
        };
        
    }
}

#endif
