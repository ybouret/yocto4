#ifndef YOCTO_AQ_EAUILIBRIUM_INCLUDED
#define YOCTO_AQ_EAUILIBRIUM_INCLUDED 1

#include "yocto/aqueous/solution.hpp"
#include "yocto/functor.hpp"

namespace yocto 
{
    
    namespace aqueous
    {
        
        //! equilibrium actor: name + stochiometric coefficient
        class actor
        {
        public:
            actor( const string &id, int stochio );
            ~actor() throw();
            actor( const actor &);
            const string & key() const throw();
            
            const string name; //!< identifier for library
            const int    nu;   //!< its algebraic stochiometric coefficient
            
            typedef set<string,actor> db;
            
        private:
            YOCTO_DISABLE_ASSIGN(actor);
        };
        
        //! equilibirum
        class equilibrium : public counted, public object
        {
        public:
            typedef functor<double,TL1(double)> constant;
            
            explicit equilibrium( const library &L, const string &id, const constant &eqK);
            explicit equilibrium( const library &L, const string &id, double eqK );
            virtual ~equilibrium() throw();
            
            const string & key() const throw();
            
            const string     name;
            mutable constant K;
            
            typedef intr_ptr<string,equilibrium> ptr;
            typedef set<string,ptr>              db;
            
            void add( const string &id, int nu );
            void add( const char   *id, int nu );
            
            double Gamma( const solution &s, double t ) const;
            
            //! fill topologies matrices
            void fill( array<double> &row_nu, array<ptrdiff_t> &row_nuR, array<ptrdiff_t> &row_nuP ) const throw();
            
            friend std::ostream & operator<<( std::ostream &, const equilibrium & );
            
        private:
            actor::db      actors;
            actor::db      reactants;
            actor::db      products;
            const double   tmpK;
                        
            double       getK(double) const throw();
            YOCTO_DISABLE_COPY_AND_ASSIGN(equilibrium);
        public:
            const library &lib;
        };
        
        typedef equilibrium::db equilibria;
       
        
    }
    
}

#endif
