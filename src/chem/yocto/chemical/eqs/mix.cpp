#include "yocto/chemical/equilibria.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace chemical
    {
        void equilibria:: mix(array<double>         &C,
                              const array<solution> &solutions,
                              const array<double>   &weights,
                              const double           t)
        {
            assert(weights.size()==solutions.size());
            assert(M<=C.size());
            for(size_t j=M;j>0;--j)
            {
                C[j] = 0;
            }
            for(size_t i=solutions.size();i>0;--i)
            {
                const solution &S = solutions[i];
                assert(S.size()==M);
                const double    w = weights[i];
                for(size_t j=M;j>0;--j)
                {
                    C[j] += w * S[j];
                }
            }
            if(!balance(C))
            {
                throw exception("equilibria.mix: no possible balance!");
            }
            if(!normalize(C, t, true))
            {
                throw exception("equilibria.mix: no possible normalization!");
            }
        }
        
    }
    
}
