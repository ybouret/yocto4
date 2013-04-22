#include "yocto/fs/local-fs.hpp"
#include "yocto/exceptions.hpp"

#if defined(YOCTO_BSD)
#include <sys/stat.h>
#include <unistd.h>
#include <cerrno>
#endif

#if defined(YOCTO_WIN)
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>

#if defined(__DMC__)
#define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
#endif

#endif

namespace yocto
{
    
	
    const char local_fs::name[] = "local-fs";
    
    local_fs:: local_fs() throw()
    {
    }
    
    local_fs:: ~local_fs() throw()
    {
        
    }
    
    vfs::entry::attribute local_fs:: query_attribute( const string &path, bool &is_link ) const throw()
    {
        is_link = false;
        
#if defined(YOCTO_WIN)
        const DWORD m = ::GetFileAttributes( TEXT(&path[0]) );
        
        if( m == INVALID_FILE_ATTRIBUTES )
            return vfs::entry::no_ent;
        if( m &  FILE_ATTRIBUTE_DIRECTORY )
            return vfs::entry::is_dir;
        if( m & (FILE_ATTRIBUTE_NORMAL|FILE_ATTRIBUTE_ARCHIVE) )
            return vfs::entry::is_reg;
        
        return vfs::entry::is_unk;
#endif
        
        
#if defined(YOCTO_BSD)
        struct stat buf;
        const mode_t *m = &(buf.st_mode);
        
        
        
        if( lstat( &path[0], &buf ) != 0 )
            return  vfs::entry::no_ent;
        
        
        
        if( S_ISLNK( (*m) ) ) {
            /* follow link */
            is_link = true;
            
            if( stat( &path[0], &buf ) !=0 )
                return  vfs::entry::no_ent; /* TODO: loop ? */
        }
        
        
        if( S_ISDIR( (*m) ) )
            return  vfs::entry::is_dir;
        
        if( S_ISREG( (*m) ) )
            return  vfs::entry::is_reg;
        
        
        return vfs::entry::is_unk;
#endif /* BSD */
    }
    
    void local_fs:: create_dir(  const string &dirname, const bool allow_already_exists )
    {
#if defined(YOCTO_BSD)
        YOCTO_GIANT_LOCK();
        if( mkdir( &dirname[0],  (S_IRUSR | S_IWUSR | S_IXUSR) | (S_IRGRP | S_IXGRP ) ) < 0 ) {
            
            const int err = errno;
            if( err == EEXIST &&  allow_already_exists ) {
                return;
            }
            else {
                throw libc::exception( err, "create_dir(%s)", &dirname[0]);
            }
            
        }
#endif
        
#if defined(YOCTO_WIN)
        YOCTO_GIANT_LOCK();
        const DWORD attr = ::GetFileAttributes( &dirname[0] );
        
        if( (attr != INVALID_FILE_ATTRIBUTES) && ( (attr & FILE_ATTRIBUTE_DIRECTORY) != 0 ) ) {
            if( !allow_already_exists  )
            {
                throw win32::exception( ERROR_ALREADY_EXISTS, "create_dir(%s)", &dirname[0] );
            }
            else
                return;
        }
        
        if( ! ::CreateDirectory( &dirname[0], NULL ) )
        {
            throw win32::exception( ::GetLastError(), "create_dir(%s)", &dirname[0] );
        }
#endif
        
    }
    
    void local_fs:: remove_dir(  const string &dirname   )
    {
        YOCTO_GIANT_LOCK();
#if defined(YOCTO_BSD)
        if( rmdir( &dirname[0] ) != 0 ) {
            throw libc::exception( errno, "remove_dir(%s)", &dirname[0] );
        }
#endif
        
#if defined(YOCTO_WIN)
        if(! ::RemoveDirectory( &dirname[0] ) ) {
            throw win32::exception( ::GetLastError(), "remove_dir(%s)", &dirname[0] );
        }
#endif
    }
    
    void local_fs:: remove_file( const string &filename )
    {
        YOCTO_GIANT_LOCK();
#if defined(YOCTO_BSD)
        if( unlink( &filename[0] ) != 0 )
        {
            throw libc::exception( errno, "unlink(%s)", &filename[0] );
        }
#endif
        
#if defined(YOCTO_WIN)
        if( ! ::DeleteFile( &filename[0] ) )
        {
            throw win32::exception( ::GetLastError(), "::DeleteFile(%s)", &filename[0] );
        }
#endif
    }
    
}
