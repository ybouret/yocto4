#include "yocto/ios/rc.hpp"
#include "yocto/exception.hpp"
#include "yocto/ios/icstream.hpp"

namespace yocto
{
    namespace ios 
    {
        
        
#define Y_RC_PACK_CTOR()          \
file_( filename, ios::writable ), \
count(0),                         \
start(0),                         \
H()
        
        
        resources:: packer:: packer( const string &filename, error_type &status ) :
        Y_RC_PACK_CTOR()
        {
            init(status);
        }
        
        resources:: packer:: packer( const char *filename, error_type &status ) :
        Y_RC_PACK_CTOR()
        {
            init(status);
        }
        
        void resources:: packer:: init( error_type &status )
        {
            file_.status = &status;
            count = 0;
            file_.unwind();
            start = file_.tell();
        }
        
        void resources:: packer:: operator()( const string &rcname, const string &filename )
        {
            //-- initialize hasher
            H.set();
            H(rcname);
            
            //-- open the resource
            ios::raw_file  source( filename, ios::readable );
            const uint64_t source_length = source.length();
            
            //-- save MAGIC
            file_.emit<uint32_t>( MAGIC );
            
            //-- save the rcname
            file_.save_buffer( rcname );
            
            //-- save the resource by raw copy
            file_.emit<uint64_t>( source_length );
            uint64_t written_length = 0;
            {
                char   buff[ BUFSIZ ];
                size_t done = 0;
                while(true)
                {
                    source.get(buff,sizeof(buff),done);
                    if( done <= 0 )
                        break;
                    H.run( buff, done );
                    written_length += done;
                }
            }
            if( source_length != written_length )
                throw exception("copy error for '%s'", rcname.c_str());
            
            //-- all done
            ++count;
            
        }
        
        resources:: packer:: ~packer() throw() 
        {
            try
            {
                // write epilog
                file_.emit< int64_t>(start);
                file_.emit<uint32_t>(count);
                file_.emit<uint32_t>(MAGIC);
            }
            catch(...)
            {
                //-- error in status !
            }
            
        }
        
        ////////////////////////////////////////////////////////////////////////
        resources:: ~resources() throw() {}
        
        resources:: resources( const string &filename ) :
        file_( filename, ios::readable )
        {
            extract();
        }
        
        resources:: resources( const char *filename ) :
        file_( filename, ios::readable )
        {
            extract();
        }
        
        
        void resources:: extract()
        {
            int64_t  start = 0;
            uint32_t count = 0;
            uint32_t magic = 0;
            const size_t epilog_size = sizeof(count) + sizeof(start) + sizeof(magic);
            
            file_.seek(epilog_size, from_end);
            start = file_.read<int64_t>();
            count = file_.read<uint32_t>();
            magic = file_.read<uint32_t>();
            if( MAGIC != magic )
                throw exception("resources: invalid MAGIC tag");
            
            //-- move at start
            file_.seek( start, from_set );
            
            
        }
        
        
    }
}
