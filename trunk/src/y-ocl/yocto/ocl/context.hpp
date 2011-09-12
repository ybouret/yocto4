#ifndef YOCTO_OPENCL_CONTEXT_INCLUDED
#define YOCTO_OPENCL_CONTEXT_INCLUDED 1

#include "yocto/ocl/driver.hpp"

namespace yocto
{

	namespace ocl
	{
		class Context;

		namespace Core
		{

			class Context : public Object
			{
			public:
				const DeviceIDs       device_id;
				const size_t        & NUM_DEVICES;
				const cl_device_id  * DEVICES;


				virtual ~Context() throw();

				typedef Proxy<Context> Pointer;

			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(Context);
				explicit Context( cl_context context, size_t num_devices, const cl_device_id *devices ); //!< generic call
				explicit Context( cl_context context );                                                  //!< clGetContextFromType

				friend class ocl::Context;
			};


		}

		class Context : public Class
		{
		public:
			explicit Context( size_t num_devices, const cl_device_id *devices);
			explicit Context( const Core::DeviceIDsMap &devmap );
			explicit Context( cl_device_type device_type );
			virtual ~Context() throw();

			Context( const Context & ) throw(); //!< shared copy


			const Core ::Context * operator->() const throw(); //!< high level
			const cl_context     & operator *() const throw(); //!< low level access: TODO: check who needs it ?


		private:
			Core::Shared<cl_context> hContext_;
			Core::Context::Pointer   pContext_;

			static cl_context __create_context( size_t num_devices, const cl_device_id *devices, void *self );
			static cl_context __create_context( cl_device_type device_type, void *self );

			static void 
			#if defined(YOCTO_WIN32)
			WINAPI
			#endif
			Notify(const char *errinfo,
										   const void *private_info,
										   size_t      cb,
										   void *      user_data);

			virtual void OnError( const char *errinfo, size_t errsize, const memory::ro_buffer &private_info ) throw();

		};


	}
}

#endif
