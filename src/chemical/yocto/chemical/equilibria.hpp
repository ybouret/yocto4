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
            virtual ~equilibria() throw();
            explicit equilibria();
            
            double pressure;
            double temperature;
            double ftol;       //!< for Newton Convergence
            double time_scale; //!< for time derivatives, default is 1e-4
            
            const double tiny;      //!< numeric cutoff: numeric<double>::tiny
            const double sqrt_tiny; //!< numeric RMS cutoff: numeric<double>::sqrt_tiny
            
            
            vector_t     C;     //!< local concentrations
            vector_t     dC;    //!< corrections
            imatrix_t    nuR;   //!< reactives, NxM
            imatrix_t    nuP;   //!< products,  NxM
            matrix_t     nu;    //!< algebraic, NxM
            vector_t     Gamma; //!< unrolled equilibria, N
            vector_t     dtGam; //!< time derivative of Gamma
            matrix_t     Phi;   //!< Gamma Jacobian, NxM
            matrix_t     W;     //!< (Phi*nu')^(-1), NxN
            vector_t     xi;    //!< local extent
            lu_t         LU;    //!< for local matrix inversion
            
            //! release all memory
            void reset() throw();
            
            //! allocate from a collection
            void build_from( collection &lib );
            
            //! make a "true" constant equilibrium
            equilibrium &add( const string &name, const double K);
            
            //! wrapper
            equilibrium &add( const char   *name, const double K);
            
            //! default water wrapper
            void add_water( const collection &lib, const double Kw);
            
            //! default acid wrapper
            void add_acid( const collection &lib, const char *name, const char *acid, const char *base, const double Ka );
            
            
            //! compute Gamma and Phi, dGamma/dt if needed
            void compute_Gamma_and_Phi( double t, bool compute_derivatives);
            
            
            //! compute Gamma and W=(Phi*nu')^(-1). dGamma/dt if needed
            void compute_Gamma_and_W( double t, bool compute_derivatives, const matrix_t &Nu);
            
            //! compute nu'*(Phi*nu')^(-1)*Phi for a NORMALIZED C
            void compute_Chi(matrix_t &Chi, double t);
            
            //! use Newton's algorithm with a Nu matrix
            void normalize_with( const matrix_t &Nu, double t );
            
            //! cleanup and Newton algorithm to find equilibria
            /**
             use the total nu matrix.
             */
            void normalize_C( double t );
            
            //! Copy first C.size() variables from y, y.size() >= C.size()
            void load_C( const array<double> &y ) throw();
            
            //! Copy first C.size() variables into y, y.size() >= C.size()
            void save_C( array<double> &y ) const throw();
            
            //! reduce dC to a legal step
            /**
             The corresponding concentrations MUST be in C before this call !
             Use computeDerivatives=false when computing initial compositions.
             */
            void legalize_with( const matrix_t &Nu, double t, bool computeDerivatives);
            
            //! Reduce dC to a legal step
            /**
             use the total nu matrix, with derivatives by default.
            */
            void legalize_dC( double t, bool computeDerivatives=true );
            
            
            //! scale all equilibrium
            void scale_all( double t ) const throw();
            
            //! build a random valid concentration after scaling
            void trial( urand32 &ran, double t );
            
            friend std::ostream & operator<<( std::ostream &, const equilibria &);
            
            //! test for positive/cutoff C
            void cleanup_C() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(equilibria);
            math::derivative<double> dervs;
            
            
        };
        
        
    }
}

#endif

