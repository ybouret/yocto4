#ifndef YOCTO_AQ_CHEMSYS_INCLUDED
#define YOCTO_AQ_CHEMSYS_INCLUDED 1

#include "yocto/aqueous/equilibrium.hpp"
#include "yocto/math/kernel/matrix.hpp"
#include "yocto/sequence/vector.hpp"


namespace yocto 
{
    namespace aqueous
    {
        using namespace math;
        
        class chemsys : public equilibria
        {
        public:
            explicit chemsys( const library &L ) throw();
            virtual ~chemsys() throw();

            equilibrium & create( const string &name, const equilibrium::constant &K );
            equilibrium & create( const string &name, double K );
            
            void build();
            
            matrix<double>    nu;    //! topology matrix
            matrix<ptrdiff_t> nuR;   //!< for reactants
            matrix<ptrdiff_t> nuP;   //!< for products
            vector<double>    K;     //!< local constants evaluation
            vector<double>    Gamma; //!< local Gamma evaluation
            vector<double>    C;     //!< copy of solution content
            
            void computeGamma( const solution &s, double t);
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(chemsys);

        public:
            const library &lib;
        };
        
        
    }
    
}

#endif
