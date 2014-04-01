/** \file v3d.hpp
 */

#ifndef YOCTO_V3D_INCLUDED
#define YOCTO_V3D_INCLUDED 1

#include "yocto/math/v2d.hpp"

namespace yocto {
	
	namespace math {
		
		template <class T>
		struct v3d {
			T x;
			T y;
			T z;
			
			//----------------------------------------------------------------------
			// constructors
			//----------------------------------------------------------------------
			v3d() throw();                       //!< (0,0)
			v3d( const T X, const T Y, const T Z) throw(); //!< (X,Y)
			v3d( const v3d & ) throw();
			v3d( const v3d &A, const v3d &B) throw(); // A->B
			
			//----------------------------------------------------------------------
			// assign
			//----------------------------------------------------------------------
			v3d &operator=( const v3d & ) throw();
			
			//----------------------------------------------------------------------
			// +
			//----------------------------------------------------------------------
			v3d &operator+=( const v3d &) throw();
			v3d operator+() const throw();
			v3d operator+(const v3d & ) const throw();
			
			//----------------------------------------------------------------------
			// -
			//----------------------------------------------------------------------
			v3d &operator-=( const v3d &) throw();
			v3d operator-() const throw();
			v3d operator-(const v3d & ) const throw();
			
			
			//----------------------------------------------------------------------
			// * (and see template friends)
			//----------------------------------------------------------------------
			v3d &operator*=( const T ) throw();
			
			T norm2() const throw();
			T norm()  const throw();
			void normalize() throw();
			void ldz() throw();
			
			friend
			inline std::ostream & operator<<( std::ostream &v3d_os, const v3d &v )
			{
				v.output(v3d_os);
				return v3d_os;
			}
			
			v3d( Random::Uniform &ran) throw();
			v3d( Random::Uniform &ran, const T radius) throw();
			
			static v3d mul_( const T, const v3d & ) throw();
            static v3d div_( const v3d &, const T );

			static T   dot_( const v3d &, const v3d & ) throw();
			static v3d cross_( const v3d &, const v3d & ) throw();
			void output( std::ostream & ) const;
			
			T &       operator[]( size_t i ) throw(); //!< i=0..2
			const T & operator[]( size_t i) const throw(); //!< i=0..2
			
		};
		
		template <class T>
		inline v3d<T>  operator*(const T a, const v3d<T> &v) throw()
		{
			return v3d<T>::mul_(a,v);
		}
		
		template <class T>
		inline T  operator*(const v3d<T> &a, const v3d<T> &b) throw()
		{
			return v3d<T>::dot_(a,b);
		}
		
        template <class T>
		inline v3d<T> operator/( const v3d<T> &lhs, const T a )
		{
			return v3d<T>::div_( lhs, a );
		}
        
		template <class T>
		inline v3d<T> operator^(const v3d<T> &a, const v3d<T> &b) throw()
		{
			return v3d<T>::cross_(a,b);
		}
		
        
	}
	
}

YOCTO_SUPPORT_C_STYLE_OPS(math::v3d<float>)
YOCTO_SUPPORT_C_STYLE_OPS(math::v3d<double>)
YOCTO_SUPPORT_C_STYLE_OPS(math::v3d<ptrdiff_t>)

#endif
