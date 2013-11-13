#include "yocto/fs/vfs.hpp"

#include <cstring>
#include <new>

namespace yocto
{
	
    
    vfs:: scanner:: ~scanner() throw()
    {
        free_entry();
    }
    
    
    vfs:: scanner:: scanner( const string &dirname, const vfs &owner ) :
    vfs_( owner   ),
    dir_( dirname ),
    ent_( NULL )
    {
        vfs::as_directory(dir_);
    }
    
    void vfs:: scanner:: free_entry() throw()
    {
        if( 0 != ent_ )
        {
            destruct<vfs::entry>( ent_ );
            memset( data_, 0, sizeof(data_) );
            ent_ = 0;
        }
    }
    
    
    void vfs:: scanner:: make_entry(const char *entry_name)
    {
        assert( length_of(entry_name)>0 );
        free_entry();
        
        vfs::entry *tmp       = (vfs::entry *)(void *)data_;
        string      full_path = dir_ + entry_name;
        
        vfs::as_directory( full_path ).trim( 1 );
        new (tmp) vfs::entry( full_path, vfs_ );
        ent_ = tmp;
    }
    
	
}
