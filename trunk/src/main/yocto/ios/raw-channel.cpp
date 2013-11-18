#include "yocto/ios/raw-channel.hpp"

namespace yocto
{
    
    namespace ios
    {
        
        raw_channel:: ~raw_channel() throw() {}
        
        local_file & raw_channel:: handle() throw()
        {
            return h;
        }
     
        const local_file & raw_channel:: handle() const throw()
        {
            return h;
        }
        
        raw_channel:: raw_channel( size_t access, const string &filename, offset_t at, error_type *status) :
        h( filename, access )
        {
            h.status = status;
            h.seek(at, from_set);
        }

        raw_channel:: raw_channel( size_t access, const char *filename, offset_t at, error_type *status) :
        h( filename, access )
        {
            h.status = status;
            h.seek(at, from_set);
        }

        raw_channel:: raw_channel( size_t access, raw_file::handle_t user_handle, error_type *status) throw() :
        h( user_handle, access )
        {
            h.status = status;
    
        }

        
        
    }

}
