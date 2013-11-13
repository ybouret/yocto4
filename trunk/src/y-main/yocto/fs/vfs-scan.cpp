#include "yocto/fs/vfs.hpp"

#include <cstring>
#include <new>

namespace yocto
{
	
    
    vfs:: scanner:: ~scanner() throw()
    {
    }
    
    
    vfs:: scanner:: scanner( const string &dirname, const vfs &owner ) :
    fs( owner   ),
    folder( dirname ),
    data()
    {
        vfs::as_directory((string&)folder);
    }
    
    
    
    const vfs::entry * vfs:: scanner:: make_entry(const char *entry_name)
    {
        assert( length_of(entry_name)>0 );
        
        string      full_path = folder + entry_name;
        vfs::as_directory( full_path ).trim( 1 );
        data.build<vfs::entry,string,vfs>(full_path,fs);
        
        return & data.as<vfs::entry>();
    }
   
	
}
