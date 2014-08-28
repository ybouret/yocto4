#ifndef YOCTO_CHEMICAL_SOLUTION_INCLUDED
#define YOCTO_CHEMICAL_SOLUTION_INCLUDED 1

#include "yocto/chemical/collection.hpp"
#include "yocto/ios/ostream.hpp"

namespace yocto
{
    
    namespace chemical
    {
        //! for a solution
        class component
        {
        public:
            component( const species::ptr &sp ) throw();
            ~component() throw();
            const species::ptr spec; //!< from a collection
            double             concentration; //!< the real concentration
            
            const string &key() const throw(); //!< spec->name
            
            component(const component &other ) throw();
            
            typedef set<string,component> db;
            
        private:
            YOCTO_DISABLE_ASSIGN(component);
        };
        
        class equilibria;
        
        //! built from a collection
        class solution
        {
        public:
            explicit solution( const collection &lib );
            solution(const solution &other);
            virtual ~solution() throw();
            
            //! number of components..
            const size_t components;
            
            //! for debug: same components, same order
            bool has_same_components_than( const solution &other ) const throw();
            
            double & operator[]( const string & );             //!< get by bame
            const double & operator[]( const string &) const;  //!< get by name
            
            double & operator[]( const char * );               //!< get by name
            const double &operator[]( const char *) const;     //!< get by name
            
            double       & operator[]( size_t i ) throw();      //!< get by index
            const double & operator[]( size_t i ) const throw();//!< get by index

            void load( const array<double> &C ) throw();        //!< first components into slots
            void save( array<double> &C) const throw();         //!< first components into slots
            
            void mul( double a ) throw();      //!< multiply all
            void add( const solution &other ); //!< must be compatible (not necessarily same components)
            
            double pH() const; //!< get pH if any
            
            friend std::ostream & operator<<( std::ostream &, const solution &);
            
            typedef component::db::iterator       iterator;
            typedef component::db::const_iterator const_iterator;
            
            iterator       begin() throw();
            iterator       end()   throw();
            const_iterator begin() const throw();
            const_iterator end()   const throw();
            
            void   ldz() throw();          //!< everyone to zero
            double sum_zC() const throw(); //!< charge excess, in concentration units
            
            void write_header( ios::ostream & ) const;
            void write_values( ios::ostream & ) const;
            void mix( equilibria &cs, const array<solution> &solutions, const array<double> &weights, double t);
            
            component *lookup( const string & ) throw();
            
        private:
            component::db composition;
            const size_t  name_max;
            YOCTO_DISABLE_ASSIGN(solution);
            
        };
        
    }
    
}

#endif
