#include "yocto/ocl/program.hpp"
#include "yocto/ocl/driver.hpp"
#include "yocto/ocl/_query.hpp"

namespace yocto
{
	
	namespace ocl 
	{
		
		Program:: ~Program() throw() {}
		
		static inline
		size_t _ref_count( cl_program program )
		{
			cl_uint      ans = 0;
			const cl_int err = clGetProgramInfo( program, CL_PROGRAM_REFERENCE_COUNT, sizeof(ans), &ans, NULL);
			if( err != CL_SUCCESS )
			{
				throw Exception( err, "clGetProgramInfo( REF_COUNT )" );
			}
			return ans;
		}
		
		YOCTO_OPENCL_SHARED_IMPL(cl_program,clRetainProgram,clReleaseProgram, _ref_count);
		
		//======================================================================
		static inline
		cl_program __create_program_with_sources( const Context &ctx, const Sources &src )
		{
			const size_t count = src.size();
			
			ArrayOf<char *>::Type sources( count );
			ArrayOf<size_t>::Type lengths( count );
			
			for( size_t i=0,j=1; i < count; ++i, ++j )
			{
				const Code &code = src[j];
				sources[i] = code->data;
				lengths[i] = code->size;
			}
			
			cl_int     err     = CL_SUCCESS;
			cl_program program = clCreateProgramWithSource( *ctx, count, (const char **)sources(), lengths(), &err); 
			if( CL_SUCCESS != err )
				throw Exception( err, "clCreateProgramWithSource" );
			return program;
		}
		
		Program:: Program( const Context &ctx, const Sources &src ) :
		Shared<cl_program>( __create_program_with_sources(ctx,src) ),
		deviceIDs( ctx.NUM_DEVICES ),
		devices( deviceIDs(), deviceIDs.size ),
		NUM_DEVICES( devices.size  ),
		DEVICES(     devices()     )
		{
			
		}
		
		
		//======================================================================
		//
		// Compilation
		//
		//======================================================================
		
		static inline void __build_dev( void *, void * ) throw() {}
		
		static inline void __build_log( void *ptr, void *args )
		{
			assert( ptr  != NULL );
			assert( args != NULL );
			const string &str = *(string *)args;
			new (ptr) string( str );
		}
		
		
		void Driver:: Build(Program            &program,
							const DeviceIDs    &devices,
							const char *        options )
		{
			YOCTO_LOCK(access);
			BuildLogs.free();
			program.devices.free();
			
			//==================================================================
			//== manual check
			//==================================================================
			if( devices.size > program.deviceIDs.size )
				throw Exception( CL_INVALID_VALUE, "Driver::Build(Too Many Devices)");
			
			//==================================================================
			//== Build It !
			//==================================================================
			const cl_int err = clBuildProgram( *program, devices.size, devices(), options, NULL, NULL);
			
			//==================================================================
			//== Collect the BuildLogs
			//==================================================================
			try 
			{
				for( size_t i=0; i < num_devices; ++i )
				{
					//----------------------------------------------------------
					//-- extract the build log
					//----------------------------------------------------------
					const cl_device_id dev = devices[i];
					size_t             len  = 0;
					cl_int             err2 = clGetProgramBuildInfo( *program, dev, CL_PROGRAM_BUILD_LOG, 0, NULL, &len);
					string             info = "//" + (*this)[dev].NAME + '\n';
					if( CL_SUCCESS != err2 )
					{
						info.append( StrErr( err2 ) );
						BuildLogs( __build_log, &info );
					}
					else
					{
						ArrayOf<char>::Type buff( len );
						err2 = clGetProgramBuildInfo( *program, dev, CL_PROGRAM_BUILD_LOG, len, buff(), NULL);
						if( CL_SUCCESS != err2 )
						{
							info.append( StrErr( err2 ) );
							BuildLogs( __build_log, &info );
						}
						else 
						{
							info.append( buff(), buff.size );
							BuildLogs( __build_log, &info );
						}
					}
					
					//----------------------------------------------------------
					//-- make room for the device
					//----------------------------------------------------------
					program.devices( __build_dev, NULL );
					program.devices[ i ] = dev;
					
					
					
				}
				
				{
					//----------------------------------------------------------
					// check devices
					//----------------------------------------------------------
					size_t len  = 0;
					cl_int err3 = clGetProgramInfo( *program, CL_PROGRAM_DEVICES, 0, NULL, &len);
					
					if( CL_SUCCESS != err3 )
						throw Exception( err3, "CL_PROGRAM_DEVICES length" );
					
					if( len != program.devices.size * sizeof(cl_device_id) )
						throw Exception( CL_INVALID_VALUE, "CL_PROGRAM_DEVICES mismatch" );
					
					err3 = clGetProgramInfo( *program, CL_PROGRAM_DEVICES, len, (void*)devices(), NULL);
					if( CL_SUCCESS != err3 )
						throw Exception( err3, "CL_PROGRAM_DEVICES value" );
					
				}
				
			}
			catch (...) {
				BuildLogs.free();
				program.devices.free();
				throw;
			}
			
			//==================================================================
			//== Finalize
			//==================================================================
			if( err != CL_SUCCESS )
				throw Exception( err, "clBuildProgram" );
			
		}
		
		
	}
	
}

