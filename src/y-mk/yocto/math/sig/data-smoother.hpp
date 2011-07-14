#ifndef YOCTO_DATA_SMOOTHER_INCLUDED
#define YOCTO_DATA_SMOOTHER_INCLUDED 1


#include "yocto/math/sig/data-extender.hpp"
#include "yocto/math/types.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/math/kernel/matrix.hpp"
#include "yocto/sequence/list.hpp"

namespace yocto
{
	
	namespace math 
	{
		
		template <class T>
		class smoother {
		public:
			virtual ~smoother() throw();
			explicit smoother(
							  const size_t nl,
							  const size_t nr,
							  T          (*K)(T),
							  const size_t d);
			
			size_t degree() const throw();
			
			
			void run(array<T>                &odata,
					 const extender<T>       &idata,
					 const size_t             level) const;
			
			static T uniform( T x );
			static T triangle( T x );
			static T quadratic( T x );
			static T quartic( T x );
			static T triweight( T x );
			static T gaussian( T x );
			static T cosine( T x );
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(smoother);
			typedef vector<T> array_t;
			
			const ptrdiff_t    nl_;      //!< #left  points
			const ptrdiff_t    nr_;      //!< #right points
			const size_t       np_;      //!< nr+nl+1: #points
			array_t            weight_;  //!< weights from -nl to nr
			const size_t       degree_;  //!< fit degree
			const size_t       ncoeff_;  //!< degree+1
			matrix<T>          filter_;  //!< ncoeff_ X np_
			mutable list<T>    dataQ_;   //!< for processing
		};
		
#if defined(__DMC__)
		template class smoother<float>;
		template class smoother<double>;
#endif
		
	}
	
}

#endif
