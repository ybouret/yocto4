/** \file interpolate.hpp
*/

#ifndef _YOCTO_INTERPOLATE_HPP_
#define _YOCTO_INTERPOLATE_HPP_ 1

#include "yocto/math/types.hpp"
#include "yocto/sequence/vector.hpp"

namespace yocto {

	namespace math {

		template <class T>
		class interpolate : public object {
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

		//YOCTO_DMC_MATH(class interpolate)

	}

}

#endif
