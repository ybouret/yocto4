#ifndef YOCTO_IOS_RC_INCLUDED
#define YOCTO_IOS_RC_INCLUDED 1

#include "yocto/ios/ichannel.hpp"
#include "yocto/ios/raw-file.hpp"
#include "yocto/intrusive-ptr.hpp"
#include "yocto/hashing/sha1.hpp"
#include "yocto/code/fourcc.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/ordered/catalog.hpp"

namespace yocto 
{
    
    namespace ios
    {
     
        
        //! handling resources
        class resources
        {
        public:
            typedef hashing::sha1 hasher;
            
            explicit resources( const string &filename ); //! open file and load resources
            explicit resources( const char   *filename ); //! open file and load resources
            virtual ~resources() throw();
            
            static const uint32_t MAGIC = YOCTO_FOURCC('D','A','T','A');
            
            //! packer class
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
                catalog<string> db;
            };
            
                        
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(resources);
            
            //! resource item, name+location
            class item 
            {
            public:
                const string   label;
                const int64_t  start;
                const uint64_t bytes;
                
                explicit item( const string &, int64_t at, uint64_t nb );
                item( const item & );
                const string & key() const throw();
                ~item() throw();
                
            private:
                YOCTO_DISABLE_ASSIGN(item);
            };
            
            
            
            raw_file         file_;
            hasher           H;
            set<string,item> db;
            void     extract();
        };
    }
    
}

#endif
