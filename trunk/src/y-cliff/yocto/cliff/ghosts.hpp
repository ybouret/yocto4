#ifndef YOCTO_CLIFF_GHOSTS_INCLUDED
#define YOCTO_CLIFF_GHOSTS_INCLUDED 1


#include "yocto/cliff/layout.hpp"


namespace yocto
{
	
	namespace cliff
	{
		
		class ghosts_base
		{
		public:
			virtual ~ghosts_base() throw();
			
			offsets_list offsets;
			
		protected:
			explicit ghosts_base(size_t max_offsets );
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(ghosts_base);
		};
		
		template <typename COORD>
		class ghosts :  public layout<COORD>, public ghosts_base
		{
		public:
			typedef typename layout<COORD>::param_coord param_coord;
			
			explicit ghosts( param_coord lo, param_coord hi , const layout <COORD> &outline ) :
			layout<COORD>( lo, hi ),
			ghosts_base( this->items )
			{
				assert( outline.has(this->lower) );
				assert( outline.has(this->upper) );
				layout<COORD>::load_offsets( this->offsets, *this, outline );
			}
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(ghosts);
		};
		
	}
	
}

#endif

