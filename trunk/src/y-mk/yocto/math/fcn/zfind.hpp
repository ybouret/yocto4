/** \file  zfind.hpp
    \brief Root Finding by Ridders' Method.
*/

#ifndef YOCTO_ZFIND_INCLUDED
#define YOCTO_ZFIND_INCLUDED 1

#include "yocto/math/triplet.hpp"

namespace yocto {

	namespace math {

		//! find zero between x.a and x.c
		/**
			f.a and f.c must be provided and f.a * f.c <= 0
			xerr>=0
		*/
		template <typename T>
		class zfind
		{
        public:
            explicit zfind( T default_xerr ) throw();
            virtual ~zfind() throw();
            
			T xerr; //!< error on x

			//! find zero between x.a and x.c
			/**
				f.a and f.c must be provided and f.a * f.c <= 0
			*/
			void run( typename numeric<T>::function &proc, triplet<T> &x, triplet<T> &f) const;
			T    get( typename numeric<T>::function &proc, T xlo, T xhi ) const;

			inline T operator()( typename numeric<T>::function &proc, T xlo, T xhi ) const
			{
				return this->get( proc, xlo, xhi );
			}

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(zfind);
		};

		//YOCTO_DMC_MATH(struct zfind)
	}

}

#endif
