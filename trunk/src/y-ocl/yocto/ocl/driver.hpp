#ifndef YOCTO_OCL_DRIVER_INCLUDED
#define YOCTO_OCL_DRIVER_INCLUDED 1


#include "yocto/ocl/platform.hpp"
#include "yocto/threading/singleton.hpp"

#include <iosfwd>

namespace yocto
{
	namespace ios { class ostream; }
	
	namespace  ocl 
	{
		class Program;
		
		class Driver : public singleton<Driver>
		{
		public:
			const cl_uint                               num_platforms;
			const ArrayOf<cl_platform_id>::Type         platforms;
		private: ArrayOf<Platform>::Type                Platforms_;
		public: const memory::records_of<Platform>      Platforms;       
			const cl_uint                               num_devices;     //!< total of devices
			
			const Device & operator[]( const cl_device_id ) const;
			
			void Build(Program            &program,
					   const DeviceIDs    &devices,
					   const char *        options );
			            
		private:
			explicit Driver();
			virtual ~Driver() throw();
			
			static const threading::longevity life_time = 7;
			static const char                 name[];
			
			YOCTO_DISABLE_COPY_AND_ASSIGN(Driver);
			friend class singleton<Driver>;
			DeviceMap     devmap_;
			cl_device_id *devices_;
			string       *builds_;
			void         *wksp;
			size_t        wlen;
			
		public:
			memory::records_of<string> BuildLogs;
			void BuildLogsOut( std::ostream & ) const;
			void BuildLogsOut( ios::ostream & ) const;
			
		};
		
#define YOCTO_OpenCL ocl::Driver & OpenCL = ocl::Driver::instance()
		
	}
}


#endif
