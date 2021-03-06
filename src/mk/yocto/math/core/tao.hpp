#ifndef YOCTO_MATH_CORE_TAO_INCLUDED
#define YOCTO_MATH_CORE_TAO_INCLUDED 1

#include "yocto/math/types.hpp"
#include "yocto/type/traits.hpp"
#include "yocto/code/unroll.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/code/arith.hpp"

namespace yocto
{
    namespace math
    {
        //______________________________________________________________________
        //
        //! Templated Algebraic Operations
        //
        // for vector operation, right hand arguments may be bigger
        // than the destination...
        //______________________________________________________________________
        
        struct tao
        {
            //------------------------------------------------------------------
            //
            // Level 1: vector/vector operations
            //
            //------------------------------------------------------------------
#define YOCTO_TAO_LOOP(N,FUNC)  YOCTO_LOOP_FUNC(N,Y_TAO_##FUNC,1)
#define YOCTO_TAO_LOOP_(N,FUNC) YOCTO_LOOP_FUNC_(N,Y_TAO_##FUNC,1)

            //! a = x
            template <typename ARRAY>
            static inline void ld( ARRAY &a, typename ARRAY::param_type x ) throw()
            {
#define Y_TAO_LD(I) a[I] = x
                YOCTO_TAO_LOOP(a.size(),LD);
#undef Y_TAO_LD
            }
            
            //! a = b, a.size() <= b.size()
            template <typename ARR, typename BRR>
            static inline void set( ARR &a, const BRR &b ) throw()
            {
                assert(a.size()<=b.size());
#define Y_TAO_SET(I) a[I] = static_cast<typename ARR::type>(b[I])
                YOCTO_TAO_LOOP(a.size(),SET);
#undef Y_TAO_SET
            }
            
            
            //! a += b
            template <typename ARR, typename BRR>
            static inline void add( ARR &a, const BRR &b ) throw()
            {
                assert(a.size()<=b.size());
#define Y_TAO_ADD(I) a[I] += static_cast<typename ARR::type>(b[I])
                YOCTO_TAO_LOOP(a.size(),ADD);
#undef Y_TAO_ADD
            }
            
            //! a -= b
            template <typename ARR, typename BRR>
            static inline void sub( ARR &a, const BRR &b ) throw()
            {
                assert(a.size()<=b.size());
#define Y_TAO_SUB(I) a[I] -= static_cast<typename ARR::type>(b[I])
                YOCTO_TAO_LOOP(a.size(),SUB);
#undef Y_TAO_SUB
            }
            
            //! a = b-a
            template <typename ARR, typename BRR>
            static inline void subp( ARR &a, const BRR &b ) throw()
            {
                assert(a.size()<=b.size());
#define Y_TAO_SUBP(I) a[I] = static_cast<typename ARR::type>(b[I]) - a[I]
                YOCTO_TAO_LOOP(a.size(),SUBP);
#undef  Y_TAO_SUBP
            }
            
            
            //! a = -b
            template <typename ARR, typename BRR>
            static inline void neg( ARR &a, const BRR &b ) throw()
            {
                assert(a.size()<=b.size());
#define Y_TAO_NEG(I) a[I] = -static_cast<typename ARR::type>(b[I])
                YOCTO_TAO_LOOP(a.size(),NEG);
#undef  Y_TAO_NEG
            }

            template <typename ARR, typename U>
            static inline void shift( ARR &a, U alpha) throw()
            {
                const typename ARR::type value(alpha);
#define Y_TAO_SHIFT(I) a[I] += value
                YOCTO_TAO_LOOP(a.size(),SHIFT);
#undef  Y_TAO_SHIFT
            }


            //! a *= x;
            template <typename ARR>
            static inline void mulby(typename ARR::param_type x, ARR &a ) throw()
            {
#define Y_TAO_MULBY(I) a[I] *= x
                YOCTO_TAO_LOOP(a.size(),MULBY);
#undef Y_TAO_MULBY
            }

