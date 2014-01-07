#ifndef YOCTO_MATH_KERNEL_ALGEBRA_INCLUDED
#define YOCTO_MATH_KERNEL_ALGEBRA_INCLUDED 1

#include "yocto/math/kernel/matrix.hpp"
#include "yocto/math/types.hpp"

namespace yocto
{
	namespace math
	{
		
		template <typename T>
		struct algebra
		{
			static void set(    array<T> &v, T a ) throw();                     //!< v = a
            static void mul(    array<T> &v, T a ) throw();                     //!< v *= a
			static void set(    array<T> &v, const array<T> &u ) throw();       //!< v = u
			static void add(    array<T> &v, const array<T> &u ) throw();       //!< v += u
            static void sub(    array<T> &v, const array<T> &u ) throw();       //!< v  = v-u
            static void subp(   array<T> &v, const array<T> &u ) throw();       //!< v  = u-v
            static void vec(    array<T> &v, const array<T> &a, const array<T> &b) throw(); //!< v = b-a
			static void muladd( array<T> &v, T a, const array<T> &u ) throw();  //!< v += a * u
			static void mulset( array<T> &v, T a, const array<T> &u ) throw();  //!< v  = a * u
            static void neg( array<T> &v, const array<T> &u ) throw();          //!< v  = -u
            
			static void mul( array<T> &v, const matrix<T> &M, const array<T> &u ) throw();
			static void muladd( array<T> &v, const matrix<T> &M, const array<T> &u ) throw();
			static void mulsub( array<T> &v, const matrix<T> &M, const array<T> &u ) throw();

			static void mul_trn( array<T> &v, const matrix<T> &M, const array<T> &u ) throw();
            static void muladd_trn( array<T> &v, const matrix<T> &M, const array<T> &u ) throw();
            static void mulsub_trn( array<T> &v, const matrix<T> &M, const array<T> &u ) throw();

			static void mul( matrix<T> &M, const matrix<T> &lhs, const matrix<T> &rhs) throw();
			static void mul_ltrn( matrix<T> &M, const matrix<T> &lhs, const matrix<T> &rhs) throw();
			static void mul_rtrn( matrix<T> &M, const matrix<T> &lhs, const matrix<T> &rhs) throw();
			static void gram_schmidt( matrix<T> &U, const matrix<T> &V, bool normalize);
            
            template <typename U>
            static inline T norm2( const array<U> &v ) throw()
            {
                T ans(0);
                for( size_t i=v.size();i>0;--i) ans += Square( v[i] );
                return Sqrt(ans);
            }
            
            template <typename U>
            static inline T norm1( const array<U> &v ) throw()
            {
                T ans(0);
                for( size_t i=v.size();i>0;--i) ans += Fabs( v[i] );
                return ans;
            }
            
            template <typename U>
            static inline T norm_infty( const array<U> &v ) throw()
            {
                T ans(0.0);
                for(size_t i=v.size();i>0;--i)
                {
                    const T tmp = Fabs(v[i]);
                    if(tmp>ans) ans=tmp;
                }
                return ans;
            }
            
            static T dot( const array<T> &u, const array<T> &v ) throw();
            static void normalize( array<T> &v );
            static T sum( const array<T> &u ) throw();
            
		};
        
        
	}
	
}

#endif
