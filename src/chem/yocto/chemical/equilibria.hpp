#ifndef YOCTO_CHEMICAL_EQUILIBRIA_INCLUDED
#define YOCTO_CHEMICAL_EQUILIBRIA_INCLUDED 1

#include "yocto/chemical/equilibrium.hpp"
#include "yocto/math/kernel/crout.hpp"

namespace yocto
{
    namespace chemical
    {
        
        typedef math::matrix<double>    matrix_t;
        typedef math::matrix<ptrdiff_t> imatrix_t;
        typedef vector<double>          vector_t;
        typedef math::crout<double>     lu_t;
        
        class equilibria : public equilibrium::database
        {
        public:
            
            explicit equilibria();
            virtual ~equilibria() throw();
            
            //__________________________________________________________________
            //
            // Dynamic Quantities, allocated with startup...
            //__________________________________________________________________
            const size_t M; //!< #species from collection
            const size_t N; //!< #reactions
            
            matrix_t     Nu;    //!< NxM, may be reduced in case of fixed species
            matrix_t     Nu0;   //!< NxM, full topology
            vector_t     K;     //!< N constants at time t,
            vector_t     Gamma; //!< N constraints
            matrix_t     Phi;   //!< NxM dGamma/dX,
            matrix_t     W;     //!< Phi*Nu'
            vector_t     xi;    //!< N extent
            lu_t         LU;
            vector_t     dC;    //!< M concentrations increase (Newton's Step)
            vector_t     grad;  //!< M, 1/2 Gamma^2 gradient
            vector_t     Ctry;  //!< M trial concentration
            
            //__________________________________________________________________
            //
            // API
            //__________________________________________________________________
            void  startup( const collection &lib);
            void  cleanup() throw();
            
            void output( std::ostream & ) const;
            inline friend std::ostream & operator<<( std::ostream &os, const equilibria &eqs)
            {
                eqs.output(os);
                return os;
            }
            
            //! initilialize K and compute Gamma, return F
            double computeGamma(double t, const array<double> &C );
            
            //! compute Gamma once K is computed, return F
            double updateGamma(const array<double> &C);
            
            //! initialize K, compute Gamma,Phi and gradient, return getF
            double computeGammaAndPhi(double t, const array<double> &C);
            
            //! recompute Gamma and Phi for the same constants (also gradient), return getF()
            double updateGammaAndPhi(const array<double> &C) throw();
            
            //! update only Phi and the gradient part
            void updatePhi(const array<double> &C) throw();
            
            
            //! compute the advancement getting back to Gamma=0
            /**
             compute W = Phi * Nu' and xi = -W^(-1).Gamma,
             then dC = Nu' * xi.
             \return false for a singular compositon
             */
            bool computeNewtonStep() throw();
            
            //! objective function : 1/2 Gamma^2
            double getF() const throw();
            
            //! normalize system at time t
            bool  normalize( double t, array<double> &C );
            
            
        private:
            double computeTrialFrom(const array<double> &C, const double lambda) throw();
            YOCTO_DISABLE_COPY_AND_ASSIGN(equilibria);
        };
        
    }
    
}

#endif
