#ifndef YOCTO_AQUEOUS_SOLUTION_INCLUDED
#define YOCTO_AQUEOUS_SOLUTION_INCLUDED 1

#include "yocto/aqueous/library.hpp"

namespace yocto 
{
    namespace aqueous
    {
        
        class component
        {
        public:
            explicit component( const string &id );
            virtual ~component() throw();
            component( const component & );
            
            const string name; //!< identifier
            double       C;    //!< concentration
            
            const string & key() const throw();
            typedef set<string,component> db;
            typedef db::iterator          iterator;
            typedef db::const_iterator    const_iterator;
            
        private:
            YOCTO_DISABLE_ASSIGN(component);
        };
        
        class solution
        {
        public:
            explicit solution( const library & );
            virtual ~solution() throw();
            solution( const solution &s );
            
            double &       operator[]( const string &name );
            const double & operator[]( const string &name ) const;
            double &       operator[]( const char *name );
            const double & operator[]( const char *name ) const;
            
            double sum_zC() const throw();
            
            void put( array<double> &vec ) const throw();
            void get( const array<double> &vec ) throw();
            
            const library  &lib;
            
        private:
            component::db   components;
            YOCTO_DISABLE_ASSIGN(solution);
            
        public:
            const size_t size;
            
        };
        
    }
    
}

#endif
