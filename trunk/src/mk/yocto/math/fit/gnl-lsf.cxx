#include "yocto/math/ztype.hpp"
#include "yocto/math/fit/gnl-lsf.hpp"


namespace yocto
{
    namespace math
    {
        
        template <>
        LeastSquares<real_t>::Sample:: ~Sample() throw()
        {
        }
        
        template <>
        LeastSquares<real_t>::Sample:: Sample(const Array &userX,
                                              const Array &userY,
                                              const Array &userZ) throw() :
        X( userX ),
        Y( userY ),
        Z( userZ ),
        N(0),
        Q(0),
        M(0),
        Gamma()
        {
        }
        
        template <>
        void LeastSquares<real_t>::Sample:: prepare(size_t local_nvar, size_t global_nvar)
        {
            assert(local_nvar>0);
            assert(global_nvar>0);
            assert(local_nvar<=global_nvar);
            assert( X.size() == Y.size() );
            assert( X.size() == Z.size() );
            (size_t &)N = X.size();
            if(local_nvar>N)
            {
                throw exception("too many variables");
            }
            (size_t &)Q = local_nvar;
            (size_t &)M = global_nvar;
            
            Gamma.make(Q,M);
            
            
        }
        
        
    }
    
}



namespace yocto
{
    namespace math
    {
        
        
        
        template <>
        LeastSquares<real_t>:: ~LeastSquares() throw()
        {
        }
        
        template <>
        LeastSquares<real_t>:: LeastSquares()
        {
        }
        
        
    }
}
