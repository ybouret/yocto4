//! \file

#ifndef YOCTO_CLIFF_ARRAY1D_INCLUDED
#define YOCTO_CLIFF_ARRAY1D_INCLUDED 1

#include "yocto/cliff/linear.hpp"
#include "yocto/cliff/in1d.hpp"
#include "yocto/functor.hpp"

#include <iosfwd>

namespace yocto
{
	
	namespace cliff
	{	
		
#define YOCTO_CLIFF_ARRAY_OPS(ARRAY) \
inline void set( const ARRAY &other, const layout_type &sub ) throw() { foreach( other, sub, linear_type::set2_cb, NULL); } \
inline void add( const ARRAY &other, const layout_type &sub ) throw() { foreach( other, sub, linear_type::add2_cb, NULL); } \
inline void muladd( param_type k, const ARRAY &other, const layout_type &sub ) throw() { foreach( other, sub, linear_type::muladd_cb, (void*)&k); }
		
		//! one dimension array of float/double/complex...
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
			
			typedef typename linear_type::callback_type  callback_type;
			typedef typename linear_type::const_cb_type  const_cb_type;
			typedef typename linear_type::callback2_type callback2_type;			
			virtual void foreach( const layout1D &sub, callback_type  proc, void *args);
			virtual void foreach( const layout1D &sub, const_cb_type  proc, void *args) const;
			void         foreach( const array1D &other, const layout1D &sub, callback2_type proc, void *args);

			
			template <typename U>
			friend std::ostream & operator<<( std::ostream &os, const array1D<U> &a);
			
			YOCTO_CLIFF_ARRAY_OPS(array1D)
			
		private:
			type *item_; //!< offseted entry
			YOCTO_DISABLE_COPY_AND_ASSIGN(array1D);
		};
	}
}

#endif
