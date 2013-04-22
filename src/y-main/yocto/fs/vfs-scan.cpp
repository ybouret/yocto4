#include "yocto/fs/vfs.hpp"
#include "yocto/string/vfs-utils.hpp"

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
        _vfs::as_directory(dir_);
        //std::cout << "scanner [" << dir_ << "]" << std::endl;
    }
    
    void vfs:: scanner:: free_entry() throw()
    {
        if( ent_ )
        {
            destruct( ent_ );
            memset( data_, 0, sizeof(data_) );
            ent_ = NULL;
        }
    }
    
    
    void vfs:: scanner:: make_entry(const char *entry_name)
    {
        assert( length_of(entry_name)>0 );
        free_entry();
        
        vfs::entry *tmp       = (vfs::entry *)(void *)data_;
        string      full_path = dir_ + entry_name;
        
        _vfs::as_directory( full_path ).trim( 1 );
        new (tmp) vfs::entry( full_path, vfs_ );
        ent_ = tmp;
    }
    
	
}
