#include "yocto/ocl/device.hpp"
#include "yocto/ocl/_query.hpp"

namespace yocto
{
	namespace ocl
	{
		
		Device:: ~Device() throw()
		{
		}
		
		
		static inline
		string __name_for( const string &NAME )
		{
			string name( NAME.size(), as_capacity );
			char   prev = 0;
			for( size_t i=0; i < NAME.size(); ++i )
			{
				char curr = NAME[i];
				switch( curr )
				{
					case ' ':
					case '(':
					case ')':
						curr = '_';
						break;
						
					default:
						break;
				}
				if( curr != '_' || prev != '_' )
					name.append( curr );
				prev = curr;
			}
			return name;
		}
		
#define YCLD_GET(TYPE,FIELD) FIELD( Core::GetInfoValue( type2type<TYPE>(), clGetDeviceInfo, id, CL_DEVICE_ ## FIELD, "CL_DEVICE_" #FIELD ) )
#define YCLD_STR(FIELD)      FIELD( Core::GetInfoString( clGetDeviceInfo, id, CL_DEVICE_ ## FIELD, "CL_DEVICE_" #FIELD ) )
	
		Device:: Device( cl_device_id device ) :
		id( device ),
		YCLD_GET(cl_bool,COMPILER_AVAILABLE),
		YCLD_GET(cl_bool,ERROR_CORRECTION_SUPPORT),
		YCLD_STR(EXTENSIONS),
		extensions( Core::CountExtensions( EXTENSIONS ) ),
		YCLD_GET(cl_ulong,GLOBAL_MEM_CACHE_SIZE),
		YCLD_GET(cl_ulong,GLOBAL_MEM_SIZE),
		YCLD_GET(cl_bool,IMAGE_SUPPORT),
		YCLD_GET(cl_ulong,LOCAL_MEM_SIZE),
		YCLD_GET(cl_uint,MAX_CLOCK_FREQUENCY),
		YCLD_GET(cl_uint,MAX_COMPUTE_UNITS),
		YCLD_GET(cl_ulong,MAX_CONSTANT_BUFFER_SIZE),
		YCLD_GET(cl_ulong,MAX_MEM_ALLOC_SIZE),
		YCLD_GET(size_t,MAX_WORK_GROUP_SIZE),
		YCLD_GET(cl_uint,MAX_WORK_ITEM_DIMENSIONS),
		MAX_WORK_ITEM_SIZES( MAX_WORK_ITEM_DIMENSIONS ),
		YCLD_STR(NAME),
		name( __name_for(NAME) ),
		YCLD_GET(cl_platform_id,PLATFORM),
		YCLD_STR(PROFILE),
		YCLD_GET(size_t,PROFILING_TIMER_RESOLUTION),
		YCLD_GET(cl_command_queue_properties,QUEUE_PROPERTIES),
		YCLD_GET(cl_device_type,TYPE),
		YCLD_STR(VENDOR),
		YCLD_STR(VENDOR_ID),
		YCLD_STR(VERSION),
		DRIVER_VERSION( Core::GetInfoString( clGetDeviceInfo, id, CL_DRIVER_VERSION, "CL_DRIVER_VERSION" ) )
		{
			
			//== read MAX_WORK_ITEM_SIZES
			const int err = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_SIZES, MAX_WORK_ITEM_SIZES.bytes, (void*) MAX_WORK_ITEM_SIZES(), NULL);
			if( CL_SUCCESS != err ) throw Exception( err, "CL_DEVICE_MAX_WORK_ITEM_SIZES" );
			
			//== parse extensions
			Core:: ParseExtensions(EXTENSIONS, extensions);
		}
		
		bool Device:: is_available() const
		{
			return Core:: GetInfoValue( type2type<cl_bool>(), clGetDeviceInfo, id, CL_DEVICE_AVAILABLE, "CL_DEVICE_AVAILABLE" );
		}
		
		
	}
}

