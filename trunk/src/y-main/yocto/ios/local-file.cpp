#include "yocto/ios/local-file.hpp"
//#include "yocto/threading/singleton.hpp"

namespace yocto
{
	namespace ios
	{
        
		const local_file::cstdin_t  cstdin  = {};
		const local_file::cstdout_t cstdout = {};
		const local_file::cstderr_t cstderr = {};
        
        
		threading::mutex local_file::stdio_lock("STDIO");
        
		local_file:: ~local_file() throw()
		{
            switch(type)
            {
                case is_regular:
                case is_pipe:
                {
                    YOCTO_GIANT_LOCK();
                    delete access_;
				}
                    break;
                    
                
                default:
                    break;
            }
			
		}
        
		local_file:: local_file( type_t t ) :
		type( t ),
        access_(NULL)
		{
			switch( type )
			{
                case is_regular: {
                    YOCTO_GIANT_LOCK();
                    access_ = new threading::mutex("REG");
                }
                    break;
                    
                case is_pipe: {
                    YOCTO_GIANT_LOCK();
                    access_ = new threading::mutex("PIPE");
                }
                    break;
                    
                case is_stdin:
                    access_ = & stdio_lock;
                    break;
                    
                    
                case is_stderr:
                case is_stdout:
                    access_ = & stdio_lock;
                    break;
			}
		}
        
	}
}

