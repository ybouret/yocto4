#include "yocto/ocl/device.hpp"
#include "yocto/ocl/_query.hpp"

namespace yocto 
{
	namespace ocl {
		
		namespace Core 
		{
			
			static inline
			string _create_nickname( const string &name )
			{
				static const char repl = '_';
				string ans( name.size(), as_capacity );
				char   prev = 0;
				for( size_t i=0; i < name.size(); ++i )
				{
					char curr = name[i];
					
					switch( curr )
					{
						case ' ':
						case '(':
						case ')':
						case '@':
							curr = repl;
							break;
							
						default:
							break;
					}
					
					if( repl != curr || prev != curr )
						ans.append( curr );
					
					prev = curr;

				}
				return ans;
			}
			
#define YOCTO_CLD_(FIELD,OF) FIELD( GetInfoValue( type2type<OF>(), clGetDeviceInfo, id, CL_DEVICE_##FIELD, "CL_DEVICE_" #FIELD ) )
#define YOCTO_CLD_str(X)     X( GetInfoString( clGetDeviceInfo, id, CL_DEVICE_##X, "CL_DEVICE_" #X ) )
			
			Device:: Device( cl_device_id __id ) :
			id( __id ),
			YOCTO_CLD_(PLATFORM,cl_platform_id),
			YOCTO_CLD_(TYPE,cl_device_type),
			YOCTO_CLD_(VENDOR_ID,cl_uint),
			YOCTO_CLD_(MAX_COMPUTE_UNITS,cl_uint),
			
			YOCTO_CLD_(MAX_WORK_ITEM_DIMENSIONS,cl_uint),
			MAX_WORK_ITEM_SIZES( MAX_WORK_ITEM_DIMENSIONS ),
			YOCTO_CLD_(MAX_WORK_GROUP_SIZE,size_t),
			
			YOCTO_CLD_(MAX_CLOCK_FREQUENCY,cl_uint),
			
			YOCTO_CLD_(ADDRESS_BITS,cl_uint),
			
			YOCTO_CLD_(IMAGE_SUPPORT,cl_bool),
			YOCTO_CLD_(MAX_READ_IMAGE_ARGS,cl_uint),
			YOCTO_CLD_(MAX_WRITE_IMAGE_ARGS,cl_uint),
			YOCTO_CLD_(IMAGE2D_MAX_WIDTH,size_t),
			YOCTO_CLD_(IMAGE2D_MAX_HEIGHT,size_t),
			YOCTO_CLD_(IMAGE3D_MAX_WIDTH,size_t),
			YOCTO_CLD_(IMAGE3D_MAX_HEIGHT,size_t),
			YOCTO_CLD_(IMAGE3D_MAX_DEPTH,size_t),
			YOCTO_CLD_(MAX_SAMPLERS,cl_uint),
			
			YOCTO_CLD_(MAX_MEM_ALLOC_SIZE,cl_ulong),
			YOCTO_CLD_(GLOBAL_MEM_SIZE,cl_ulong),
			YOCTO_CLD_(LOCAL_MEM_SIZE,cl_ulong),
			YOCTO_CLD_str(NAME),
			YOCTO_CLD_str(VENDOR),
			YOCTO_CLD_str(VERSION),
			DRIVER( GetInfoString( clGetDeviceInfo, id, CL_DRIVER_VERSION, "CL_DRIVER_VERSION" )),
			YOCTO_CLD_str(PROFILE),
			YOCTO_CLD_str(EXTENSIONS),
			extension( CountExtensions(EXTENSIONS) ),
			nickname( _create_nickname(NAME) )
			{
				//--------------------------------------------------------------
				// final update
				//--------------------------------------------------------------
				ReadInfoValue(clGetDeviceInfo,
							  id,
							  CL_DEVICE_MAX_WORK_ITEM_SIZES,
							  (void*)MAX_WORK_ITEM_SIZES(0),
							  MAX_WORK_ITEM_SIZES.bytes,
							  "CL_DEVICE_MAX_WORK_ITEM_SIZE");
				//--------------------------------------------------------------
				// parse extensions
				//--------------------------------------------------------------
				ParseExtensions(EXTENSIONS, extension );
			}
			
			
			Device:: ~Device() throw()
			{
			}
		}
		
		
		Device:: Device( cl_device_id __id ) :
		pDevice_( new Core::Device( __id ) )
		{
		}
		
		Device:: ~Device() throw()
		{
		}
		
		
		Device:: Device( const Device &other ) throw() :
		pDevice_( other.pDevice_ )
		{
			
		}
		
		const Core::Device * Device:: operator->() const throw()
		{
			return & (*pDevice_);
		}
		
	}
}
