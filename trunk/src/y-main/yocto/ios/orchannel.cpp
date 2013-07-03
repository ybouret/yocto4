#include "yocto/ios/orchannel.hpp"

namespace yocto
{
    namespace ios
    {
        
        orchannel:: ~orchannel() throw()
		{
			
		}
		
		orchannel:: orchannel( const string &filename, offset_t at, error_type *status ) :
		file_( filename, readable )
		{
			file_.status = status;
			file_.seek( at, from_set );
		}
        
        orchannel:: orchannel( const char *filename, offset_t at, error_type *status ) :
		file_( filename, readable )
		{
			file_.status = status;
			file_.seek( at, from_set );
		}
		
		void orchannel:: put( const void *data, size_t size, size_t &done )
		{
			file_.put( data, size, done );
		}
		
        
        orchannel:: orchannel( raw_file::handle_t handle, error_type *status) :
        file_( handle, writable )
        {
            file_.status = status;
        }

        
    }
}
