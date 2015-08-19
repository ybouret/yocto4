/** \file v2d.hpp
 */

#ifndef YOCTO_V2D_INCLUDED
#define YOCTO_V2D_INCLUDED 1

#include "yocto/bitwise.hpp"
#include "yocto/random/uniform.hpp"
#include <iosfwd>

namespace yocto {
    
	namespace math {
        
		template <class T>
		struct v2d {
            typedef T type;
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
			
            void ldz() throw();
			T norm2() const throw();
			T norm()  const throw();
			void normalize() throw();
            
			T angle() const throw(); //! in ]-\pi,\pi]
			T positive_angle() const throw(); //! in [0;2pi]
            
			friend inline
			std::ostream &operator<<( std::ostream &v2d_os, const v2d<T> &v )
			{
				v.output(v2d_os);
				return v2d_os;
			}
            
			void output( std::ostream &os ) const;
            
			v2d( Random::Uniform &ran) throw();
			v2d( Random::Uniform &ran, const T radius ) throw();
            
			static v2d mul_( const T, const v2d & ) throw();
			static T   dot_( const v2d &, const v2d & ) throw();
			static v2d div_( const v2d &, const T );
            
			T &       operator[]( size_t i ) throw(); //!< i=0..1
			const T & operator[]( size_t i) const throw(); //!< i=0..1
            
            static
            T angle_of( const v2d &a, const v2d &b ) throw();
            
            static
            T det( const v2d &a, const v2d &b) throw();
            
            v2d rotated_by( T angle ) const throw();
            
            static T dist2( const v2d &, const v2d &) throw();
            static T dist(const v2d &, const v2d & ) throw();
            
            static const v2d<T> zero;
            
            static inline
            int lexicographical_compare( const v2d &lhs, const v2d &rhs ) throw()
            {
                if( lhs.x < rhs. x)
                    return -1;
                else
                {
                    if(rhs.x<lhs.x)
                        return 1;
                    else
                    {
                        // lhs.x == rhs.x
                        return lhs.y < rhs.y ? -1 : ( rhs.y < lhs.y ? 1 : 0 );
                    }
                }
            }
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


YOCTO_SUPPORT_C_STYLE_OPS(math::v2d<float>)
YOCTO_SUPPORT_C_STYLE_OPS(math::v2d<double>)
YOCTO_SUPPORT_C_STYLE_OPS(math::v2d<unit_t>)



#endif
