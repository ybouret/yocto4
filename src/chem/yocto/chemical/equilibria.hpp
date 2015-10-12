#ifndef YOCTO_CHEMICAL_EQUILIBRIA_INCLUDED
#define YOCTO_CHEMICAL_EQUILIBRIA_INCLUDED 1

#include "yocto/chemical/equilibrium.hpp"
#include "yocto/chemical/library.hpp"
#include "yocto/chemical/ode.hpp"

#include "yocto/math/matrix.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/ordered/sorted-vector.hpp"
#include "yocto/code/rand32.hpp"

namespace yocto
{
    namespace chemical
    {
        
        typedef math::matrix<double>         matrix_t;
        typedef vector<double>               vector_t;
        
        typedef vector<bool>                 bvector_t;
        typedef vector<size_t>               uvector_t;
        typedef math::matrix<ptrdiff_t>      imatrix_t;
        typedef vector<equilibrium::pointer> evector_t;
        typedef vector<ptrdiff_t>            ivector_t;
        
        
        class boot;
        
        class equilibria
        {
        public:
            explicit equilibria();
            virtual ~equilibria() throw();
            
            
            const size_t M; //!< compiled #species
            const size_t N; //!< compiled #equilibria
            
            vector_t     C;      //!< [M] local conc
            vector_t     dC;     //!< [M] local dC
            bvector_t    active; //!< [M] reactive species
            vector_t     Cs;     //!< [M] starting point for probing
            uvector_t    Jneg;   //!< [0..M] indices of negative concentration
            ivector_t    beta;   //!< [M]    balancing: dE/dC
            ivector_t    dCp;    //!< [M]    balancing: integer descent direction
            vector_t     alpha;  //!< [0..M] balancing: max scaling
            uvector_t    aindx;  //!< [0..M] balancing: associated #species
            vector_t     Xstar;  //!< [M] for booting
            bvector_t    aboot;  //!< [M] active for booting
            uvector_t    fixedJ; //!< [0..M] booting: fixed indices
            vector_t     fixedC; //!< [0..M] booting: fixed concentrations
            
            vector_t     K;      //!< [N] constants
            vector_t     Gamma;  //!< [N] Gamma
            vector_t     GamSF;  //!< [N] Gamma Scaling Factor
            vector_t     dtGam;  //!< [N] dGamma/dt
            vector_t     xi;     //!< [N] extents
            imatrix_t    Nu;     //!< [NxM] current topology
            matrix_t     Phi;    //!< [NxM] dGamma/dC
            matrix_t     W;      //!< [NxN]
            evector_t    online; //!< [0..N] balancing: online eqs for balancing
            ivector_t    xip;    //!< [N]    balancing: integer descent extent
            imatrix_t    Q;      //!< [NxM]  booting
            integer_t    Delta;  //!< C=Xstar/Delta + Q'*V
            vector_t     xis;    //!< [N], xi start...
            vector_t     U;      //!< [N] xi increment for booting
            
            //__________________________________________________________________
            //
            // Building API
            //__________________________________________________________________
            
            equilibrium &add( equilibrium *pEq );
            void         remove(const string &name);
            
            void compile_for( const library &lib );
            void restore_topology();
            
            void clear() throw();
            
            void output( std::ostream &os ) const;
            friend inline std::ostream & operator<<( std::ostream &os, const equilibria &eqs )
            {
                eqs.output(os);
                return os;
            }
            
            //__________________________________________________________________
            //
            // Chemistry API
            //__________________________________________________________________
            
            //! compute all limits for equilibria
            void compute_limits() throw();
            
            //! balance concentration
            bool balance( array<double> &C0 );
            void copy_to( array<double> &C0 ) const throw();
            
            equilibrium       & operator[](size_t i) throw();
            const equilibrium & operator[](size_t i) const throw();
            
            void computeK(double t);            //!< compute all constantes
            void updateScaling();               //!< update all scaling factors
            void computeGammaAndPhi(double t);
            void updateGammaAndPhi();
            void updateGamma();
            
            //!  normalize AFTER a balancing was done..
            bool normalize( array<double> &C0, double t, bool recomputeK );
            
            //! clip xi according to C
            void clip_extents();
            
            //! create solution from information
            void create(array<double> &C0, const boot &loader, const double t );
            
            
            //! C = Xstar/Delta + Q'*V
            void compute_C( const array<double> &V );
            
            //! mix
            void mix(array<double>         &C,
                     const array<vector_t> &solutions,
                     const array<double>   &weights,
                     const double           t);
            
            void mix(array<double>        &C,
                     const matrix_t       &solutions,
                     const array<double>  &weights,
                     const double          t);
            
            
            //__________________________________________________________________
            //
            // Kinetics API
            //__________________________________________________________________
            
            //! absorb a part of rho assuming that C0 is normalized !
            /**
             Append the derivatives of Gamma
             */
            void absorb(const double         t,
                        array<double>       &rho,
                        const array<double> &C0);
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(equilibria);
            vector<equilibrium::pointer>        eqs;
            math::numeric<double>::scalar_field optH;
            math::numeric<double>::vector_field optG;
            math::numeric<double>::function     optF;
            
            //! evaluate energy and compute descent direction
            double computeE( array<ptrdiff_t> & );
            
            
            //! evaluate objective function to go to positive energies
            double computeH( const array<double> &V );
            
            //! evaluate the gradient
            void   computeG( array<double> &G, const array<double> &V);
            
            //! optimize a given xi
            void   optimize();
            
            //! |Gamma/GamSF|
            double scaledGamma() const throw();
            
            //! xi = xis + ratio * u, optimize xi, update Gamma, return scaledGamma
            double   computeF(double ratio);
            
            //! callback for a differential equation: balance and normalized trial
            void odecb(array<double> &Y,double t);
            
        public:
            uniform_generator<double> ran;
            math::derivative<double>  drvs;
            double                    dt_drvs;  //!< default 1e-4, to be set
            diff_callback             callback; //!< ODE callback
        };
        
    }
    
}


#endif
