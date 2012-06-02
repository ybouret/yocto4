#ifndef YOCTO_AQ_EAUILIBRIUM_INCLUDED
#define YOCTO_AQ_EAUILIBRIUM_INCLUDED 1

#include "yocto/aqueous/solution.hpp"
#include "yocto/functor.hpp"

namespace yocto 
{
    
    namespace aqueous
    {
        
        class actor
        {
        public:
            actor( const string &id, int stochio );
            ~actor() throw();
            actor( const actor &);
            const string & key() const throw();
            
            const string name;
            const int    nu;
            
            typedef set<string,actor> db;
            
        private:
            YOCTO_DISABLE_ASSIGN(actor);
        };
        
        class equilibrium : public counted, public object
        {
        public:
            typedef functor<double,TL1(double)> constant;
            
            explicit equilibrium( const string &id, const constant &eqK);
            explicit equilibrium( const string &id, double eqK );
            virtual ~equilibrium() throw();
            
            const string & key() const throw();
            
            const string name;
            constant     K;
            
            typedef intrusive_ptr<string,equilibrium> ptr;
            typedef set<string,ptr>                   db;
            
            void add( const string &name, int nu );
           
        private:
            actor::db    actors;
            actor::db    reactants;
            actor::db    products;
            const double tmpK;
            double       getK(double) const throw();
            YOCTO_DISABLE_COPY_AND_ASSIGN(equilibrium);
        };
        
       
        
    }
    
}

#endif
