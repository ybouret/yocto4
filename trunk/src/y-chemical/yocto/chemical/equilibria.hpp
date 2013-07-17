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
        
        class equilibria : public equilibrium::db
        {
        public:
            virtual ~equilibria() throw();
            explicit equilibria();
            double pressure;
            double temperature;
            double ftol;       //!< for Newton Convergence
            double time_scale; //!< for time derivatives
            
            imatrix_t nuR;   //!< reactives, NxM
            imatrix_t nuP;   //!< products,  NxM
            matrix_t  nu;    //!< algebraic, NxM
            vector_t  Gamma; //!< unrolled equilibria, N
            vector_t  dtGam; //!< time derivative of Gamma
            matrix_t  Phi;   //!< Gamma Jacobian, NxM
            matrix_t  W;     //!< (Phi*nu')^(-1), NxN
            vector_t  C;     //!< local concentrations
            vector_t  xi;    //!< local extent
            vector_t  dC;    //!< corrections
            lu_t      LU;    //!< for local matrix inversion
            
            void reset() throw();
            void build_from( collection &lib );
            
            //! make a "true" constant equilibrium
            equilibrium &add( const string &name, const double K);
            
            //! wrapper
            equilibrium &add( const char   *name, const double K);

            void add_water( const collection &lib, const double Kw = 1e-14 );
            void add_acid( const collection &lib, const char *name, const char *acid, const char *base, const double Ka );
            
            
            //! compute Gamma and W=(Phi*nu')^(-1). dGamma/dt if needed
            void compute_Gamma_and_W( double t, bool compute_derivatives);
            
            //! Newton algorithm to find equilibria
            void normalize_C( double t );
            
            //! Reduce dC to a legal step
            void legalize_dC( double t );
            
            friend std::ostream & operator<<( std::ostream &, const equilibria &);
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(equilibria);
            math::derivative<double> dervs;
        };
        
        
    }
}

#endif

