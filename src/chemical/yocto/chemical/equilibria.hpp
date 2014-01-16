#ifndef YOCTO_CHEMICAL_EQUILIBRIA_INCLUDED
#define YOCTO_CHEMICAL_EQUILIBRIA_INCLUDED 1

#include "yocto/chemical/equilibrium.hpp"
#include "yocto/math/kernel/lu.hpp"
#include "yocto/math/fcn/derivative.hpp"

namespace yocto
{
    namespace chemical
    {
        
        typedef math::matrix<double>    matrix_t;
        typedef math::matrix<ptrdiff_t> imatrix_t;
        typedef vector<double>          vector_t;
        typedef math::lu<double>        lu_t;
        
        //! a database of equilibrium
        class equilibria : public equilibrium::db
        {
        public:
            static const size_t MAX_NEWTON_STEPS = 512;
            
            virtual ~equilibria() throw();
            explicit equilibria();
            
            double pressure;
            double temperature;
            double ftol;       //!< for Newton Convergence
            double time_scale; //!< for time derivatives, default is 1e-4
            
            
            
            vector_t     C;     //!< local concentrations
            vector_t     dC;    //!< corrections
            lu_t         LU;    //!< for local matrix inversion
            imatrix_t    nuR;   //!< reactives, NxM
            imatrix_t    nuP;   //!< products,  NxM
            matrix_t     nu;    //!< algebraic, NxM
            vector_t     Gamma; //!< unrolled equilibria, N
            vector_t     dtGam; //!< time derivative of Gamma
            matrix_t     Phi;   //!< Gamma Jacobian, NxM
            matrix_t     W;     //!< (Phi*nu')^(-1), NxN
            vector_t     xi;    //!< local extent
            vector_t     CC;    //!< temporary/auxiliary
            
            //! release all memory
            void reset() throw();
            
            
            //! allocate from a collection
            void build_from( collection &lib );
            
            
            //! test for positive/cutoff C
            void cleanup_C() throw();
            
            
            //! make a "true" constant equilibrium
            equilibrium &add( const string &name, const double K);
            
            //! wrapper
            equilibrium &add( const char   *name, const double K);
            
            
            //! default water wrapper
            void add_water( const collection &lib, const double Kw);
            
            
            //! default acid wrapper
            void add_acid( const collection &lib, const char *name, const char *acid, const char *base, const double Ka );
            
            
            //! compute Gamma and Phi=dGamma/dC, dGamma/dt if needed
            /**
             Uses the exact topology for Gamma and Phi,
             the set to zero the fixed derivatives.
             */
            void compute_Gamma_and_Phi( double t, bool compute_derivatives);
            
            
            //! compute Gamma and W=(Phi*nu')^(-1). dGamma/dt if needed
            /**
             \return false in case of singular composition
             */
            bool compute_Gamma_and_W( double t, bool compute_derivatives);
            
            
            //! compute nu'*(Phi*nu')^(-1)*Phi, valid for a NORMALIZED C
            void compute_Chi(matrix_t &Chi, double t);
            
            
            //! Newton algorithm to find equilibria
            bool normalize_C( double t);
            
            
            
            //! Copy first C.size() variables from y, y.size() >= C.size()
            void load_C( const array<double> &y ) throw();
            
            //! Copy first C.size() variables into y, y.size() >= C.size()
            void save_C( array<double> &y ) const throw();
            
            //! Reduce dC to a legal step
            /**
             The corresponding concentrations MUST be in C before this call !
             Use computeDerivatives=false when computing initial compositions.
             */
            void legalize_dC( double t, bool computeDerivatives=true );
            
            //! scale all equilibrium
            /**
             for each equilibirum, call scale(t)
             */
            void scale_all( double t ) const throw();
            
            //! build a random valid concentration (after scaling)
            bool trial( urand32 &ran, double t );
            
            
            friend std::ostream & operator<<( std::ostream &, const equilibria &);
            
            
            //! compute Gamma Only
            void   compute_Gamma(const double t) throw();
            
            //! convert Gamma to a RMS value
            double Gamma2RMS() const throw();
            
            //! call compute_Gamma and Gamma2RMS
            double compute_rms(double t) throw();
            
            //! try to decrease C with dC
            /**
             safe decrease: if d[i]>=C[i], divide C[i].
             Otherwise C[i] -= d[i];
             \return false is one coordinate was cut
             */
            bool full_decrease_C_with( const array<double> &d ) throw();
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(equilibria);
            math::derivative<double> dervs;
            
            
        };
        
        
    }
}

#endif

