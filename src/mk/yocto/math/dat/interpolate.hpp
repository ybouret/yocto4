/** \file interpolate.hpp
 */

#ifndef _YOCTO_INTERPOLATE_HPP_
#define _YOCTO_INTERPOLATE_HPP_ 1

#include "yocto/math/types.hpp"
#include "yocto/sequence/vector.hpp"

namespace yocto {
    
	namespace math {
        
		template <class T>
		class interpolate
        {
		public:
			explicit interpolate() throw();
			explicit interpolate(const size_t);
			void     acquire( const size_t nvar );
			void     release() throw();
            
			virtual ~interpolate() throw();
            
			T polint( const T x, const array<T> &xa, const array<T> &ya, T &dy ) const;
			T ratint( const T x, const array<T> &xa, const array<T> &ya, T &dy ) const;
            
            
		private:
			mutable vector<T> wksp_;
			YOCTO_DISABLE_COPY_AND_ASSIGN(interpolate);
		};
        
        template <typename T>
        class interpolator
        {
        public:
            explicit interpolator() throw();
            explicit interpolator(size_t npoints);
            virtual ~interpolator() throw();
            
            void reserve(size_t npoints);
            
            void insert( T x, T y );
            void free() throw();
            void optimize() throw();
            
            //! must have > 0 points
            T    polint( T x );
            
            //! must have > 0 points
            T    ratint( T x );
            
        private:
            vector<T>      xa;
            vector<T>      ya;
            interpolate<T> ip;
            YOCTO_DISABLE_COPY_AND_ASSIGN(interpolator);
        public:
            T dy; //!< last error
        };
	}
    
}

#endif
