#include "yocto/filesys/vfs.hpp"
#include "yocto/string/vfs-utils.hpp"

namespace yocto {
	
	
	namespace filesys
	{
		
		
		vfs::entry:: entry( const string &vfs_path, const vfs &vfs_from ) :
        //fs( vfs_from ),
		path( vfs_path ),
		cstr( path.c_str()  ),
		base_name( _vfs::get_base_name( path ) ),
		extension( _vfs::get_extension( path )  ),
		link( false ),
		attr( vfs_from.query_attribute( path,  *(bool *)&link ) )
		{
		}
		
		vfs::entry:: ~entry() throw() {
		}
		
		
		vfs:: entry:: entry( const entry &other ) :
        //fs( other.fs ),
		path( other.path ),
		cstr( path.c_str() ),
		base_name( _vfs::get_base_name( path ) ),
		extension( _vfs::get_extension( path ) ),
		link( other.link ),
		attr( other.attr )
		{
			
		}
		
		
		void vfs:: foreach_in( const string &dirname,  vfs::entry::callback &on_entry ) const
		{
			scanner *scan = new_scanner( dirname );
			try
			{
				const entry *ep = scan->next();
				while( NULL != ep)
				{
					if( !on_entry( *ep ) ) return;
					ep = scan->next();
				}
				delete scan;
			}
			catch(...)
			{
				delete scan;
				throw;
			}
			
		}
		
	}
	
}
