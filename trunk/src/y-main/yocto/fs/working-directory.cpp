#include "yocto/fs/working-directory.hpp"
#include "yocto/memory/pooled.hpp"
#include "yocto/memory/buffers.hpp"

#include "yocto/exceptions.hpp"
#include "yocto/fs/local-fs.hpp"

#if defined(YOCTO_BSD)
#include <unistd.h>
#include <cerrno>
#endif

#if defined(YOCTO_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif


namespace yocto
{
	
    
    const char working_directory::name[] = "working-dir";
    
    working_directory:: working_directory() throw()
    {
    }
    
    working_directory:: ~working_directory() throw()
    {
    }
    
    
    string working_directory:: get() const
    {
        YOCTO_LOCK( access );
        
#if defined(YOCTO_BSD)
        size_t length = 64;
        while( true )
        {
            memory::buffer_of<char,memory::pooled> buffer( length );
            YOCTO_GIANT_LOCK();
            const char   *wd = getcwd( buffer(), buffer.length() );
            if( !wd )
            {
                const int err = errno;
                if( err != ERANGE )
                    throw libc::exception( err, "getcwd()");
                length *= 2;
            }
            else
            {
                string ans(wd);
                return string( vfs::as_directory( ans ));
            }
        }
#endif
        
#if defined(YOCTO_WIN)
        YOCTO_GIANT_LOCK();
        const DWORD len = ::GetCurrentDirectory(0,NULL);
        if( !len )
            throw win32::exception( ::GetLastError(), "::GetCurrentDirectory()" );
        
        memory::buffer_of<char,memory::pooled> buffer( len+1 ); assert( buffer.length() > len );
        const DWORD res = ::GetCurrentDirectory( len, buffer() );
        if( !res )
            throw win32::exception( ::GetLastError(), "::GetCurrentDirectory()" );
        if( res > len )
            throw exception( "CurrentDirectory changed!");
        string ans( buffer() );
        return string( vfs::as_directory( ans ) );
#endif
        
        
    }
    
    
    void working_directory:: set( const string &dir_name ) const
    {
        YOCTO_LOCK(access);
        
        const char *wd = & dir_name[0] ;
#if defined(YOCTO_WIN)
        if( ! ::SetCurrentDirectory( wd ) )
            throw win32::exception( GetLastError(), "::SetCurrentDirectory(%s)", wd);
#endif
        
#if defined(YOCTO_BSD)
        if( chdir( wd ) != 0 )
            throw libc::exception( errno, "chdir(%s)", wd );
#endif
    }
    
    
    
    
    working_directory:: change_lock:: change_lock( const string &target, bool *status ) :
    source_( working_directory:: instance(). get() ),
    status_( status )
    {
        if( status_ ) *status_ = true;
        try
        {
            working_directory:: instance().set( target );
        }
        catch(...)
        {
            if( status_ ) *status_ = false;
            throw;
        }
    }
    
    
    working_directory:: change_lock:: change_lock( const char *target, bool *status ) :
    source_( working_directory:: instance(). get() ),
    status_( status )
    {
        if( status_ ) *status_ = true;
        try
        {
            const string tgt(target);
            working_directory:: instance().set( tgt );
        }
        catch(...)
        {
            if( status_ ) *status_ = false;
            throw;
        }
    }
    
    
    
    
    working_directory:: change_lock:: ~change_lock()throw()
    {
        try
        {
            assert( working_directory::exists() );
            working_directory::location()->set( source_ );
        }
        catch(...)
        {
            if( status_ ) *status_ = false;
        }
    }
    
	
}

