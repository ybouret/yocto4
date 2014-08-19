#ifndef YOCTO_CHEMICAL_EQUILIBRIA_INCLUDED
#define YOCTO_CHEMICAL_EQUILIBRIA_INCLUDED 1

#include "yocto/chemical/equilibrium.hpp"
#include "yocto/math/kernel/crout.hpp"
#include "yocto/container/tuple.hpp"
#include "yocto/code/rand.hpp"


namespace yocto
{
    namespace chemical
    {
        
        typedef math::matrix<double>      matrix_t;
        typedef ptrdiff_t                 integer_t;
        typedef math::matrix<integer_t>   imatrix_t;
        typedef vector<double>            vector_t;
        typedef vector<integer_t>         ivector_t;
        typedef math::crout<double>       lu_t;
        typedef uniform_generator<double> alea_t;
        
        class boot;
        
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
            ivector_t      dNu;         //!< [N], Delta_r Nu
            ivector_t      dNuP;        //!< [N], Delta_r Nu_{products}
            vector_t       K;           //!< [N] constants at time t,
            vector_t       Gamma;       //!< [N] constraints
            matrix_t       Phi;         //!< [NxM] dGamma/dX,
            matrix_t       W;           //!< [NxN] Phi*Nu'
            vector_t       xi;          //!< [N] extent
            vector<extent> limits;      //!< [N] infos
            vector<size_t> online;      //!< [N] online reactions
            vector_t       scaled;      //!< [N] scaled concentration
            vector_t       gammaC;      //!< [N] scaling factor for Gamma
            vector<size_t> active;      //!< [M] number or reaction involving each species
            vector_t       dC;          //!< [M] concentrations increase (Newton's Step)
            vector<size_t> bad;         //!< [M] bad concentrations indices
            
            drvs_type drvs;
            double    h;    //!< scaling for derivative, initial 1e-4
            
            //__________________________________________________________________
            //
            // API
            //__________________________________________________________________
            double derivate( function_type &f, const double t);
            
            //! allocate memory, find Nu0 and Nu, find_active_species()
            void  startup( const collection &lib);
            
            //! find active species from Nu
            void  find_active_species() throw();
            
            //! release memory
            void  cleanup() throw();
            
            //! restore from Nu0
            void restore_topology() throw();
            
            
            //! try to find valid concentrations
            bool  validate( array<double> &C );
            
            
            void output( std::ostream & ) const;
            inline friend std::ostream & operator<<( std::ostream &os, const equilibria &eqs)
            {
                eqs.output(os);
                return os;
            }
            
            //! initialize K
            void computeK(double t);
            
            //! compute scaled concentrations, once K is computed
            void  compute_scaled_concentrations() throw();
            
            
            //! generate a set of positive ACTIVE and PRE-SCALED concentrations
            /**
             Non-active concentrations are left untouched
             */
            void generate(array<double> &C, alea_t &ran) throw();
            
            
            
            //! initilialize K and compute Gamma, return F
            void computeGamma(double t, const array<double> &C );
            
            //! compute Gamma once K is computed, return F
            void updateGamma(const array<double> &C);
            
            //! initialize K, compute Gamma,Phi  return getF
            void computeGammaAndPhi(double t, const array<double> &C);
            
            //! recompute Gamma and Phi for the same constants, return getF()
            void updateGammaAndPhi(const array<double> &C) throw();
            
            //! update only Phi
            void updatePhi(const array<double> &C) throw();
         
            //! normalize system at time t
            bool  normalize( double t, array<double> &C, bool recomputeK=true );
            
            //! find limits of extents
            void  find_limits_of( const array<double> &C ) throw();
            
            //! to debug
            void  show_limits() const;
            
            //! apply limits to current extents xi/scaling
            void  clip_extents() throw();
            
            //! compute dGamma/dt (in xi) and Phi
            void computeGammaPrimeAndPhi(double t, const array<double> &C);
            
            
            //! assume that C is normalized: aborb a part of X at time t
            void  absorb(double t, array<double> &X, const array<double> &C);
            
            void initialize_with(const boot       &loader,
                                 const collection &lib,
                                 const double      t,
                                 array<double>     &C0);
            
        private:
            //! remove unactive column
            void cleanPhi() throw();
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(equilibria);
        };
        
    }
    
}

#endif
