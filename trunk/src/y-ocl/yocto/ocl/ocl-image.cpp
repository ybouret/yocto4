#include "yocto/ocl/image.hpp"
#include "yocto/ocl/_query.hpp"

namespace yocto
{
	namespace ocl
	{
		
		namespace Core 
		{
			
#define YOCTO_CLI_(FIELD,OF) FIELD( GetInfoValue( type2type<OF>(), clGetImageInfo, img, CL_IMAGE_##FIELD, "CL_IMAGE_" #FIELD ) )
			
			
			Image:: Image(  cl_mem img ) :
			MemObject( img ),
			YOCTO_CLI_(FORMAT,cl_image_format),
			YOCTO_CLI_(ELEMENT_SIZE,size_t),
			YOCTO_CLI_(ROW_PITCH,size_t),
			YOCTO_CLI_(SLICE_PITCH,size_t),
			YOCTO_CLI_(WIDTH,size_t),
			YOCTO_CLI_(HEIGHT,size_t),
			YOCTO_CLI_(DEPTH,size_t)
			{
			}
			
			Image:: ~Image() throw()
			{
			}
			
			
		}
		
		
		Image:: ~Image() throw()
		{
		}
		
		Image:: Image( cl_mem img ) :
		MemObject( img ),
		pImg_( new Core::Image(img) )
		{
		}
		
		Image:: Image( const Image &other ) throw() :
		MemObject( other ),
		pImg_( other.pImg_ )
		{
		}
		
		const Core::Image * Image:: operator->() const throw()
		{
			return & (*pImg_);
		}
		
		
		
	}
	
}

#include "yocto/ocl/kernel.hpp"
namespace yocto
{
	
	namespace ocl
	{
		void Kernel:: SetArg( cl_uint arg_index, Image & __memobj )
		{
			SetArg( arg_index, sizeof(cl_mem), (void *) & ( __memobj->ASSOCIATED_MEMOBJECT ) );
		}
	}
}

