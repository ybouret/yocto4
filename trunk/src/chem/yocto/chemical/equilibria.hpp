#ifndef YOCTO_CHEMICAL_EQUILIBRIA_INCLUDED
#define YOCTO_CHEMICAL_EQUILIBRIA_INCLUDED 1

#include "yocto/chemical/equilibrium.hpp"
#include "yocto/math/kernel/crout.hpp"
#include "yocto/container/tuple.hpp"

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
            
            YOCTO_PENTUPLE_DECL(extent,
                                bool,  has_forward_limit,
                                double,max_forward_value,
                                bool,  has_reverse_limit,
                                double,max_reverse_value,
                                bool,  blocked);
            inline extent() throw() :
            has_forward_limit(false),
            max_forward_value(0.0),
            has_reverse_limit(false),
            max_reverse_value(0.0),
            blocked(false){}
            YOCTO_DISABLE_ASSIGN(extent);
            YOCTO_TUPLE_END();
            
            
            //__________________________________________________________________
            //
            // Dynamic Quantities, allocated with startup...
            //__________________________________________________________________
            const size_t M; //!< #species from collection
            const size_t N; //!< #reactions
            
            matrix_t       Nu;          //!< [NxM], may be reduced in case of fixed species
            matrix_t       Nu0;         //!< [NxM], full initial topology
            vector_t       K;           //!< [N] constants at time t,
            vector_t       Gamma;       //!< [N] constraints
            matrix_t       Phi;         //!< [NxM] dGamma/dX,
            matrix_t       W;           //!< [NxN] Phi*Nu'
            vector_t       xi;          //!< [N] extent
            vector<extent> limits;      //!< [N] infos
            lu_t           LU;          //!< [N] solver
            vector<size_t> active;      //!< [M] number or reaction involving each species
            vector_t       dC;          //!< M concentrations increase (Newton's Step)
            vector_t       Ctmp;        //!< M temp concentrations
            
            //__________________________________________________________________
            //
            // API
            //__________________________________________________________________
            void  startup( const collection &lib);
            void  find_active_species() throw(); //!< fill active from Nu
            void  cleanup() throw();
            
            bool  validate( array<double> &C );
            
            
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
            
            //! initialize K, compute Gamma,Phi  return getF
            double computeGammaAndPhi(double t, const array<double> &C);
            
            //! recompute Gamma and Phi for the same constants, return getF()
            double updateGammaAndPhi(const array<double> &C) throw();
            
            //! update only Phi
            void updatePhi(const array<double> &C) throw();
            
            //! objective function : 1/2 Gamma^2
            double getF() const throw();
            
            //! normalize system at time t
            bool  normalize( double t, array<double> &C );
            
            //! find limits of extents
            void  find_limits_of( const array<double> &C ) throw();
            
            //! apply limits to current extents xi
            void  clip_extents() throw();
            
            
        private:
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(equilibria);
        };
        
    }
    
}

#endif
