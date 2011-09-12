#include "yocto/ocl/program.hpp"
#include "yocto/ocl/_query.hpp"

#include <iostream>

namespace yocto
{
	
	namespace ocl
	{
		
		namespace Core 
		{
			Program:: ~Program() throw()
			{
				
			}
			
			Program:: Program( const ocl::Context & __context ) :
			context( __context ),
			CONTEXT( *context  ),
			device_id_( context->NUM_DEVICES ),
			device_map( device_id_(0), device_id_.size ),
			NUM_DEVICES( device_map.size ),
			DEVICES( device_map(0) )
			{
				assert( 0 == NUM_DEVICES );
			}
			
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
			
			static inline
			cl_program __create_program_with_sources( cl_context context, const ocl::Codes &sources )
			{
				GlobalMemoryLock access;
				
				const size_t ns  = sources.size();
				cl_int       err = CL_SUCCESS;
				ArrayOf<const char *>::Type strings( ns );
				ArrayOf<size_t>::Type       lengths( ns );
				
				for( size_t i=0; i < ns; ++i )
				{
					const Chunk &src = **sources(i);
					strings[i] = static_cast<const char *>( src.data );
					lengths[i] = src.size;
				}
				cl_program prg = clCreateProgramWithSource( context, ns, strings(0), lengths(0), &err);
				if( CL_SUCCESS != err )
					throw Exception( err, "clCreateProgramWithSource" );
				return prg;
			}
			
			static inline
			cl_program __create_program_with_binaries( const ocl::Context & context, const ocl::Codes &binaries )
			{
				//-- check input
				const size_t num_devices = context->NUM_DEVICES;
				if( binaries.size() != num_devices )
					throw exception( "OpenCL( #binaries=%u != #devices=%u )", unsigned(binaries.size()), unsigned(num_devices) );
				
				//-- prepare OpenCL input
				const cl_device_id *device_list = context->DEVICES;
				
				ArrayOf<size_t>::Type                ocl_len( num_devices );
				ArrayOf<const unsigned char *>::Type ocl_bin( num_devices );
				ArrayOf<cl_int>::Type                ocl_err( num_devices );
				
				const ocl::Code *bin = binaries(0);
				for( size_t i=0; i < num_devices; ++i )
				{
					ocl_len[ i ] = bin[i]->size;
					ocl_bin[ i ] = static_cast<unsigned char *>( bin[i]->data );
					ocl_err[ i ] = CL_SUCCESS;
				}
				
				cl_int     err = CL_SUCCESS;
				cl_program pgm = clCreateProgramWithBinary( *context, 
														   num_devices, 
														   device_list, 
														   ocl_len(0),
														   ocl_bin(0),
														   ocl_err(0),
														   &err);
				if( err != CL_SUCCESS )
				{
					throw Exception( err, "clCreateProgramWithBinary" );
				}
				return pgm;
			}
			
		}
		
		
		Program:: Program(const Context &              __context,
						  const Sources &               sources ) :
		hProgram_( Core::__create_program_with_sources( * __context, sources ) ) ,
		pProgram_( new Core::Program( __context ) )
		{
		}
		
		
		
		Program:: Program(const Context          &         context,
						  const Binaries         &         binaries ) :
		hProgram_( Core::__create_program_with_binaries( context, binaries ) ),
		pProgram_( new Core::Program( context ) )
		{
		}
		
		
		Program:: Program( const Program &other ) throw() :
		hProgram_( other.hProgram_ ),
		pProgram_( other.pProgram_ )
		{
		}
		
		Program:: ~Program() throw() {}
		
		const Core::Program * Program:: operator->() const throw() { return & ( *pProgram_ ); }
		
		
		static
		void _check_compile_parameters(const char *        __fn,
									   Program       &     program,
									   cl_uint             num_devices,
									   const cl_device_id *device_list )
		{
			
			if( num_devices <= 0 || NULL == device_list )
			{
				throw exception("%s( Invalid num_devices=%u and device_list@%p )", __fn, unsigned( num_devices ), (void*) device_list );
			}
			else 
			{
				const Context &context = program->context;
				if( num_devices > context->NUM_DEVICES )
				{
					throw exception("%s( Too Many Devices for context (%u>%u ) )", __fn, unsigned( num_devices ), unsigned( context->NUM_DEVICES ) );
				}
				else
				{
					
					//-- Perform Check for devices
					for( size_t i=0; i < num_devices; ++i )
					{
						const cl_device_id dev_i = device_list[i];
						bool               found = false;
						
						//-- Loop 1: check belonging here
						for( size_t j=0; j < context->NUM_DEVICES; ++j )
						{
							if( dev_i == context->DEVICES[j] )
							{
								found = true;
								break;
							}
						}
						if( !found )
						{
							throw exception("%s( Invalid Device #%u )", __fn, unsigned( i ) );
						}
						
						
						//-- Loop 2: check no multiple
						for( size_t j=i+1; j < num_devices; ++j )
						{
							if( dev_i == device_list[j] )
								throw exception("%s( Multiple Device #%u )",  __fn, unsigned( j ) );
						}
						
						
					}
					
				}
				
			}
			
		}
		

