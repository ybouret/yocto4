#ifndef YOCTO_DATA_EXTENDER_INCLUDED
#define YOCTO_DATA_EXTENDER_INCLUDED 1

#include "yocto/sequence/array.hpp"

namespace yocto
{
	
	
	namespace math 
	{
		
		
		enum extension_type
		{
		    extension_constant,
		    extension_cyclic,
		    extension_reflect,
		    extension_symmetric
		};
		
		
		template <class T>
		class extender : public object {
		public:
			
			explicit extender(const array<T>      &data,
							  const extension_type dxlo,
							  const extension_type dxup  );
			virtual ~extender() throw();
			
			
			T operator[]( ptrdiff_t i ) const throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(extender);
			
			const array<T>      &data_; //!< proxy to a sequence
			const ptrdiff_t      size_; //!< signed
			const extension_type dxlo_;
			const extension_type dxup_;
			const T              tdlo_; //! twice data lower
			const T              tdup_; //! twice data upper
		};
		
		
	}
	
}

#endif
