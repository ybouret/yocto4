#include "yocto/ocl/types.hpp"

namespace yocto
{
	
	namespace ocl
	{
		namespace Core 
		{
#define YOCTO_CL_STRERR(X) X
			const char * StrErr(const cl_int err)throw()
			{
				switch (err) {
					case CL_SUCCESS:                          return YOCTO_CL_STRERR("Success!");
					case CL_DEVICE_NOT_FOUND:                 return YOCTO_CL_STRERR("Device not found.");
					case CL_DEVICE_NOT_AVAILABLE:             return YOCTO_CL_STRERR("Device not available");
					case CL_COMPILER_NOT_AVAILABLE:           return YOCTO_CL_STRERR("Compiler not available");
					case CL_MEM_OBJECT_ALLOCATION_FAILURE:    return YOCTO_CL_STRERR("Memory object allocation failure");
					case CL_OUT_OF_RESOURCES:                 return YOCTO_CL_STRERR("Out of resources");
					case CL_OUT_OF_HOST_MEMORY:               return YOCTO_CL_STRERR("Out of host memory");
					case CL_PROFILING_INFO_NOT_AVAILABLE:     return YOCTO_CL_STRERR("Profiling information not available");
					case CL_MEM_COPY_OVERLAP:                 return YOCTO_CL_STRERR("Memory copy overlap");
					case CL_IMAGE_FORMAT_MISMATCH:            return YOCTO_CL_STRERR("Image format mismatch");
					case CL_IMAGE_FORMAT_NOT_SUPPORTED:       return YOCTO_CL_STRERR("Image format not supported");
					case CL_BUILD_PROGRAM_FAILURE:            return YOCTO_CL_STRERR("Program build failure");
					case CL_MAP_FAILURE:                      return YOCTO_CL_STRERR("Map failure");
					case CL_INVALID_VALUE:                    return YOCTO_CL_STRERR("Invalid value");
					case CL_INVALID_DEVICE_TYPE:              return YOCTO_CL_STRERR("Invalid device type");
					case CL_INVALID_PLATFORM:                 return YOCTO_CL_STRERR("Invalid platform");
					case CL_INVALID_DEVICE:                   return YOCTO_CL_STRERR("Invalid device");
					case CL_INVALID_CONTEXT:                  return YOCTO_CL_STRERR("Invalid context");
					case CL_INVALID_QUEUE_PROPERTIES:         return YOCTO_CL_STRERR("Invalid queue properties");
					case CL_INVALID_COMMAND_QUEUE:            return YOCTO_CL_STRERR("Invalid command queue");
					case CL_INVALID_HOST_PTR:                 return YOCTO_CL_STRERR("Invalid host pointer");
					case CL_INVALID_MEM_OBJECT:               return YOCTO_CL_STRERR("Invalid memory object");
					case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:  return YOCTO_CL_STRERR("Invalid image format descriptor");
					case CL_INVALID_IMAGE_SIZE:               return YOCTO_CL_STRERR("Invalid image size");
					case CL_INVALID_SAMPLER:                  return YOCTO_CL_STRERR("Invalid sampler");
					case CL_INVALID_BINARY:                   return YOCTO_CL_STRERR("Invalid binary");
					case CL_INVALID_BUILD_OPTIONS:            return YOCTO_CL_STRERR("Invalid build options");
					case CL_INVALID_PROGRAM:                  return YOCTO_CL_STRERR("Invalid program");
					case CL_INVALID_PROGRAM_EXECUTABLE:       return YOCTO_CL_STRERR("Invalid program executable");
					case CL_INVALID_KERNEL_NAME:              return YOCTO_CL_STRERR("Invalid kernel name");
					case CL_INVALID_KERNEL_DEFINITION:        return YOCTO_CL_STRERR("Invalid kernel definition");
					case CL_INVALID_KERNEL:                   return YOCTO_CL_STRERR("Invalid kernel");
					case CL_INVALID_ARG_INDEX:                return YOCTO_CL_STRERR("Invalid argument index");
					case CL_INVALID_ARG_VALUE:                return YOCTO_CL_STRERR("Invalid argument value");
					case CL_INVALID_ARG_SIZE:                 return YOCTO_CL_STRERR("Invalid argument size");
					case CL_INVALID_KERNEL_ARGS:              return YOCTO_CL_STRERR("Invalid kernel arguments");
					case CL_INVALID_WORK_DIMENSION:           return YOCTO_CL_STRERR("Invalid work dimension");
					case CL_INVALID_WORK_GROUP_SIZE:          return YOCTO_CL_STRERR("Invalid work group size");
					case CL_INVALID_WORK_ITEM_SIZE:           return YOCTO_CL_STRERR("Invalid work item size");
					case CL_INVALID_GLOBAL_OFFSET:            return YOCTO_CL_STRERR("Invalid global offset");
					case CL_INVALID_EVENT_WAIT_LIST:          return YOCTO_CL_STRERR("Invalid event wait list");
					case CL_INVALID_EVENT:                    return YOCTO_CL_STRERR("Invalid event");
					case CL_INVALID_OPERATION:                return YOCTO_CL_STRERR("Invalid operation");
					case CL_INVALID_GL_OBJECT:                return YOCTO_CL_STRERR("Invalid OpenGL object");
					case CL_INVALID_BUFFER_SIZE:              return YOCTO_CL_STRERR("Invalid buffer size");
					case CL_INVALID_MIP_LEVEL:                return YOCTO_CL_STRERR("Invalid mip-map level");
					default:                                  return YOCTO_CL_STRERR("Unknown");
				}
			}
		}
	}
	
}

