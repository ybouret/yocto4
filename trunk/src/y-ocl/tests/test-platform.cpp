#include "yocto/utest/run.hpp"
#include "yocto/ocl/driver.hpp"

using namespace yocto;

#define SHOWP(FIELD) std::cerr << "\t" #FIELD << " = " << P.FIELD << std::endl
#define SHOWD(FIELD) std::cerr << "\t\t" #FIELD << " = " << D.FIELD << std::endl


YOCTO_UNIT_TEST_IMPL(platform)
{
	YOCTO_OPENCL;
	
	std::cerr << "OpenCL.num_platforms=" << OpenCL.num_platforms << std::endl;
	for( cl_uint p = 0; p < OpenCL.num_platforms; ++p )
	{
		const ocl::Platform &P = OpenCL.Platforms[p];
		SHOWP(PROFILE);
		SHOWP(VERSION);
		SHOWP(NAME);
		SHOWP(VENDOR);
		std::cerr << "\t#extensions=" << P.extensions.size << std::endl;
		for( size_t j=0; j < P.extensions.size; ++j )
		{
			std::cerr << "\t\t'" << P.extensions[j] << "'" << std::endl;
		}
		std::cerr << P.NAME << " #num_devices=" << P.num_devices << std::endl;
		for( size_t k=0; k < P.num_devices; ++k )
		{
			const ocl::Device &D = P.Devices[k];
			std::cerr << std::endl;
			SHOWD(NAME);
			SHOWD(COMPILER_AVAILABLE);
			SHOWD(ERROR_CORRECTION_SUPPORT);
			std::cerr << "\t\t#extensions=" << D.extensions.size << std::endl;
			for( size_t j=0; j < D.extensions.size; ++j )
			{
				std::cerr << "\t\t\t'" << D.extensions[j] << "'" << std::endl;
			}
			SHOWD(GLOBAL_MEM_CACHE_SIZE);
			SHOWD(GLOBAL_MEM_SIZE);
			SHOWD(IMAGE_SUPPORT);
			SHOWD(LOCAL_MEM_SIZE);
			SHOWD(MAX_CLOCK_FREQUENCY);
			SHOWD(MAX_COMPUTE_UNITS);
			SHOWD(MAX_CONSTANT_BUFFER_SIZE);
			SHOWD(MAX_MEM_ALLOC_SIZE);
			SHOWD(MAX_WORK_GROUP_SIZE);
			SHOWD(MAX_WORK_ITEM_DIMENSIONS);
			
			std::cerr << "\t\tMAX_WORK_ITEM_SIZES = [ ";
			for( size_t j=0; j < D.MAX_WORK_ITEM_DIMENSIONS; ++j )
			{
				std::cerr << D.MAX_WORK_ITEM_SIZES[j] << " ";
			}
			std::cerr << "]" << std::endl;
			SHOWD(PROFILE);
			SHOWD(PROFILING_TIMER_RESOLUTION);
			SHOWD(TYPE);
			SHOWD(VENDOR);
			SHOWD(VENDOR_ID);
			SHOWD(VERSION);
			SHOWD(DRIVER_VERSION);
			
		}
	}
}
YOCTO_UNIT_TEST_DONE()
