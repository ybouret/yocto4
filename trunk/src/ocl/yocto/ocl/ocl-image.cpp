#include "yocto/ocl/image.hpp"
#include "yocto/ocl/_query.hpp"

namespace yocto
{
	namespace ocl
	{
		
		
#define YOCTO_CLI_(FIELD,OF) FIELD( Core::GetInfoValue( type2type<OF>(), clGetImageInfo, img, CL_IMAGE_##FIELD, "CL_IMAGE_" #FIELD ) )
		
		
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
		
		Image:: Image( const Image &other ) throw() :
		MemObject( other ),
		FORMAT( other.FORMAT ),
		ELEMENT_SIZE( other.ELEMENT_SIZE ),
		ROW_PITCH( other.ROW_PITCH ),
		SLICE_PITCH( other.SLICE_PITCH ),
		WIDTH( other.WIDTH ),
		HEIGHT( other.HEIGHT ),
		DEPTH( other.DEPTH )
		{
		}
		
		
		
	}
	
}



