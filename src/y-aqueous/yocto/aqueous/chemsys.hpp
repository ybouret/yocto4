#ifndef YOCTO_AQ_CHEMSYS_INCLUDED
#define YOCTO_AQ_CHEMSYS_INCLUDED 1

#include "yocto/aqueous/equilibrium.hpp"
#include "yocto/math/kernel/linsys.hpp"
#include "yocto/sequence/vector.hpp"


namespace yocto 
{
    namespace aqueous
    {
        using namespace math;
        
        class chemsys : public equilibria
        {
        public:
            explicit chemsys( const library &L, double frac_tol ) throw();
            virtual ~chemsys() throw();

            equilibrium & create( const string &name, const equilibrium::constant &K );
            equilibrium & create( const string &name, double K );
            
            void build();
            
            const double      ftol;   //!< fractional tolerance
            matrix<double>    nu;     //! topology matrix [NxM]
            matrix<ptrdiff_t> nuR;    //!< for reactants  [NxM]
            matrix<ptrdiff_t> nuP;    //!< for products   [NxM]
            vector<double>    K;      //!< local constants evaluation [N]
            vector<double>    Gamma;  //!< local Gamma evaluation     [N]
            vector<double>    C;      //!< copy of solution content   [M]
            matrix<double>    Phi;    //!< dGamma/dC                  [NxM]
            matrix<double>    W;      //!< (Phi * nu')^(-1)           [NxM]
            linsys<double>    solver; //!< to solve system            [N]
            vector<double>    xi;     //!< local extent               [N]
            vector<double>    dC;     //!< local modification         [M]
            
            //! compute W for a given C
            void computeW( double t);
            
            //! normalize C
            void normalize( double t );
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(chemsys);

        public:
            const library &lib;
        };
        
        
    }
    
}

#endif