            //! a = x*b
            template<typename ARR, typename BRR>
            static inline void mulset( ARR &a, typename ARR::param_type x, const BRR &b ) throw()
            {
                assert(a.size()<=b.size());
#define Y_TAO_MULSET(I) a[I] = x*static_cast<typename ARR::type>(b[I])
                YOCTO_TAO_LOOP(a.size(),MULSET);
#undef  Y_TAO_MULSET
            }
            
            //! a += x*b
            template<typename ARR, typename BRR>
            static inline void muladd( ARR &a, typename ARR::param_type x, const BRR &b ) throw()
            {
                assert(a.size()<=b.size());
#define Y_TAO_MULADD(I) a[I] += x*static_cast<typename ARR::type>(b[I])
                YOCTO_TAO_LOOP(a.size(),MULADD);
#undef  Y_TAO_MULADD
            }
            
            //! a -= x*b
            template<typename ARR, typename BRR>
            static inline void mulsub( ARR &a, typename ARR::param_type x, const BRR &b ) throw()
            {
                assert(a.size()<=b.size());
#define Y_TAO_MULSUB(I) a[I] -= x*static_cast<typename ARR::type>(b[I])
                YOCTO_TAO_LOOP(a.size(),MULSUB);
#undef  Y_TAO_MULSUB
            }

            //! a = b + x * c
            template <typename ARR, typename BRR, typename CRR>
            static inline void setprobe( ARR &a, const BRR &b, typename ARR::param_type x, const CRR &c) throw()
            {
                assert(a.size()<=b.size());
                assert(a.size()<=c.size());
#define Y_TAO_SETPROBE(I) a[I] = static_cast<typename ARR::type>(b[I]) + x * static_cast<typename ARR::type>(c[I])
                YOCTO_TAO_LOOP(a.size(),SETPROBE);
#undef  Y_TAO_SETPROBE
            }
            
            //! a*a for scalar type
            template <typename ARR>
            static inline typename ARR::type norm_sq( const ARR &a ) throw()
            {
                typename ARR::type ans(0);
#define Y_TAO_NORMSQ(I) ans += Square(a[I])
                YOCTO_TAO_LOOP(a.size(),NORMSQ);
                return ans;
#undef  Y_TAO_NORMSQ
            }
            
            //! euclidean norm
            template <typename ARR>
            static inline typename ARR::type norm( const ARR &a ) throw()
            {
                return Sqrt( Fabs( norm_sq(a) ) );
            }


            //! root mean square
            template <typename ARR>
            static inline typename ARR::type RMS( const ARR &a ) throw()
            {
                assert(a.size()>0);
                return Sqrt( Fabs(norm_sq(a))/ a.size() );
            }

            //! root mean square with type of first array
            template <typename ARR, typename BRR>
            static inline typename ARR::type RMS( const ARR &a, const BRR &b ) throw()
            {
                assert(a.size()>0);
                assert(b.size()>0);
                const size_t n = a.size();
                typename ARR::type ans(0);
#define Y_TAO_RMS(I) ans += (Square(a[I]-static_cast<typename ARR::type>(b[I])))
                YOCTO_TAO_LOOP(n,RMS);
#undef  Y_TAO_RMS
                return Sqrt(ans/n);
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
#undef  Y_TAO_DOT
            }
            
