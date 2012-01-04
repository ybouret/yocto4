#ifndef YOCTO_CLIFF_LAYOUT_INCLUDED
#define YOCTO_CLIFF_LAYOUT_INCLUDED 1

#include "yocto/cliff/types.hpp"
#include "yocto/type-traits.hpp"

namespace yocto
{
	
	namespace cliff
	{
		
		template <typename COORD>
		class layout : public layout_base
		{
		public:
			typedef COORD                                        coord_type;
			typedef typename type_traits<COORD>::mutable_type    mutable_coord;
			typedef const     mutable_coord                      const_coord;
			typedef typename type_traits<COORD>::parameter_type  param_coord;
			
			static const size_t DIMENSIONS = sizeof(COORD)/sizeof(unit_t);
			
			const_coord   lower; //!< lower coordinate
			const_coord   upper; //!< upper coordinate
			const_coord   width; //!< coordinate width
			const size_t  items; //!< number of linear items within the layout = items stride
			
			
			//! prepare layout
			inline explicit layout( param_coord lo, param_coord hi ) throw() :
			layout_base( DIMENSIONS ),
			lower( lo ),
			upper( hi ),
			width( ),
			items( layout_base::setup( &lower, &upper, &width ) )
			{
				
			}
			
			//! copy layout
			inline layout( const layout &other ) throw() :
			layout_base( DIMENSIONS ),
			lower( other.lower ),
			upper( other.upper ),
			width( other.width ),
			items( other.items )
			{
			}
			
			inline virtual ~layout() throw() {}
			
			
			inline bool has( param_coord u ) const throw()
			{
				const unit_t *l = (const unit_t *) &lower;
				const unit_t *h = (const unit_t *) &upper;
				const unit_t *q = (const unit_t *) &u;
				for( size_t i=0; i < DIMENSIONS; ++i )
				{
					const unit_t c = q[i];
					if( c < l[i] || c > h[i] ) return false;
				}
				return true;
			}
			
			
		private:
			YOCTO_DISABLE_ASSIGN(layout);
		};
		
	}
}

#endif
