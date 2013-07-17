#ifndef YOCTO_CHEMICAL_SOLUTION_INCLUDED
#define YOCTO_CHEMICAL_SOLUTION_INCLUDED 1

#include "yocto/chemical/collection.hpp"

namespace yocto
{
    
    namespace chemical
    {
        class component
        {
        public:
            component( const species::ptr &sp ) throw();
            ~component() throw();
            const species::ptr spec;
            double             conc;
            
            const string &key() const throw();
            
            component(const component &other ) throw();
            
            typedef set<string,component> db;
            
        private:
            YOCTO_DISABLE_ASSIGN(component);
        };
        
        class solution
        {
        public:
            explicit solution( const collection &lib );
            virtual ~solution() throw();
            
            const size_t components;
            
            double & operator[]( const string & );
            const double & operator[]( const string &) const;
            
            double & operator[]( const char * );
            const double &operator[]( const char *) const;
            
            double pH() const;
            
        private:
            component::db composition;
            
        };
        
    }
    
}

#endif
