#include "yocto/filesys/vfs.hpp"
#include "yocto/string/vfs-utils.hpp"
#include "yocto/auto-ptr.hpp"
#include "yocto/sequence/list.hpp"

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
        
        void vfs:: remove_files( const string &dirname, entry::callback &filter )
        {
            list<string>           matching;
            //-- fill the list of matching path
            {
                auto_ptr<vfs::scanner> scan( this->new_scanner(dirname) );
                const vfs::entry *ep = 0;
                while( 0 != (ep=scan->next()) )
                {
                    if( filter(*ep) )
                        matching.push_back( ep->path );
                }
            }
            
            //-- remove winners
            while( matching.size() )
            {
                this->remove_file( matching.back() );
                matching.pop_back();
            }
        }
	}
}
