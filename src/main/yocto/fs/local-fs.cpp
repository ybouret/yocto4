#include "yocto/fs/local-fs.hpp"
#include "yocto/exceptions.hpp"

#if defined(YOCTO_BSD)
#include <sys/stat.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#endif

#if defined(YOCTO_WIN)

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>
#if defined(__DMC__)
#define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
#endif

#if defined(__GNUC__)
extern "C"
{
BOOL WINAPI GetFileSizeEx(
    HANDLE         hFile,
    PLARGE_INTEGER lpFileSize
);
}
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
            if( ( (EEXIST==err) || (EISDIR==err) )
               &&  allow_already_exists )
            {
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

    uint64_t local_fs:: get_file_size( const string &path ) const
    {
        YOCTO_GIANT_LOCK();

#if defined(YOCTO_BSD)
#  if defined(YOCTO_APPLE) || defined(YOCTO_FREEBSD) || defined(YOCTO_OPENBSD)
#    define Y_FS_STAT stat
#  else
#    define Y_FS_STAT stat64
#  endif
        struct Y_FS_STAT s;
        memset((void*)&s,0,sizeof(s));
        if( Y_FS_STAT( path.c_str(), &s) != 0 )
        {
            throw libc::exception( errno, "stat");
        }
        if( ! S_ISREG(s.st_mode) )
            throw libc::exception( EINVAL, "stat: not a regular file");
        return s.st_size;
#endif

#if defined(YOCTO_WIN)
        LARGE_INTEGER nLargeInteger;// = { 0 } ;
        HANDLE hFile = CreateFile(path.c_str(),
                                  GENERIC_READ,
                                  FILE_SHARE_READ,
                                  NULL,
                                  OPEN_EXISTING,
                                  FILE_ATTRIBUTE_READONLY,
                                  NULL);
        if( INVALID_HANDLE_VALUE == hFile )
        {
            throw win32::exception( ::GetLastError(), "OpenExistingFile" );
        }

        if( !::GetFileSizeEx(hFile, &nLargeInteger) )
        {
            ::CloseHandle(hFile);
            throw win32::exception( ::GetLastError(), "GetFileSizeEx" );
        }

        ::CloseHandle(hFile);
        return uint64_t(nLargeInteger.QuadPart);
#endif
        throw exception("get_file_size not implemented");
    }


}
