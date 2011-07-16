#include "yocto/filesys/vfs.hpp"
#include "yocto/string/vfs-utils.hpp"
#include <iostream>

namespace yocto
{
	namespace filesys
	{
		vfs::  vfs() throw() {}
		vfs:: ~vfs() throw() {}
		
		
		void vfs:: create_sub_dir( const string &dirName ) 
		{
			string            org = dirName;
			const std::size_t len = _vfs::as_directory( org ).size();
			string            sub( len, as_capacity );
			std::size_t       idx = 0;
			while( idx < len ) 
			{
				while( org[idx] != '/' && org[idx] != '\\' ) {
					sub +=  org[idx++];
				}
				sub += org[idx++];
				
				//std::cerr << "sub=[" << sub << "]" << std::endl;
				
				this->create_dir( sub, true );
			}
		}
		
		bool vfs::is_reg( const string &path ) const throw() {
			bool link = false;
			return entry::is_reg == this->query_attribute( path, link );
		}
		
		bool vfs::is_dir( const string &path ) const throw() {
			bool link = false;
			return entry::is_dir == this->query_attribute( path, link );
		}
	}
}
