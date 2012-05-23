#ifndef YOCTO_IOS_RC_INCLUDED
#define YOCTO_IOS_RC_INCLUDED 1

#include "yocto/ios/ichannel.hpp"
#include "yocto/ios/raw-file.hpp"
#include "yocto/intrusive-ptr.hpp"
#include "yocto/hashing/sha1.hpp"

namespace yocto 
{
    
    namespace ios
    {
        
        class resources
        {
        public:
            typedef hashing::sha1 hasher;
            
            explicit resources( const string &filename );
            explicit resources( const char   *filename );
            virtual ~resources() throw();
            
            static const uint32_t MAGIC = 0x5171ca; // silica
            
            class packer
            {
            public:
                explicit packer(const string &filename, error_type &status);
                explicit packer(const char   *filename, error_type &status);

                void operator()( const string &rcname, const string &filename);
                
                virtual ~packer() throw();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(packer);
                raw_file file_;
                uint32_t count;
                int64_t  start;
                hasher   H;
                void init( error_type &status );
                
            };
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(resources);
            raw_file file_;
            void     extract();
        };
    }
    
}

#endif
