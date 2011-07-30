#ifndef YOCTO_SPADE_W3D_INCLUDED
#define YOCTO_SPADE_W3D_INCLUDED 1

#include "yocto/spade/array3d.hpp"
#include "yocto/spade/workspace.hpp"

namespace yocto
{
	
	namespace spade
	{
		template <typename T>
		class workspace3D : public workspace< array3D<T> >
		{
		public:
			YOCTO_ARGUMENTS_DECL_T;
			typedef spointer<workspace3D>   pointer;
			typedef workspace< array3D<T> > workspace_type;
			
			explicit workspace3D( const composition &C, const layout3D &L ) :
			workspace_type( C, L )
			{
			}
			
			virtual ~workspace3D() throw() {}
			
			
			
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(workspace3D);
		};
		
	}
	
}

#endif
