#ifndef YOCTO_CLIFF_GHOSTS_INCLUDED
#define YOCTO_CLIFF_GHOSTS_INCLUDED 1


#include "yocto/cliff/layout.hpp"
#include "yocto/sequence/vector.hpp"

namespace yocto
{
	
	namespace cliff
	{
		
		//! base class for ghosts: their offset
		class ghosts_base
		{
		public:
			virtual ~ghosts_base() throw();
			
			const offsets_list offsets;
			
		protected:
			explicit ghosts_base(size_t max_offsets );
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(ghosts_base);
		};
		
		//! ghosts: sub layout and offsets
		template <typename T,typename COORD>
		class ghosts :  public layout<COORD>, public ghosts_base
		{
		public:
			typedef typename layout<COORD>::param_coord param_coord;
			
			
			
			explicit ghosts( param_coord lo, param_coord hi , const layout <COORD> &outline ) :
			layout<COORD>( lo, hi ),
			ghosts_base( this->items ),
			iodata_()
			{
				static const T __ini(0);
				assert( outline.has(this->lower) );
				assert( outline.has(this->upper) );
				layout<COORD>::load_offsets( (offsets_list &)(this->offsets), *this, outline );
				iodata_.make( this->offsets.size(), __ini );
			}
			
			virtual ~ghosts() throw() {}

		private:
			vector<T> iodata_;
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(ghosts);
		};
		
		
		
		
	}
	
}

#endif

