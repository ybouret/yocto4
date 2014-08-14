#include "yocto/chemical/boot.hpp"
//#include "yocto/exception.hpp"


#include "yocto/math/types.hpp"
#include "yocto/code/utils.hpp"


namespace yocto
{
    
    using namespace math;
    
    namespace chemical
    {
        
        
        //======================================================================
        //
        // integer Gram-Schmidt
        //
        //======================================================================
        namespace
        {
            
            //------------------------------------------------------------------
            //! <v|v>
            //------------------------------------------------------------------
            static inline integer_t __inorm_sq( const array<double> &v ) throw()
            {
                integer_t ans = 0;
                
                for(size_t j=v.size();j>0;--j)
                {
                    const integer_t ii = RInt(v[j]);
                    ans += ii*ii;
                }
                
                return ans;
            }
            
            //------------------------------------------------------------------
            //! <u|v>
            //------------------------------------------------------------------
            static inline integer_t __idot( const array<double> &v, const array<double> &u) throw()
            {
                assert(u.size()==v.size());
                
                integer_t ans = 0;
                
                for(size_t j=v.size();j>0;--j)
                {
                    ans += RInt(v[j]) * RInt(u[j]);
                }
                
                return ans;
            }
            
            //------------------------------------------------------------------
            // simplify an integer row
            //------------------------------------------------------------------
            static inline void __simplify( array<integer_t> &w ) throw()
            {
                integer_t    g = 0;
                const size_t n = w.size();
                
                for(size_t i=n;i>0;--i)
                {
                    const integer_t wi = w[i];
                    const integer_t ai = wi < 0 ? -wi : wi;
                    if(!ai)
                        continue;
                    
                    for(size_t j=i;j>0;--j)
                    {
                        const integer_t wj  = w[j];
                        const integer_t aj  = wj < 0 ? -wj : wj;
                        if(!aj)
                            continue;
                        
                        const integer_t gij = gcd_of(ai,aj);
                        if(g<=0)
                        {
                            g = gij;
                        }
                        else
                        {
                            g = min_of(g,gij);
                        }
                    }
                }
                
                if(g>0)
                {
                    for(size_t i=n;i>0;--i)
                    {
                        w[i] /= g;
                    }
                }
            }
        }
        
        
        
        void boot:: igs( matrix_t &A )
        {
            const size_t dim = A.cols;
            const size_t num = A.rows;
            ivector_t    usq(num,0);
            ivector_t    w(dim,0);
            
            for(size_t i=1;i<=num;++i)
            {
                array<double> &v  = A[i];
                for(size_t k=1;k<i;++k)
                {
                    const array<double> &u  = A[k];
                    const integer_t      u2 = usq[k];
                    const integer_t      uv = __idot(u,v);
                    // construct integer vector
                    for(size_t j=dim;j>0;--j)
                    {
                        w[j] = u2* RInt(v[j]) - uv * RInt(u[j]);
                    }
                    __simplify(w);
                    for(size_t j=dim;j>0;--j)
                    {
                        v[j] = w[j];
                    }
                }
                usq[i] = __inorm_sq(v);
            }
            
        }
        
    }
    
}

