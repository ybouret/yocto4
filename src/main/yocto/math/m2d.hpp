#ifndef _YOCTO_M2D_HPP_
#define _YOCTO_M2D_HPP_ 1

#include "yocto/math/v2d.hpp"

namespace yocto {
    
	namespace math {
        
		template <typename T>
		struct m2d {
			v2d<T> ex;
			v2d<T> ey;
            
			//----------------------------------------------------------------------
			// constructors
			//----------------------------------------------------------------------
			m2d(const T diag=0) throw(); //!< [diag 0 ; 0 diag ]
			m2d(const m2d &) throw();
			m2d( const v2d<T> &diag ) throw();
			m2d( const v2d<T> &vx, const v2d<T> &vy) throw(); // [vx,vy];
			~m2d() throw();
            
			//----------------------------------------------------------------------
			// assign
			//----------------------------------------------------------------------
			m2d &operator=( const m2d & ) throw();
            
			//----------------------------------------------------------------------
			// +
			//----------------------------------------------------------------------
			m2d &operator+=( const m2d &) throw();
			m2d operator+() const throw();
			m2d operator+(const m2d & ) const throw();
            
			//----------------------------------------------------------------------
			// -
			//----------------------------------------------------------------------
			m2d &operator-=( const m2d &) throw();
			m2d operator-() const throw();
			m2d operator-(const m2d & ) const throw();
            
            
			//----------------------------------------------------------------------
			// * (and see template friends)
			//----------------------------------------------------------------------
			m2d &operator*=( const T ) throw();
			m2d &operator*=( const m2d<T> &) throw();
            
			T    det(void) const throw();
			void ortho()     throw();
			void transpose() throw();
			void inverse()   throw(); //!< unsafe
			std::ostream &to_scilab( std::ostream &os ) const;
			
			inline friend
			std::ostream & operator<<( std::ostream &os, const m2d &m )
			{
				return m.to_scilab(os);
			}
			
			void symmetric() throw(); //!< make the matrix symmetric
            
			//! jacobi tranform
			/**
             m = Q D tQ
             \return the rotation angle.
             */
			T   jacobi( m2d<T> &q, v2d<T> &lam ) throw();
            
			//----------------------------------------------------------------------
			// algebra
			//----------------------------------------------------------------------
			void mul( v2d<T> & ) const throw();
			void mul( v2d<T> &target, const v2d<T> &source ) const throw();
            
		};
        
		template <class T>
		m2d<T>  operator*(const T, const m2d<T> &) throw();
        
		template <class T>
		m2d<T>  operator*(const m2d<T> &, const m2d<T> &) throw();
        
        
	}
    
    
}

#endif