            //! sum
            template <typename ARR>
            static inline typename ARR::type sum( const ARR &a ) throw()
            {
                typename ARR::type ans(0);
#define Y_TAO_SUM(I) ans += a[I]
                YOCTO_TAO_LOOP(a.size(),SUM);
#undef  Y_TAO_SUM
            }
            
            
            //! simplify assuming integer coefficients
            template <typename ARR>
            static inline ptrdiff_t simplify( ARR &w ) throw()
            {
                register ptrdiff_t     g = 0;
                const register size_t  n = w.size();
                
                for(register size_t i=n;i>0;--i)
                {
                    w[i] = RInt(w[i]);
                    const ptrdiff_t wi = static_cast<ptrdiff_t>(w[i]);
                    const ptrdiff_t ai = wi < 0 ? -wi : wi;
                    if(!ai)
                        continue;
                    
                    for(register size_t j=i;j>0;--j)
                    {
                        w[j] = RInt(w[j]);
                        const ptrdiff_t wj = static_cast<ptrdiff_t>(w[j]);
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
            
            static inline
            ptrdiff_t i_simplify( array<ptrdiff_t> &w ) throw()
            {
                register ptrdiff_t    g = 0;
                const register size_t n = w.size();
                for(register size_t i=n;i>0;--i)
                {
                    const ptrdiff_t wi = w[i];
                    const ptrdiff_t ai = wi < 0 ? -wi : wi;
                    if(0==ai)
                    {
                        continue;
                    }
                    
                    for(register size_t j=i;j>0;--j)
                    {
                        const ptrdiff_t wj = w[j];
                        const ptrdiff_t aj = wj < 0 ? -wj : wj;
                        if(0==aj)
                        {
                            continue;
                        }
                        
                        if(g<=0)
                        {
                            g = aj;
                        }
                        else
                        {
                            g = min_of(g, gcd_of(ai,aj) );
                        }
                    }
                }
                
                if(g>1)
                {
                    for(register size_t i=n;i>0;--i)
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
            //! dot product with leading type conversion
            template <typename ARR, typename BRR, typename CRR>
            static inline typename ARR::type dot__( const ARR &, const BRR &b, const CRR &c) throw()
            {
                typename ARR::type ans(0);
#define Y_TAO_DOT__(I) ans += static_cast<typename ARR::type>(b[I])*static_cast<typename ARR::type>(c[I])
                YOCTO_TAO_LOOP(b.size(),DOT__);
#undef  Y_TAO_DOT__
                return ans;
            }
            
            //! a = M*b
            template <typename ARR, typename MAT, typename BRR>
            static inline void mul( ARR &a, const MAT &M, const BRR &b ) throw()
            {
                assert(a.size()>=M.rows);
                assert(b.size()>=M.cols);
#define Y_TAO_MULROW(I) a[I] = dot__(a,M[I],b)
                YOCTO_TAO_LOOP(M.rows,MULROW);
#undef  Y_TAO_MULROW
            }
            
            //! a += M*b
            template <typename ARR, typename MAT, typename BRR>
            static inline void mul_add( ARR &a, const MAT &M, const BRR &b ) throw()
            {
                assert(a.size()>=M.rows);
                assert(b.size()>=M.cols);
#define Y_TAO_MULADDROW(I) a[I] += dot__(a,M[I],b)
                YOCTO_TAO_LOOP(M.rows,MULADDROW);
#undef  Y_TAO_MULADDROW
            }
            
            //! a -= M*b
            template <typename ARR, typename MAT, typename BRR>
            static inline void mul_sub( ARR &a, const MAT &M, const BRR &b ) throw()
            {
                assert(a.size()>=M.rows);
                assert(b.size()>=M.cols);
#define Y_TAO_MULSUBROW(I) a[I] -= dot__(a,M[I],b)
                YOCTO_TAO_LOOP(M.rows,MULSUBROW);
#undef  Y_TAO_MULSUBROW
            }
            
            
            //! a = M'*b
            template <typename ARR, typename MAT, typename BRR>
            static inline void mul_trn( ARR &a, const MAT &M, const BRR &b ) throw()
            {
                assert(a.size()>=M.cols);
                assert(b.size()>=M.rows);
                const register size_t nr = M.rows;
#define Y_TAO_MULTRN_CORE(I)             \
typename ARR::type __sum(0);             \
for(register size_t j=nr;j>0;--j)        \
__sum += static_cast<typename ARR::type>(M[j][I]) * static_cast<typename ARR::type>(b[j])
                
#define Y_TAO_MULTRN(I) Y_TAO_MULTRN_CORE(I); a[I] = __sum
                YOCTO_TAO_LOOP(M.cols,MULTRN);
#undef  Y_TAO_MULTRN
            }
            
            //! a += M'*b
            template <typename ARR, typename MAT, typename BRR>
            static inline void mul_add_trn( ARR &a, const MAT &M, const BRR &b ) throw()
            {
                assert(a.size()>=M.cols);
                assert(b.size()>=M.rows);
                const register size_t nr = M.rows;
#define Y_TAO_MULADDTRN(I) Y_TAO_MULTRN_CORE(I); a[I] += __sum
                YOCTO_TAO_LOOP(M.cols,MULADDTRN);
#undef  Y_TAO_MULADDTRN
            }
            
            //! a -= M'*b
            template <typename ARR, typename MAT, typename BRR>
            static inline void mul_sub_trn( ARR &a, const MAT &M, const BRR &b ) throw()
            {
                assert(a.size()>=M.cols);
                assert(b.size()>=M.rows);
                const size_t nr = M.rows;
#define Y_TAO_MULSUBTRN(I) Y_TAO_MULTRN_CORE(I); a[I] -= __sum
                YOCTO_TAO_LOOP(M.cols,MULSUBTRN);
#undef  Y_TAO_MULSUBTRN
            }
            
            
            template <typename MAT, typename ARR>
            static inline typename ARR::type quadratic( const MAT &M, const ARR&a ) throw()
            {
                assert(M.is_square());
                assert(a.size()==M.rows);
                const size_t n = M.rows;
                typename ARR::type q(0);
                typename ARR::type p(0);
                for(size_t i=n;i>0;--i)
                {
                    const array<typename MAT::type> &Mi = M[i];
                    const typename ARR::type         ai = a[i];
                    q += Mi[i] * ai*ai;
                    for(size_t j=n;j>i;--j)
                    {
                        p += Mi[j] * ai * a[j];
                    }
                }
                return q+(p+p);
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
                    array<typename AMAT::type>       &ai = a[i];
                    for(size_t j=nc;j>0;--j)
                    {
                        typename AMAT::type __sum(0);
                        for(size_t k=ns;k>0;--k)
                        {
                            __sum += static_cast<typename AMAT::type>(bi[k]) * static_cast<typename AMAT::type>(c[k][j]);
                        }
                        ai[j] = __sum;
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
                    array<typename AMAT::type>       &ai = a[i];
                    for(size_t j=nc;j>0;--j)
                    {
                        typename AMAT::type  __sum(0);
                        const array<typename CMAT::type> &cj = c[j];
                        for(size_t k=ns;k>0;--k)
                        {
                            __sum += static_cast<typename AMAT::type>(bi[k]) * static_cast<typename AMAT::type>(cj[k]);
                        }
                        ai[j] = __sum;
                    }
                }
            }
            
            //! a=b'*c
            template <typename AMAT, typename BMAT, typename CMAT>
            static inline void mmul_ltrn(AMAT &a, const BMAT &b, const CMAT &c) throw()
            {
                assert(a.rows==b.cols);
                assert(a.cols==c.cols);
                assert(b.rows==c.rows);
                
                const size_t nr = a.rows;
                const size_t nc = a.cols;
                const size_t ns = b.rows;
                for(register size_t i=nr;i>0;--i)
                {
                    array<typename AMAT::type>       &ai = a[i];
                    for(size_t j=nc;j>0;--j)
                    {
                        typename AMAT::type  __sum(0);
                        for(size_t k=ns;k>0;--k)
                        {
                            __sum += static_cast<typename AMAT::type>(b[k][i]) * static_cast<typename AMAT::type>(c[k][j]);
                        }
                        ai[j] = __sum;
                    }
                }
                
            }

            //! a = b*b';
            template <typename AMAT,typename BMAT>
            static inline void row_gram(AMAT &a, const BMAT &b) throw()
            {
                assert(a.rows==b.rows);
                assert(a.rows==a.cols);
                const size_t nr = a.rows;
                const size_t nc = b.cols;
                for(register size_t i=nr;i>0;--i)
                {
                    const typename BMAT::row &b_i = b[i];
                    typename       AMAT::row &a_i = a[i];
                    for(register size_t j=nr;j>=i;--j)
                    {
                        const typename BMAT::row &b_j = b[j];
                        typename AMAT::type       __sum(0);
#define Y_TAO_ROW_GRAM(K) __sum += static_cast<typename AMAT::type>(b_i[K]) * static_cast<typename AMAT::type>(b_j[K])
                        YOCTO_TAO_LOOP_(nc,ROW_GRAM);
#undef Y_TAO_ROW_GRAM
                        a_i[j] = a[j][i] = __sum;
                    }
                }
            }
            
            
            
            
        };
        
    }
}

#endif
