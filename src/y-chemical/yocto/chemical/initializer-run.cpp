#include "yocto/chemical/initializer.hpp"
#include "yocto/exception.hpp"


namespace yocto
{
    namespace chemical
    {
        
        void initializer:: operator()( equilibria &cs, collection &lib )
        {
            //==================================================================
            //
            // initialize the equilibria
            //
            //==================================================================
            cs.build_from(lib);
            const size_t N = cs.size();
            const size_t M = lib.size();
            const size_t Nc = size();
            if( N+Nc != M )
                throw exception("initialier: #species=%u != (#equilibria=%u+#constraints=%u)", unsigned(M), unsigned(N), unsigned(Nc) );
            
            //==================================================================
            //
            // build the algebraic constraints
            //
            //==================================================================
            matrix_t P(Nc,M);
            vector_t Lam(Nc,0);
            for(size_t i=1; i <= Nc; ++i )
            {
                const constraint &Q = * (*this)[i];
                Lam[i] = Q.value;
                for( constraint_weights::const_iterator weight=Q.begin(); weight != Q.end(); ++weight )
                {
                    const species::ptr &sp = lib[weight->key];
                    P[i][sp->indx] = *weight;
                }
            }
            
            matrix_t Q(N,M);
            {
                matrix_t F(M,M);
                for(size_t i=1;i<=Nc;++i)
                {
                    for(size_t j=1;j<=M;++j)
                        F[i][j] = P[i][j];
                }
                
                
            }
        }

    }
}
