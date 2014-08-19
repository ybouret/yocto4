#include "yocto/chemical/equilibria.hpp"
#include "yocto/chemical/solution.hpp"

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
            static const char fn[] = "equilibria.mix";
            const size_t ns = solutions.size();
            assert(ns==weights.size());
            assert(C.size()>=M);
            double ws = 0;
            for(size_t j=M;j>0;--j)
            {
                C[j] = 0;
            }
            for(size_t i=ns;i>0;--i)
            {
                const double w = weights[i];
                if(w<0)
                    throw exception("%s(invalid weigths[%u]=%g)", fn, unsigned(i), w);
                ws += w;
                const array<double> &Si = solutions[i];
                for(size_t j=M;j>0;--j)
                {
                    C[j] += w * Si[j];
                }
            }
            if(ws<=0)
                throw exception("%s(invalid sum of weights)",fn);
            
            //std::cerr << "Cmix0=" << C << std::endl;
            normalize(t,C);
            //std::cerr << "Cmix1=" << C << std::endl;
        }
        
        
    }

}
