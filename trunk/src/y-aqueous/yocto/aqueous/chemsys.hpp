#ifndef YOCTO_AQ_CHEMSYS_INCLUDED
#define YOCTO_AQ_CHEMSYS_INCLUDED 1

#include "yocto/aqueous/equilibrium.hpp"
#include "yocto/math/kernel/lu.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/math/fcn/derivative.hpp"

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
            
            equilibrium & create( const char *name, const equilibrium::constant &K );
            equilibrium & create( const char *name, double K );
            
            
            void build();
            const double       ftol;    //!< fractional tolerance
            matrix<double>     nu;      //! topology matrix [NxM]
            matrix<ptrdiff_t>  nuR;     //!< for reactants  [NxM]
            matrix<ptrdiff_t>  nuP;     //!< for products   [NxM]
            vector<double>     K;       //!< local constants evaluation [N]
            vector<double>     Gamma;   //!< local Gamma evaluation     [N]
            vector<double>     dtGam;   //!< local d_Gamma/d_t          [N]
            vector<double>     C;       //!< copy of solution content   [M]
            matrix<double>     Phi;     //!< dGamma/dC                  [NxM]
            matrix<double>     W;       //!< (Phi * nu')^(-1)           [NxM]
            lu<double>         LU;      //!< solver                     [N]
            vector<double>     xi;      //!< local extent               [N]
            vector<double>     dC;      //!< local modification         [M]
            derivative<double> drvs;    //!< local derivative context
            double             t_scale; //!< derivative time scale, default=1e-4
            
            //! compute Gamma for a given C at time t
            void computeGammaAndPhi( double t, bool computeDerivative );
            
            //! compute W for a given C at time t
            /**
             call computeGammaAndPhi.
             */
            void computeW( double t, bool computeDerivative);
            
            //! normalize a loaded C
            /**
             advance all equilibria
             */
            void normalize( double t );
            
            //! reduce a loaded dC w.r.t a loaded C
            /**
             use the projection.
             */
            void reduce( double t );
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(chemsys);
            
        public:
            const library &lib;
        };
        
        
    }
    
}

#endif
