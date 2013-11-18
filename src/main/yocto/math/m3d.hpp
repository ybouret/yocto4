#ifndef _YOCTO_M3D_HPP_
#define _YOCTO_M3D_HPP_ 1

#include "yocto/math/v3d.hpp"
#include "yocto/math/m2d.hpp"

namespace yocto {
    
	namespace math {
        
		template <typename T>
		struct m3d {
			v3d<T> ex;
			v3d<T> ey;
			v3d<T> ez;
            
			//----------------------------------------------------------------------
			// constructors
			//----------------------------------------------------------------------
			m3d(const T diag=0) throw(); //!< [diag 0 0 ; 0 diag 0 0; 0 0 diag ]
			m3d(const m3d &) throw();
			m3d( const v3d<T> &vx, const v3d<T> &vy, const v3d<T> &vz) throw(); // [vx,vy,vz];
			~m3d() throw();
            
			//----------------------------------------------------------------------
			// assign
			//----------------------------------------------------------------------
			m3d &operator=( const m3d & ) throw();
            
			//----------------------------------------------------------------------
			// +
			//----------------------------------------------------------------------
			m3d &operator+=( const m3d &) throw();
			m3d operator+() const throw();
			m3d operator+(const m3d & ) const throw();
            
			//----------------------------------------------------------------------
			// -
			//----------------------------------------------------------------------
			m3d &operator-=( const m3d &) throw();
			m3d operator-() const throw();
			m3d operator-(const m3d & ) const throw();
            
            
			//----------------------------------------------------------------------
			// * (and see template friends)
			//----------------------------------------------------------------------
			m3d &operator*=( const T ) throw();
			m3d &operator*=( const m3d<T> &) throw();
            
			T det(void) const throw();
            
            
			//----------------------------------------------------------------------
			// special
			//----------------------------------------------------------------------
			void ortho()     throw(); //!< unsafe
			void transpose() throw();
			void inverse()   throw(); //!< unsafe
			std::ostream &to_scilab( std::ostream &os ) const;
			
			inline friend
			std::ostream & operator<<( std::ostream &os, const m3d &m )
			{
				return m.to_scilab(os);
			}
			
			//------------------------------------------------------------------
			// algebra
			//------------------------------------------------------------------
			void mul( v3d<T> & ) const throw();
			void mul( v3d<T> &target, const v3d<T> &source ) const throw();
            
            T & operator()( size_t i, size_t j ) throw();
            const T & operator()( size_t i, size_t j ) const throw();
			
		};
        
		template <class T>
		m3d<T>  operator*(const T, const m3d<T> &) throw();
        
		template <class T>
		m3d<T>  operator*(const m3d<T> &, const m3d<T> &) throw();
        
	}
    
    
}

#endif
