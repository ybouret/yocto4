#ifndef YOCTO_IOS_RC_INCLUDED
#define YOCTO_IOS_RC_INCLUDED 1

#include "yocto/ios/iflux.hpp"
#include "yocto/ios/raw-file.hpp"
#include "yocto/hashing/sha1.hpp"
#include "yocto/code/fourcc.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/ordered/catalog.hpp"

#include "yocto/memory/buffers.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/ptr/shared.hpp"
#include "yocto/container/sequence.hpp"

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
            typedef set<string,item> item_db;
            
            
            ios::ichannel *load_channel( const string &rcname ) const;
            ios::ichannel *load_channel( const char   *rcname ) const;
            ios::istream  *load_stream(  const string  &rcname ) const;
            ios::istream  *load_stream(  const char    *rcname ) const;
            void           load_chars( sequence<char> &seq, const string &rcname, size_t num_pad ) const;
            void           load_chars( sequence<char> &seq, const char   *rcname, size_t num_pad ) const;
            void           load_string( string &str, const string &rcname ) const;
            void           load_string( string &str, const char   *rcname ) const;

            typedef item_db::const_iterator iterator;
            iterator begin() const;
            iterator end()   const;
            size_t   size() const throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(resources);
            
            shared_ptr<raw_file> rc;
            hasher               H;
            item_db              db;
            void     extract();
            
            //! specialized input channel 
            class rc_channel : public ios::ichannel
            {
            public:
                explicit rc_channel( const shared_ptr<raw_file> &fp, const int64_t pos, const uint64_t len ) throw();
                virtual ~rc_channel() throw();
                virtual void get( void *data, size_t size, size_t &done );
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(rc_channel);
                shared_ptr<raw_file> rc;
                const int64_t        at;
                int64_t              curr;
                const uint64_t       size;
                const int64_t        last;
            };
            
            typedef memory::buffer_of<char,memory::global> rc_buffer;
            class rc_stream : public rc_channel, public rc_buffer, public iflux
            {
            public:
                explicit rc_stream( const shared_ptr<raw_file> &fp, const int64_t pos, const uint64_t len );
                virtual ~rc_stream() throw();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(rc_stream);
            };
            
        };
    }
    
}

#endif
