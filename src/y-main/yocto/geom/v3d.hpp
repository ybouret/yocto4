/** \file v3d.hpp
 */

#ifndef _YOCTO_v3d_HPP_
#define _YOCTO_v3d_HPP_ 1

#include "yocto/geom/v2d.hpp"

namespace yocto {
	
	namespace geom {
		
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
			
			
			friend
			inline std::ostream & operator<<( std::ostream &os, const v3d &v )
			{
				v.output(os);
				return os;
			}
			
			v3d( Random::Uniform &ran) throw();
			v3d( Random::Uniform &ran, const T radius) throw();
			
			static v3d mul_( const T, const v3d & ) throw();
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
		inline v3d<T> operator^(const v3d<T> &a, const v3d<T> &b) throw()
		{
			return v3d<T>::cross_(a,b);
		}
		
	}
	
}

YOCTO_SUPPORT_C_STYLE_OPS(geom::v3d<float>)
YOCTO_SUPPORT_C_STYLE_OPS(geom::v3d<double>)
YOCTO_SUPPORT_C_STYLE_OPS(geom::v3d<int>)

#endif
