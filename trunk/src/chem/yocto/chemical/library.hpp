#ifndef YOCTO_CHEMICAL_LIBRARY_INCLUDED
#define YOCTO_CHEMICAL_LIBRARY_INCLUDED 1

#include "yocto/chemical/species.hpp"
#include <iostream>

namespace yocto
{
    namespace chemical
    {
        
        //! a library of species
        /**
         counted is used by solution
         */
        class library : public counted
        {
        public:
            typedef species::database       db_type;
            typedef db_type::iterator       iterator;
            typedef db_type::const_iterator const_iterator;
            
            explicit library() throw();
            virtual ~library() throw();
            size_t size() const throw(); //!< #species
            
            species::pointer       &operator[](const string &id);
            const species::pointer &operator[](const string &id) const;
            species::pointer       &operator[](const char   *id);
            const species::pointer &operator[](const char   *id) const;
            
            species & add(const string &name, const int z);
            species & add(const char   *name, const int z);
            
            void      remove(const string &id);
            void      remove(const char   *id);
            
            iterator begin() throw();
            iterator end()   throw();
            
            const_iterator begin() const throw();
            const_iterator end()   const throw();
            
            void output(std::ostream &os) const;
            
            friend inline std::ostream & operator<<( std::ostream &os, const library &lib )
            {
                lib.output(os);
                return os;
            }
            
            bool has(const string &name) const throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(library);
            species::database db;
            size_t            max_name_length;
            void   find_max_name_length() throw();
            
        };
        
    }
}


#endif

