#ifndef YOCTO_CLIFF_ARRAY1D_INCLUDED
#define YOCTO_CLIFF_ARRAY1D_INCLUDED 1

#include "yocto/cliff/linear.hpp"
#include "yocto/cliff/in1d.hpp"


namespace yocto
{
	
	namespace cliff
	{	
		
		template <typename T>
		class array1D : public linear<T,layout1D>
		{
		public:
			YOCTO_ARGUMENTS_DECL_T;
			typedef linear<T,layout1D> linear_type;
			typedef layout1D           layout_type;
			
			explicit array1D( const layout_type &L ) throw();
			virtual ~array1D() throw();
			
			type       & operator[]( unit_t x ) throw();
			const_type & operator[]( unit_t x ) const throw();
			
			virtual void link( T *addr ) throw();
			
			
			
		private:
			type *item_; //!< offseted entry
			YOCTO_DISABLE_COPY_AND_ASSIGN(array1D);
		};
	}
}

#endif
