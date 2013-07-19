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
            solution( const solution &other);
            virtual ~solution() throw();
            
            const size_t components;
            
            double & operator[]( const string & );
            const double & operator[]( const string &) const;
            
            double & operator[]( const char * );
            const double &operator[]( const char *) const;
            
            void load( const array<double> &C ) throw();
            void save( array<double> &C) const throw();
            
            double pH() const;
            
            friend std::ostream & operator<<( std::ostream &, const solution &);
            
            typedef component::db::iterator       iterator;
            typedef component::db::const_iterator const_iterator;
            
            iterator begin() throw();
            iterator end()   throw();
            const_iterator begin() const throw();
            const_iterator end()   const throw();
            
            void   ldz() throw();
            double sum_zC() const throw(); //!< charge excess, in concentration units
            
        private:
            component::db composition;
            const size_t  name_max;
            YOCTO_DISABLE_ASSIGN(solution);
        };
        
    }
    
}

#endif
