#if defined(YOCTO_BSD)
int                 flags      = 0;
static const mode_t unix_flags = (S_IWUSR | S_IRUSR) | (S_IRGRP) | (S_IROTH);

#endif

#if defined(YOCTO_WIN)
DWORD dwDesiredAccess        = 0;
DWORD dwShareMode            = FILE_SHARE_READ | FILE_SHARE_WRITE;
DWORD dwCreationDisposition  = 0;
DWORD dwFlagsAndAttributes   = FILE_ATTRIBUTE_NORMAL;
#endif

/* Setup parameters */
switch( mode )
{
        
    case readable:	
        YRAWFILE(std::cerr << filename << ": readable" << std::endl);
#if defined(YOCTO_BSD)
        flags |= O_RDONLY;
#endif
        
#if defined(YOCTO_WIN)
        dwDesiredAccess        = GENERIC_READ;
        dwCreationDisposition  = OPEN_EXISTING;
        dwFlagsAndAttributes  |= FILE_FLAG_SEQUENTIAL_SCAN;
#endif
        
        break;
        
    case writable:	
        YRAWFILE(std::cerr << filename << ": writable" << std::endl);
#if defined(YOCTO_BSD)
        flags |= O_WRONLY | O_CREAT ;
#endif
        
#if defined(YOCTO_WIN)
        dwDesiredAccess       = GENERIC_WRITE;
        dwCreationDisposition = OPEN_ALWAYS;
        /* dwFlagsAndAttributes |=0; */
#endif
        
        break;
        
    case  readable | writable:		
        YRAWFILE(std::cerr << filename << ": readable|writable" << std::endl);
#if defined(YOCTO_BSD)
        flags |= O_RDWR | O_CREAT;
#endif
        
#if defined(YOCTO_WIN)
        dwDesiredAccess       = GENERIC_READ | GENERIC_WRITE;
        dwCreationDisposition = OPEN_ALWAYS;
        dwFlagsAndAttributes  = FILE_FLAG_RANDOM_ACCESS;
#endif
        
        break;
        
    case  writable | truncate:
        YRAWFILE(std::cerr << filename << ": writable|truncate" << std::endl);
#if defined(YOCTO_BSD)
        flags |= O_WRONLY | O_CREAT | O_TRUNC;
#endif
        
#if defined(YOCTO_WIN)
        dwDesiredAccess       = GENERIC_WRITE;
        dwCreationDisposition = CREATE_ALWAYS;
        /*dwFlagsAndAttributes |=0;*/
#endif
        
        break;
        
        
    case readable | writable | truncate:
        YRAWFILE(std::cerr << filename << ": readable|writable|truncate" << std::endl);
#if defined(YOCTO_BSD)
        flags |= O_RDWR | O_CREAT | O_TRUNC;
#endif
        
#if defined(YOCTO_WIN)
        dwDesiredAccess       = GENERIC_READ | GENERIC_WRITE;
        dwCreationDisposition = OPEN_ALWAYS  | TRUNCATE_EXISTING;
        dwFlagsAndAttributes  = FILE_FLAG_RANDOM_ACCESS;
#endif
        
        break;
        
    default:
#if defined(YOCTO_BSD)
        throw libc::exception( EINVAL, "ios::raw_file('%s',%u)", filename.c_str(),unsigned(mode));
#endif
        
#if defined(YOCTO_WIN)
        throw win32::exception( ERROR_INVALID_PARAMETER, "ios::raw_file('%s',%u)", filename.c_str(),unsigned(mode) );
#endif
        
        
}

/* system calls */
#if defined(YOCTO_BSD)
YOCTO_GIANT_LOCK();
while( (handle = open( filename.c_str(), flags, unix_flags) ) == _fd::invalid() )
{
    const int err = errno;
    switch( err )
    {
        case EINTR:
            break;
            
        default:
            throw libc::exception( err, "open('%s')", filename.c_str() );
    }
}
#endif

#if defined(YOCTO_WIN)
YOCTO_GIANT_LOCK();
handle = CreateFile( TEXT(filename.c_str()),
                    dwDesiredAccess,
                    dwShareMode,
                    NULL,
                    dwCreationDisposition,
                    dwFlagsAndAttributes,
                    NULL);
if(  _fd::invalid() == handle )
{
    throw win32:: exception( ::GetLastError(), "::CreateFile('%s')", filename.c_str() );
}
#endif
