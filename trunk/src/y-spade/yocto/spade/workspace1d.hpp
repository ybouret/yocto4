#ifndef YOCTO_SPADE_W1D_INCLUDED
#define YOCTO_SPADE_W1D_INCLUDED 1

#include "yocto/spade/array1d.hpp"
#include "yocto/spade/workspace.hpp"
#include "yocto/shared-ptr.hpp"

namespace yocto
{
	
	namespace spade
	{
		template <typename T>
		class workspace1D : public workspace< array1D<T> >
		{
		public:
			YOCTO_ARGUMENTS_DECL_T;
			typedef shared_ptr<workspace1D>  pointer;
			typedef workspace< array1D<T> >  workspace_type;
			
			explicit workspace1D( const composition &C, const layout1D &L ) :
			workspace_type( C, L )
			{
			}
			
			virtual ~workspace1D() throw() {}
			
			
			
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(workspace1D);
		};
		
	}
	
}

#endif
