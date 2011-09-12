#ifndef YOCTO_OPENCL_PROGRAM_INCLUDED
#define YOCTO_OPENCL_PROGRAM_INCLUDED 1

#include "yocto/ocl/context.hpp"
#include "yocto/ocl/codes.hpp"

namespace yocto 
{
	
	namespace ocl
	{
		
		class Program;
		class Kernel;
		
		namespace Core
		{
			
			
			////////////////////////////////////////////////////////////////////////
			//
			// Program data 
			//
			////////////////////////////////////////////////////////////////////////
			class Program : public Object
			{
			public:
				
				explicit Program( const ocl::Context & __context );
				virtual ~Program() throw();
				
				const ocl::Context      context;     //!< for compilation checks
				const cl_context        CONTEXT;     //!< CL_PROGRAM_CONTEXT
				
			private:
				DeviceIDs               device_id_; //!< context->NUM_DEVICES at max
				mutable DeviceIDsMap    device_map; //!< not zero after a build
				
			public:
				const size_t       &NUM_DEVICES; //!< CL_PROGRAM_NUM_DEVICES
				const cl_device_id *DEVICES;     //!< CL_PROGRAM_DEVICES
				
				typedef Proxy<Program> Pointer;
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(Program);
				friend class ocl::Driver;
				//friend class OpenCL::Program;
			};
			
		}
		
		////////////////////////////////////////////////////////////////////////
		//
		// OpenCL Program
		//
		////////////////////////////////////////////////////////////////////////
		class Program : public Class
		{
		public:
			
			virtual ~Program() throw();
			explicit Program(const Context           &context, 
							 const Sources           &sources );
			
			//! from binaries, that must match the used context
			explicit Program(const Context          &context,
							 const Binaries         &binaries );
			
			Program( const Program & ) throw(); //!< Shared Copy
			
			const Core::Program * operator->() const throw();			
			
			void LoadBinaries( Binaries &binaries ) const;
			
		private:
			Core::Shared<cl_program>     hProgram_;
			Core::Program::Pointer       pProgram_;
			friend class ocl::Driver;
			friend class ocl::Kernel;
			YOCTO_DISABLE_ASSIGN(Program);
		};
		
	}
	
}

#endif
