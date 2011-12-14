#include "yocto/ocl/driver.hpp"
#include "yocto/ocl/exception.hpp"
#include "yocto/memory/embed.hpp"

#include "yocto/ios/ostream.hpp"
#include <iostream>

namespace yocto
{
	
	namespace ocl
	{
		
		const char Driver::name[] = "OpenCL";
		Driver:: ~Driver() throw()
		{
			memory::kind<MemoryType>::release( wksp, wlen );
		}
		
		static inline cl_uint __num_platforms()
		{
			cl_uint      num_platforms = 0;
			const cl_int err = clGetPlatformIDs( 0, NULL, &num_platforms );
			if( CL_SUCCESS != err )
				throw Exception( err, "clGetPlatformIDs(&num_platforms)" );
			return num_platforms;
		}
		
		
		static inline void __build_platform( void *ptr, void *args )
		{
			assert(ptr!=NULL);
			assert(args!=NULL);
			const cl_platform_id p_id = *(cl_platform_id *)args;
			new (ptr) Platform( p_id );
		}
		
		Driver:: Driver( ) :
		num_platforms( __num_platforms() ),
		platforms(  num_platforms  ),
		Platforms_( num_platforms  ),
		Platforms( Platforms_(), num_platforms ),
		num_devices(0),
		devmap_(NULL,0),
		devices_(NULL),
		builds_(NULL),
		wksp(NULL),
		wlen(0),
		BuildLogs(NULL,0)
		{
			//==================================================================
			//
			// Load cl_platform_id
			//
			//==================================================================
			const cl_int err = clGetPlatformIDs( num_platforms, (cl_platform_id *) platforms(), NULL);
			if( CL_SUCCESS != err ) 
				throw Exception( err, "clGetPlatformIDs(%u,platforms)", num_platforms);
			
			//==================================================================
			//
			// Build All Platforms
			//
			//==================================================================
			for( cl_uint i=0; i < num_platforms; ++i ) 
			{
				Platforms( __build_platform, (void*) & platforms[i] );
				*(size_t *)(void*)&num_devices += Platforms[i].num_devices;	
				
			}
			assert( num_platforms == platforms.size );
			
			//==================================================================
			//
			// Final Memory Allocation
			//
			//==================================================================
			Allocator alloc;
			YOCTO_EMBED_START()
			{
				YOCTO_EMBED(devices_,num_devices),
				YOCTO_EMBED(builds_,num_devices)
			}
			YOCTO_EMBED_ALLOC(alloc,wksp,wlen);
			
			devmap_.reset( devices_,  num_devices );
			BuildLogs.reset( builds_, num_devices );
		}
		
		
		const Device & Driver:: operator[]( const cl_device_id device_id ) const
		{
			for( cl_uint i=0; i < num_platforms; ++i ) 
			{
				const Platform &P = Platforms[i];
				for( cl_uint j=0; j < P.num_devices; ++j )
				{
					if( P.devices[j] == device_id) return P.Devices[j];
				}
			}
			throw Exception( CL_INVALID_VALUE, "OpenCL[device_id]");
		}
		
		void Driver:: BuildLogsOut( std::ostream &os ) const
		{
			for( size_t i=0; i < BuildLogs.size; ++i )
			{
				os << BuildLogs[i] << std::endl;
			}
		}
		
		void Driver:: BuildLogsOut( ios::ostream &os ) const
		{
			for( size_t i=0; i < BuildLogs.size; ++i )
			{
				os.append(BuildLogs[i]);
				os.append( '\n' );
			}
		}
		
	}
	
}

