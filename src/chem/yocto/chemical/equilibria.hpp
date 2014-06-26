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
            
            matrix_t     Nu;    //!< NxM
            vector_t     K;     //!< N constants at time t,
            vector_t     Gamma; //!< N constraints
            matrix_t     Phi;   //!< NxM dGamma/dX,
            
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
            
            void computeGamma(double t, const array<double> &C );
            void updateGamma(const array<double> &C);
            void computeGammaAndPhi(double t, const array<double> &C);
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(equilibria);
        };
        
    }
    
}

#endif
