#ifndef YOCTO_CHEMICAL_EQUILIBRIA_INCLUDED
#define YOCTO_CHEMICAL_EQUILIBRIA_INCLUDED 1

#include "yocto/chemical/equilibrium.hpp"
#include "yocto/chemical/library.hpp"

#include "yocto/math/kernel/matrix.hpp"
#include "yocto/sequence/vector.hpp"

namespace yocto
{
    namespace chemical
    {
        
        typedef math::matrix<double>    matrix_t;
        typedef vector<double>          vector_t;
        typedef math::matrix<ptrdiff_t> imatrix_t;
        
        class equilibria : public equilibrium::database
        {
        public:
            explicit equilibria() throw();
            virtual ~equilibria() throw();
            
            
            const size_t M; //!< compiled #species
            const size_t N; //!< compiled #equilibria
            
            
            vector_t     K;     //!< [N] constants
            vector_t     Gamma; //!< [N] Gamma
            vector_t     xi;    //!< [N] extents
            imatrix_t    Nu;    //!< [NxM] current topology
            imatrix_t    Nu0;   //!< [NxM] initial topology
            
            void compile_for( const library &lib );
            
            void clear() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(equilibria);
            
        };
        
    }
    
}


#endif
