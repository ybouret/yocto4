#ifndef YOCTO_STRING_VFS_FILE_NAME_INCLUDED
#define YOCTO_STRING_VFS_FILE_NAME_INCLUDED 1

#include "yocto/string.hpp"

namespace yocto
{
	
	struct _vfs
	{
		static const char * get_base_name( const char *buffer, size_t buflen ) throw();
		static const char * get_extension(  const char *buffer, size_t buflen ) throw();
		static const char * get_file_dir( size_t &dirlen, const char *buffer, size_t buflen ) throw();
		
		static const char * get_base_name( const char   *path ) throw();
		static const char * get_base_name( const string &path ) throw();
		
		static const char * get_extension( const char   *path ) throw();
		static const char * get_extension( const string &path ) throw();
		
		static const char * get_file_dir( size_t &dirlen, const char   *path) throw();
		static const char * get_file_dir( size_t &dirlen, const string &path) throw();
		
		static string get_file_dir( const char   *path, size_t pathlen );
		static string get_file_dir( const char   *path );
		static string get_file_dir( const string &path );
		
		
		static string &as_directory( string &path );
		static string  to_directory( const string &path );
		static void    change_extension( string &path, const string &ext );
		
	};
	
}

#endif
