#ifndef YOCTO_MATH_KERNEL_TAO_INCLUDED
#define YOCTO_MATH_KERNEL_TAO_INCLUDED 1

#include "yocto/math/types.hpp"
#include "yocto/type/traits.hpp"
#include "yocto/math/kernel/matrix.hpp"
#include "yocto/code/unroll.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace math
    {
        
        struct tao
        {
            //------------------------------------------------------------------
            //
            // Level 1: vector/vector operations
            //
            //------------------------------------------------------------------
#define YOCTO_TAO_LOOP(N,FUNC) YOCTO_LOOP_FUNC(N,Y_TAO_##FUNC,1)
            
            //! a = x
            template <typename ARRAY>
            static inline void ld( ARRAY &a, typename ARRAY::param_type x ) throw()
            {
#define Y_TAO_LD(I) a[I] = x
                YOCTO_TAO_LOOP(a.size(),LD);
            }
            
            //! a = b
            template <typename ARR, typename BRR>
            static inline void set( ARR &a, const BRR &b ) throw()
            {
                assert(a.size()<=b.size());
#define Y_TAO_SET(I) a[I] = b[I]
                YOCTO_TAO_LOOP(a.size(),SET);
            }
            
            //! a = -b
            template <typename ARR, typename BRR>
            static inline void neg( ARR &a, const BRR &b ) throw()
            {
                assert(a.size()<=b.size());
#define Y_TAO_NEG(I) a[I] = -static_cast<typename ARR::type>(b[I])
                YOCTO_TAO_LOOP(a.size(),NEG);
            }
            
            
            //! a = x*b
            template<typename ARR, typename BRR>
            static inline void mulset( ARR &a, typename ARR::param_type x, const BRR &b ) throw()
            {
                assert(a.size()<=b.size());
#define Y_TAO_MULSET(I) a[I] = x*static_cast<typename ARR::type>(b[I])
                YOCTO_TAO_LOOP(a.size(),MULSET);
            }
            
            //! a += x*b
            template<typename ARR, typename BRR>
            static inline void muladd( ARR &a, typename ARR::param_type x, const BRR &b ) throw()
            {
                assert(a.size()<=b.size());
#define Y_TAO_MULADD(I) a[I] += x*static_cast<typename ARR::type>(b[I])
                YOCTO_TAO_LOOP(a.size(),MULADD);
            }
            
            //! a -= x*b
            template<typename ARR, typename BRR>
            static inline void mulsub( ARR &a, typename ARR::param_type x, const BRR &b ) throw()
            {
                assert(a.size()<=b.size());
#define Y_TAO_MULSUB(I) a[I] -= x*static_cast<typename ARR::type>(b[I])
                YOCTO_TAO_LOOP(a.size(),MULSUB);
            }
            
            //! a*a for scalar type
            template <typename ARR>
            static inline typename ARR::type norm_sq( const ARR &a ) throw()
            {
                typename ARR::type ans(0);
#define Y_TAO_NORMSQ(I) ans += Square(a[I])
                YOCTO_TAO_LOOP(a.size(),NORMSQ);
                return ans;
            }
            
            //! a*b for scalar type, a is the leading type
            template<typename ARR, typename BRR>
            static inline typename ARR::type dot( const ARR &a, const BRR &b ) throw()
            {
                assert(a.size()<=b.size());
                typename ARR::type ans(0);
#define Y_TAO_DOT(I) ans += a[I]*static_cast<typename ARR::type>(b[I])
                YOCTO_TAO_LOOP(a.size(),DOT);
                return ans;
            }
            
            //! sum
            template <typename ARR>
            static inline typename ARR::type dot( const ARR &a ) throw()
            {
                typename ARR::type ans(0);
#define Y_TAO_SUM(I) ans += a[I]
                YOCTO_TAO_LOOP(a.size(),SUM);
            }
            
            //! simplify assuming integer coefficients
            template <typename ARR>
            static inline ptrdiff_t simplify( ARR &w ) throw()
            {
                ptrdiff_t     g = 0;
                const size_t  n = w.size();
                
                for(size_t i=n;i>0;--i)
                {
                    const ptrdiff_t wi = (w[i] = RInt(w[i]));
                    const ptrdiff_t ai = wi < 0 ? -wi : wi;
                    if(!ai)
                        continue;
                    
                    for(size_t j=i;j>0;--j)
                    {
                        const ptrdiff_t wj = (w[j] = RInt(w[j]) );
                        const ptrdiff_t aj = wj < 0 ? -wj : wj;
                        if(!aj)
                            continue;
                        const ptrdiff_t gij = gcd_of(ai,aj);
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
                return g;
            }
            
            //------------------------------------------------------------------
            //
            // Level 2: matrix/vector operations
            //
            //------------------------------------------------------------------
            template <typename ARR, typename MAT, typename BRR>
            static inline void mul( ARR &a, const MAT &M, const BRR &b ) throw()
            {
                assert(a.size()>=M.rows);
                assert(b.size()>=M.cols);
                
#define Y_TAO_MULROW(I) a[I] = static_cast<typename ARR::type>(dot(M[I],b));
                YOCTO_TAO_LOOP(M.rows,MULROW);
            }
            
            template <typename ARR, typename MAT, typename BRR>
            static inline void mul_add( ARR &a, const MAT &M, const BRR &b ) throw()
            {
                assert(a.size()>=M.rows);
                assert(b.size()>=M.cols);
#define Y_TAO_MULADDROW(I) a[I] += static_cast<typename ARR::type>(dot(M[I],b))
                
                YOCTO_TAO_LOOP(M.rows,MULADDROW);
            }
            
            template <typename ARR, typename MAT, typename BRR>
            static inline void mul_sub( ARR &a, const MAT &M, const BRR &b ) throw()
            {
                assert(a.size()>=M.rows);
                assert(b.size()>=M.cols);
#define Y_TAO_MULSUBROW(I) a[I] -= static_cast<typename ARR::type>(dot(M[I],b))
                
                YOCTO_TAO_LOOP(M.rows,MULSUBROW);
            }
            
            
            template <typename ARR, typename MAT, typename BRR>
            static inline void mul_trn( ARR &a, const MAT &M, const BRR &b ) throw()
            {
                assert(a.size()>=M.cols);
                assert(b.size()>=M.rows);
                const size_t nr = M.rows;
#define Y_TAO_MULTRN_CORE(I)           \
typename MAT::type sum(0);             \
for(size_t j=nr;j>0;--j) sum += M[j][I]
#define Y_TAO_MULTRN(I) Y_TAO_MULTRN_CORE(I); a[I] = static_cast<typename ARR::type>(sum)
                
                YOCTO_TAO_LOOP(M.cols,MULTRN);
            }
            
            template <typename ARR, typename MAT, typename BRR>
            static inline void mul_add_trn( ARR &a, const MAT &M, const BRR &b ) throw()
            {
                assert(a.size()>=M.cols);
                assert(b.size()>=M.rows);
                const size_t nr = M.rows;
#define Y_TAO_MULADDTRN(I) Y_TAO_MULTRN_CORE(I); a[I] += static_cast<typename ARR::type>(sum)
                YOCTO_TAO_LOOP(M.cols,MULADDTRN);
            }
            
            template <typename ARR, typename MAT, typename BRR>
            static inline void mul_sub_trn( ARR &a, const MAT &M, const BRR &b ) throw()
            {
                assert(a.size()>=M.cols);
                assert(b.size()>=M.rows);
                const size_t nr = M.rows;
#define Y_TAO_MULSUBTRN(I) Y_TAO_MULTRN_CORE(I); a[I] -= static_cast<typename ARR::type>(sum)
                
                YOCTO_TAO_LOOP(M.cols,MULSUBTRN);
            }
            
            //------------------------------------------------------------------
            //
            // Level 3: matrix/matrix operations
            //
            //------------------------------------------------------------------
            
            //! a=b*c
            template <typename AMAT, typename BMAT, typename CMAT>
            static inline void mmul(AMAT &a, const BMAT &b, const CMAT &c) throw()
            {
                assert(a.rows==b.rows);
                assert(a.cols==c.cols);
                assert(b.cols==c.rows);
                const size_t nr = a.rows;
                const size_t nc = a.cols;
                const size_t ns = b.cols;
                for(size_t i=nr;i>0;--i)
                {
                    const array<typename BMAT::type> &bi = b[i];
                    for(size_t j=nc;j>0;--j)
                    {
                        typename AMAT::type sum(0);
                        for(size_t k=ns;k>0;--k)
                        {
                            sum += static_cast<typename AMAT::type>(bi[k]) * static_cast<typename AMAT::type>(c[k][j]);
                        }
                        a[i][j] = sum;
                    }
                }
            }
            
            //! a=b*c'
            template <typename AMAT, typename BMAT, typename CMAT>
            static inline void mmul_rtrn(AMAT &a, const BMAT &b, const CMAT &c) throw()
            {
                assert(a.rows==b.rows);
                assert(a.cols==c.rows);
                assert(b.cols==c.cols);
                const size_t nr = a.rows;
                const size_t nc = a.cols;
                const size_t ns = b.cols;
                for(size_t i=nr;i>0;--i)
                {
                    const array<typename BMAT::type> &bi = b[i];
                    for(size_t j=nc;j>0;--j)
                    {
                        typename AMAT::type sum(0);
                        const array<typename CMAT::type> &cj = c[j];
                        for(size_t k=ns;k>0;--k)
                        {
                            sum += static_cast<typename AMAT::type>(bi[k]) * static_cast<typename AMAT::type>(cj[k]);
                        }
                        a[i][j] = sum;
                    }
                }
            }

            
            
            
        };
        
    }
}

#endif
