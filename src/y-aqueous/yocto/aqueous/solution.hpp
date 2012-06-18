#ifndef YOCTO_AQUEOUS_SOLUTION_INCLUDED
#define YOCTO_AQUEOUS_SOLUTION_INCLUDED 1

#include "yocto/aqueous/library.hpp"
#include "yocto/ios/ostream.hpp"

namespace yocto 
{
    namespace aqueous
    {
        
        //! component for a solution
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
        
        
        //! database of components
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
            
            double       & operator()( size_t i) throw();
            const double & operator()(size_t i) const throw();
            
            double sum_zC() const throw();
            
            void put( array<double> &vec ) const throw();
            void get( const array<double> &vec ) throw();
            
            const library  &lib;
            friend std::ostream & operator<<( std::ostream &, const solution & );
            
            void mul( double a ) throw(); //!< rescale all
            void ldz() throw(); //!< set to zero
            
            double pH() const;
            
            component::iterator       begin();
            component::iterator       end();
            component::const_iterator begin() const;
            component::const_iterator end() const;
            
            void save_header( ios::ostream &fp ) const;
            void save_values( ios::ostream &fp ) const;
            
            void copy( const solution &other );
            
        private:
            component::db   components;
            YOCTO_DISABLE_ASSIGN(solution);
            
        public:
            const size_t size;
            
        };
        
    }
    
}

#endif
