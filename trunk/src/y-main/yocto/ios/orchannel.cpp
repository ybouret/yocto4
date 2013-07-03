#include "yocto/ios/orchannel.hpp"

namespace yocto
{
    namespace ios
    {
        
        orchannel:: ~orchannel() throw()
		{
			
		}
		
		orchannel:: orchannel( const string &filename, offset_t at, error_type *status ) :
        raw_channel( writable, filename, at, status )
		{
		}
        
        orchannel:: orchannel( const char *filename, offset_t at, error_type *status ) :
        raw_channel( writable, filename, at, status )
		{
        }
		
		void orchannel:: put( const void *data, size_t size, size_t &done )
		{
			h.put( data, size, done );
		}
		
        
        orchannel:: orchannel( raw_file::handle_t handle, error_type *status) :
        raw_channel( writable, handle, status )
        {
        }
        
        
    }
}
