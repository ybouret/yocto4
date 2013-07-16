#ifndef YOCTO_CHEMICAL_EQUILIBRIA_INCLUDED
#define YOCTO_CHEMICAL_EQUILIBRIA_INCLUDED 1

#include "yocto/chemical/equilibrium.hpp"
#include "yocto/math/kernel/lu.hpp"

namespace yocto
{
    namespace chemical
    {
        
        typedef math::matrix<double>    matrix_t;
        typedef math::matrix<ptrdiff_t> imatrix_t;
        typedef vector<double>          vector_t;
        
        class equilibria : public equilibrium::db
        {
        public:
            virtual ~equilibria() throw();
            explicit equilibria();
            double pressure;
            double temperature;

            imatrix_t nuR;   //!< reactives, NxM
            imatrix_t nuP;   //!< products,  NxM
            matrix_t  nu;    //!< algebraic, NxM
            vector_t  Gamma; //!< unrolled equilibria, N
            vector_t  dGam;  //!< time derivative of Gamma
            matrix_t  Phi;   //!< Gamma Jacobian, NxM
            matrix_t  W;     //!< (Phi*nu')^(-1), NxN
            vector_t  C;     //!< local concentrations
            
            void reset() throw();
            void build_from( collection &lib );
            
            //! make a "true" constant equilibrium
            equilibrium &add( const string &name, const double K);
            
            //! wrapper
            equilibrium &add( const char   *name, const double K);

            
            //!
            void compute_Gamma_and_W( double t, bool compute_derivatives);
            
            
            friend std::ostream & operator<<( std::ostream &, const equilibria &);
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(equilibria);
            
        };
        
        
    }
}

#endif

