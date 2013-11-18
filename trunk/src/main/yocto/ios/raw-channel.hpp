#ifndef YOCTO_IOS_RAW_CHANNEL_INCLUDED
#define YOCTO_IOS_RAW_CHANNEL_INCLUDED 1

#include "yocto/ios/raw-file.hpp"

namespace yocto
{
    
    namespace ios
    {
        
        class raw_channel 
        {
        public:
            virtual ~raw_channel() throw();
            
            local_file       &handle() throw();
            const local_file &handle() const throw();
            
        protected:
            raw_file h;
            
            explicit raw_channel( size_t access, const string &filename, offset_t at, error_type *status);
            explicit raw_channel( size_t access, const char   *filename, offset_t at, error_type *status);
            explicit raw_channel( size_t access, raw_file::handle_t user_handle, error_type *status) throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(raw_channel);
            
        };
        
    }
}

#endif

