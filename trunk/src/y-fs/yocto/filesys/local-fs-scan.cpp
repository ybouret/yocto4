#include "yocto/filesys/local-fs.hpp"
#include "yocto/exceptions.hpp"

#if defined(YOCTO_BSD)
#include <dirent.h>
#include <cerrno>
#endif

#if defined(YOCTO_WIN)
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>
#endif


namespace yocto
{

	namespace filesys
	{
		namespace {

			class local_scanner : public vfs::scanner 
			{
			public:

				const char *local_dir() const throw() 
				{
					return &dir_[0];
				}

#if defined(YOCTO_BSD)
				explicit local_scanner( const string &dirname , const vfs &fs) :
				vfs::scanner( dirname, fs),
					handle_( opendir( local_dir() ) )
				{
					if( NULL == handle_ )
					{
						throw libc::exception( errno, "opendir(%s)", local_dir() );
					}
				}

				virtual const vfs::entry *next(void) {
					errno = 0;
					struct dirent *ep = readdir( handle_ );
					if( !ep ) {
						const int err = errno;
						if( err == 0)
							return NULL;
						throw libc::exception( errno, "readdir(%s)", local_dir() );
					}
					else {
						make_entry( ep->d_name );
						return ent_;
					}
				}
#endif

#if defined(YOCTO_WIN)
				explicit local_scanner( const string &dirname , const vfs &fs) :
				vfs::scanner( dirname, fs),
					hFind_( INVALID_HANDLE_VALUE ),
					hData_(),
					valid_( true )
				{

					const string args = dir_ + '*';
					hFind_ = ::FindFirstFile( &args[0], &hData_ );

					if( hFind_ == INVALID_HANDLE_VALUE ) 
					{
						throw windows::exception( ::GetLastError(), "::FindFirstFile(%s)", local_dir() );
					}
					//std::cerr << "[WIN32] Opened handle to '" << dir_ << "'" << std::endl;
					assert( NULL != hData_.cFileName );


				}

				virtual const vfs::entry *next(void) 
				{
					if(valid_ ) 
					{
						//std::cerr << "[WIN32] has next entry '" << hData_.cFileName << "'" << std::endl;
						make_entry( hData_.cFileName );
						if( ! ::FindNextFile( hFind_, &hData_ ) )
						{
							const DWORD err = ::GetLastError();
							valid_ = false;
							
							if( err != ERROR_NO_MORE_FILES )
							{
								throw windows::exception( err, "::FindNextFile(%s)",local_dir());
							}
						}
						return ent_;
					} 
					else 
					{
						//std::cerr << "[WIN32] NO  next entry." << std::endl;
						free_entry();
						return NULL;
					}
				}
#endif

				virtual ~local_scanner() throw()
				{
#if defined(YOCTO_BSD)
					closedir( handle_ );
#endif

#if defined(YOCTO_WIN)
					//std::cerr << "[WIN32] Closing handle to '" << dir_ << "'" << std::endl;
					::FindClose( hFind_ );
#endif
				}



			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(local_scanner);

#if defined(YOCTO_BSD)
				DIR *handle_;
#endif

#if defined(YOCTO_WIN)
				HANDLE           hFind_;
				WIN32_FIND_DATA  hData_;
				bool             valid_;
#endif
			};
		}

		vfs::scanner *local_fs:: new_scanner( const string &dirname ) const
		{
			return new local_scanner( dirname, *this );
		}

	}

}
