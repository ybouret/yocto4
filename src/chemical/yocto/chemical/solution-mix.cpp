#include "yocto/chemical/solution.hpp"
#include "yocto/chemical/equilibria.hpp"
#include "yocto/exception.hpp"
#include "yocto/sort/quick.hpp"

namespace yocto
{
    namespace chemical
    {
        
        
        
        void solution:: mix(equilibria            &cs,
                            const array<solution> &solutions,
                            const array<double>   &weights,
                            double                 t)
        {
            assert(solutions.size()==weights.size());
            assert(components==cs.C.size());
            const size_t ns = solutions.size();
            double       ws = 0;
            ldz();
            vector<double>         wC(ns,0);
            vector<const_iterator> it(ns,as_capacity);
            
            //------------------------------------------------------------------
            //-- initialize and check weights
            //------------------------------------------------------------------
            for(size_t i=1;i<=ns;++i)
            {
                assert(has_same_components_than(solutions[i]));
                it.push_back( solutions[i].begin() );
                const double w = weights[i];
                if(w<0)
                    throw exception("solution::mix(invalid weights[%u]=%g)", unsigned(i), w);
                ws += w;
            }
            if(ws<=0)
                throw exception("solution::mix(invalid sum of weights)");

            //------------------------------------------------------------------
            //-- fill  cs.C
            // for each component
            //------------------------------------------------------------------
            for(size_t j=1;j<=components;++j)
            {
                //-- for each solution
                for(size_t i=1;i<=ns;++i)
                {
                    const_iterator  &iter = it[i];
                    const component &comp = *iter;
                    wC[i] = weights[i] * comp.concentration;
                    ++iter;
                }
                quicksort(wC);
                double sum = 0;
                for(size_t i=1;i<=ns;++i)
                    sum += wC[i];
                cs.C[j] = sum;
            }
            
            cs.normalize_C(t);
            this->load(cs.C);
            
        }
    }
    
}

