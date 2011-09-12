#include "yocto/ocl/driver.hpp"
#include "yocto/ocl/exception.hpp"

#include "yocto/memory/embed.hpp"

#include <iostream>

namespace yocto
{
	namespace threading
	{
		template <>
		mutex threaded_class< ocl::Driver >::mutex_( "OpenCL" );
	}
	
	namespace ocl
	{
		
		Driver:: ~Driver() throw()
		{
			DeleteDeviceMapper();
			build_log.free();
			memory::pooled_release( wksp_ );
		}
		
		
		static inline 
		cl_uint __GetPlatformIDsCount()
		{
			cl_uint      ans = 0;
			const cl_int err = clGetPlatformIDs(0, NULL, &ans);
			if( err != CL_SUCCESS || ans <= 0 ) throw Exception( err, "clGetPlatformIDs(count)");
			return ans;
		}
		
		static inline
		void __GetPlatformIDsValue( const cl_uint num_entries, cl_platform_id *platform )
		{
			assert( num_entries > 0 );
			assert( platform   != NULL );
			
			cl_uint      chk = 0;
			const cl_int err = clGetPlatformIDs( num_entries, platform, &chk );
			if( err != CL_SUCCESS || num_entries != chk ) throw Exception( err, "clGetPlatformIDs(%u)", num_entries);
		}
		
		Driver:: Driver() :
		num_platforms( __GetPlatformIDsCount() ),
		platform_id( num_platforms ),
		platform_(   num_platforms ),
		platform( platform_(0), platform_.size ),
		max_devices(0),
		device_map(NULL,0 ),
		build_log(NULL, 0 ),
		wksp_(NULL),
		device_id_(NULL),
		build_log_(NULL),
		mapper_( NULL )
		{
			
			assert( platform.maxi == num_platforms );
			
			//------------------------------------------------------------------
			// Read the plaform Ids
			//------------------------------------------------------------------
			__GetPlatformIDsValue( num_platforms, (cl_platform_id *) platform_id(0) );
			
			//------------------------------------------------------------------
			// construct the platforms
			//------------------------------------------------------------------
			cl_uint & sum = *(cl_uint *) &max_devices;
			for( cl_uint i=0; i < num_platforms; ++i )
			{
				const Platform P( platform_id[i] );
				(*(Core::Platforms *)&platform).append( P );
				sum += P->num_devices;
				
			}
			assert( platform.size == num_platforms );
			
			//------------------------------------------------------------------
			//-- allocate extra space
			//------------------------------------------------------------------
			YOCTO_EMBED_DECL(reg)
			YOCTO_EMBED(device_id_,max_devices),
			YOCTO_EMBED(build_log_,max_devices),
			YOCTO_EMBED_PACK(wksp_,reg,pooled,NULL);
			
			build_log.rebase(  build_log_,  max_devices );
			device_map.rebase( device_id_,  max_devices );
			
		}
		
		const Device & Driver:: operator[]( const cl_device_id id ) const
		{
			
			for( size_t i=0; i < num_platforms; ++i )
			{
				const Platform &P = platform[i];
				for( size_t j=0; j < P->num_devices; ++j )
				{
					const Device &D = P->device[j];
					if( id == D->id )
						return D;
				}
			}
			throw Exception( CL_INVALID_DEVICE, "OpenCL[cl_device_id]" );
		}
		
		void Driver:: BuildLogOut( std::ostream &os ) const
		{
			for( size_t i=0; i < build_log.size; ++i )
			{
				os << build_log[i] << std::endl;
			}
			
		}
		
		
	}
	
}