		void Driver:: Compile(Program &           program,
							  cl_uint             num_devices,
							  const cl_device_id *device_list,
							  const char *        options )
		{
			static const char __fn[] = "Driver::Compile";
			Core::GlobalMemoryLock  access;
			lock                    self;
			
			
			std::cerr << "--> OpenCL.Compiling.Init" << std::endl;
			
			//------------------------------------------------------------------
			//
			//-- initialize build_logs and program->device_map
			//
			//------------------------------------------------------------------
			build_log.free();
			program->device_map.free();
			
			
			//------------------------------------------------------------------
			//
			//-- sanity check
			//
			//------------------------------------------------------------------
			std::cerr << "--> OpenCL.Compiling.Check" << std::endl;
			if( NULL == device_list && 0 == num_devices )
			{
				num_devices = program->context->NUM_DEVICES;
				device_list = program->context->DEVICES;
			}
			else 
				_check_compile_parameters(__fn, program, num_devices, device_list );
			
			std::cerr << "--> OpenCL.Compiling.Record  #device=" << num_devices << "  @" << (void *)device_list << std::endl;
			//------------------------------------------------------------------
			//
			//-- record devices for the program
			//
			//------------------------------------------------------------------
			try
			{
				assert( 0 == build_log.size );
				assert( 0 == program->device_map.size );
				const string nope;
				for( size_t j=0; j < num_devices; ++j )
				{
					build_log.append( nope );
					program->device_map.append( device_list[j] );
					std::cerr << "     (*) " << (*this)[ device_list[j] ]->NAME << std::endl;
				}
			}
			catch (...) {
				program->device_map.free();
				build_log.free();
				throw;
			}
			
			
			std::cerr << "--> OpenCL.Compiling.BuildProgram" << std::endl;			
			//------------------------------------------------------------------
			//
			//-- use OpenCL
			//
			//------------------------------------------------------------------
			cl_program      prg = * program.hProgram_;
			const cl_int    err = clBuildProgram( prg, num_devices, device_list, options, NULL, NULL);
			
			
			//------------------------------------------------------------------
			//
			//-- collect logs
			//
			//------------------------------------------------------------------
			for( size_t i=0; i < num_devices; ++i )
			{
				string             & _log = build_log[i];
				const cl_device_id & _dev = device_list[i];
				const Device       & _Dev = (*this)[ _dev ];
				
				//-- prolog
				_log.append( "|" );
				_log.append( _Dev->NAME );
				_log.append( ">\n" );
				
				
				//-- log
				size_t        len = 0;
				cl_int       _err = clGetProgramBuildInfo(prg, _dev, CL_PROGRAM_BUILD_LOG, 0, NULL, &len );
				if( CL_SUCCESS != _err )
				{
					_log.append( "couldn't read build_log size\n" );
				}
				else {
					Core::ArrayOf<char>::Type msg(len);
					_err = clGetProgramBuildInfo(prg, _dev, CL_PROGRAM_BUILD_LOG, msg.size, msg(0), NULL );
					if( CL_SUCCESS != _err )
					{
						_log.append( "couldn't read build_log char[]\n" );
					}
					else 
					{
						_log.append( msg(0) );
					}
					
				}
				
				//-- epilog
				if( 0 == _log.size() || _log.back() != '\n' )
				{
					_log.append( '\n' );
				}
				_log.append( "<" );
				_log.append( _Dev->NAME );
				_log.append( "|" );
			}
			
			//------------------------------------------------------------------
			//-- throw on error
			//------------------------------------------------------------------
			if( err )
			{
				throw Exception( err, "clBuildProgram" );
			}
			
			
		}
		
		void Driver:: Compile( Program & __program, const char *options )
		{
			Compile( __program, 0, NULL, options );
		}
		
		
		
		
		void Program:: LoadBinaries( Binaries &binaries ) const
		{
			const Core::Program  & program = *pProgram_;
			
			binaries.free();
			
			cl_program   prg  = * hProgram_;
			const size_t pnd  = program.NUM_DEVICES;
			
			//==================================================================
			//-- query the sizes and create empty binaries
			//==================================================================
			{
				Core::Sizes         bsz( pnd );
				const cl_int        err = clGetProgramInfo(prg, 
														   CL_PROGRAM_BINARY_SIZES, 
														   bsz.bytes,
														   bsz(0),
														   NULL);
				
				
				if( CL_SUCCESS != err )
				{
					throw Exception( err, "clGetProgramInfo(CL_PROGRAM_BINARY_SIZES)" );
				}
				
				for( size_t j=0; j < pnd; ++j )
				{
					const Code bin = new Core::Chunk( bsz[j] );
					binaries.push_back( bin );
				}
			}
			
			//==================================================================
			//-- load the binaries
			//==================================================================
			{
				Core::ArrayOf<unsigned char *>::Type bins( pnd );
				for( size_t j=0; j < pnd; ++j )
				{
					bins[j] = static_cast<unsigned char *>( binaries[j+1]->data );
				}
				const cl_int  err = clGetProgramInfo(prg, CL_PROGRAM_BINARIES, bins.bytes, bins(0), NULL);
				
				if( CL_SUCCESS != err )
				{
					throw Exception( err, "clGetProgramInfo(CL_PROGRAM_BINARIES)" );
				}
				
				
			}
			
		}
		
		
	}
	
}
