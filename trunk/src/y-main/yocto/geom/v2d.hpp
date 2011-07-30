/** \file v2d.hpp
*/

#ifndef _YOCTO_v2d_HPP_
#define _YOCTO_v2d_HPP_ 1

#include "yocto/bitwise.hpp"
#include "yocto/random/uniform.hpp"
#include <iosfwd>

namespace yocto {

	namespace geom {

		template <class T>
		struct v2d {
			T x;
			T y;

			//----------------------------------------------------------------------
			// constructors
			//----------------------------------------------------------------------
			v2d() throw();                       //!< (0,0)
			v2d( const T X, const T Y ) throw(); //!< (X,Y)
			v2d( const v2d & ) throw();
			v2d( const v2d &A, const v2d &B) throw(); // A->B

			//----------------------------------------------------------------------
			// assign
			//----------------------------------------------------------------------
			v2d &operator=( const v2d & ) throw();

			//----------------------------------------------------------------------
			// +
			//----------------------------------------------------------------------
			v2d &operator+=( const v2d &) throw();
			v2d operator+() const throw();
			v2d operator+(const v2d & ) const throw();

			//----------------------------------------------------------------------
			// -
			//----------------------------------------------------------------------
			v2d &operator-=( const v2d &) throw();
			v2d operator-() const throw();
			v2d operator-(const v2d & ) const throw();


			//----------------------------------------------------------------------
			// * (and see template friends)
			//----------------------------------------------------------------------
			v2d &operator*=( const T ) throw();
			v2d &operator/=( const T );
			
			T norm2() const throw();
			T norm()  const throw();
			void normalize() throw();

			T angle() const throw();
			
			friend inline
			std::ostream &operator<<( std::ostream &os, const v2d<T> &v )
			{
				v.output(os);
				return os;
			}

			void output( std::ostream &os ) const;

			v2d( Random::Uniform &ran) throw();
			v2d( Random::Uniform &ran, const T radius ) throw();

			static v2d mul_( const T, const v2d & ) throw();
			static T   dot_( const v2d &, const v2d & ) throw();
			static v2d div_( const v2d &, const T );

			T &       operator[]( size_t i ) throw(); //!< i=0..1
			const T & operator[]( size_t i) const throw(); //!< i=0..1

		};

		template <class T>
		inline v2d<T>  operator*(const T a, const v2d<T> &v) throw()
		{
			return v2d<T>::mul_(a,v);
		}

		template <class T>
		inline T  operator*(const v2d<T> &lhs, const v2d<T> &rhs) throw()
		{
			return v2d<T>::dot_(lhs,rhs);
		}
		
		template <class T>
		inline v2d<T> operator/( const v2d<T> &lhs, const T a )
		{
			return v2d<T>::div_( lhs, a );
		}
		

	}

}


YOCTO_SUPPORT_C_STYLE_OPS(geom::v2d<float>)
YOCTO_SUPPORT_C_STYLE_OPS(geom::v2d<double>)
YOCTO_SUPPORT_C_STYLE_OPS(geom::v2d<ptrdiff_t>)



#endif
