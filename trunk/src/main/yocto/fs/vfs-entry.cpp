#include "yocto/fs/vfs.hpp"
#include <cstring>

namespace yocto {
	
    bool vfs::entry:: is_regular()   const throw()
    {
        return is_reg == attr;
    }
    
    bool vfs::entry:: is_directory()   const throw()
    {
        return is_dir == attr;
    }
    
    bool vfs:: entry:: is_dot()  const throw()
    {
        assert(base_name);
        return is_dir == attr && 0 == strcmp(".",base_name);
    }
    
    bool vfs:: entry:: is_ddot()  const throw()
    {
        assert(base_name);
        return is_dir == attr && 0 == strcmp("..",base_name);
    }
    
    bool vfs::entry:: is_dot_or_ddot() const throw()
    {
        assert(base_name);
        return is_dir == attr && ( 0 == strcmp(".",base_name) || 0 == strcmp("..",base_name));
    }
    
    bool vfs::entry::  is_subdir() const throw()
    {
        return is_dir == attr && 0 != strcmp(".",base_name) && 0 != strcmp(".",base_name);
    }
    
    
    vfs::entry:: entry( const string &vfs_path, const vfs &vfs_from ) :
    path( vfs_path ),
    cstr( path.c_str()  ),
    base_name( vfs::get_base_name( path ) ),
    extension( vfs::get_extension( path )  ),
    link( false ),
    attr( vfs_from.query_attribute( path,  *(bool *)&link ) )
    {
    }
    
    vfs::entry:: ~entry() throw() {
    }
    
    
    vfs:: entry:: entry( const entry &other ) :
    path( other.path ),
    cstr( path.c_str() ),
    base_name( vfs::get_base_name( path ) ),
    extension( vfs::get_extension( path ) ),
    link( other.link ),
    attr( other.attr )
    {
        
    }
    
    bool vfs::entry:: has_extension( const string &ext ) const throw()
    {
        return (extension!=0) && (strcmp( ext.c_str(), extension ) == 0);
    }
    
    bool vfs::entry:: has_extension( const char *ext ) const throw()
    {
        if( 0 == ext )
        {
            return (0 == extension);
        }
        else
        {
            return (extension!=0) && (strcmp(ext,extension) == 0);
        }
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
