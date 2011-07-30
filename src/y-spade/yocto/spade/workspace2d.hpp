#ifndef YOCTO_SPADE_W2D_INCLUDED
#define YOCTO_SPADE_W2D_INCLUDED 1

#include "yocto/spade/array2d.hpp"
#include "yocto/spade/workspace.hpp"

namespace yocto
{
	
	namespace spade
	{
		template <typename T>
		class workspace2D : public workspace< array2D<T> >
		{
		public:
			YOCTO_ARGUMENTS_DECL_T;
			typedef spointer<workspace2D>   pointer;
			typedef workspace< array2D<T> > workspace_type;
			
			explicit workspace2D( const composition &C, const layout2D &L ) :
			workspace_type( C, L )
			{
			}
			
			virtual ~workspace2D() throw() {}
			
			
			
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(workspace2D);
		};
		
	}
	
}

#endif
