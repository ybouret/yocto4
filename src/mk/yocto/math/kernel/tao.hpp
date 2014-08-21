#ifndef YOCTO_MATH_KERNEL_TAO_INCLUDED
#define YOCTO_MATH_KERNEL_TAO_INCLUDED 1

#include "yocto/math/types.hpp"
#include "yocto/type/traits.hpp"
#include "yocto/math/kernel/matrix.hpp"
#include "yocto/code/unroll.hpp"

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
#define Y_TAO_MULROW(I) a[I] = dot(M[I],b)
                YOCTO_TAO_LOOP(M.rows,MULROW);
            }
            
            template <typename ARR, typename MAT, typename BRR>
            static inline void mul_add( ARR &a, const MAT &M, const BRR &b ) throw()
            {
                assert(a.size()>=M.rows);
                assert(b.size()>=M.cols);
#define Y_TAO_MULADDROW(I) a[I] += dot(M[I],b)
                YOCTO_TAO_LOOP(M.rows,MULADDROW);
            }
            
            template <typename ARR, typename MAT, typename BRR>
            static inline void mul_sub( ARR &a, const MAT &M, const BRR &b ) throw()
            {
                assert(a.size()>=M.rows);
                assert(b.size()>=M.cols);
#define Y_TAO_MULSUBROW(I) a[I] -= dot(M[I],b)
                YOCTO_TAO_LOOP(M.rows,MULSUBROW);
            }

            
            
        };
        
    }
}

#endif
