#include "yocto/utest/run.hpp"

#include "yocto/swamp/opencl.hpp"

using namespace yocto;
using namespace swamp;

typedef array2D<float> Array2D;
YOCTO_UNIT_TEST_IMPL(buffer)
{
    
    //! create host data
    layout2D               L( coord2D(1,1), coord2D(10,20) );
    fields_setup<layout2D> F;
    Y_SWAMP_DECL_VAR(F, "A", Array2D );
    Y_SWAMP_DECL_VAR(F, "B", Array2D );
    ghosts_setup<coord2D>  G;
    dataspace<layout2D>    D(L,G,F);
    
    //! create context
    YOCTO_OpenCL;
    const ocl::Platform &platform = OpenCL.Platforms[0];
    for( cl_uint j = 0; j < platform.num_devices; ++j )
	{
		const ocl::Device &D = platform.Devices[j];
        std::cerr << "\t * " << D.NAME << std::endl;
    }
    ocl::Context context( platform.devices );
    
    OCL_Buffer oclA( D.query( "A" ), context );
    OCL_Buffer oclB( D.query( "B" ), context );
    
    
    for( size_t j=0; j < context.NUM_DEVICES; ++j )
    {
        const cl_device_id dev = context.devices[j];
        ocl::CommandQueue Q( context, dev, 0 );
        
        oclA.Write(Q, CL_FALSE, YOCTO_OPENCL_NO_EVENT );
        oclA.Read( Q, CL_FALSE, YOCTO_OPENCL_NO_EVENT );
        
        Q.Flush();
    }
    
}
YOCTO_UNIT_TEST_DONE()
