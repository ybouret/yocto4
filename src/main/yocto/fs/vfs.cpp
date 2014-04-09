#include "yocto/fs/vfs.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/sequence/list.hpp"

#include <iostream>

namespace yocto
{
	
    vfs::  vfs() throw() {}
    vfs:: ~vfs() throw() {}
    
   
    
    void vfs:: create_sub_dir( const string &dirName )
    {
        string            org = dirName;
        const std::size_t len = vfs::as_directory( org ).size();
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
    
    void vfs:: remove_files( const string &dirname, bool (*filter)( const entry &) )
    {
        assert(filter);
        entry::callback cb( cfunctor(filter) );
        remove_files( dirname, cb );
    }
    
    namespace
    {
        class rm_wrapper
        {
        public:
            const string &ext;
            rm_wrapper( const string &user_ext ) throw() : ext(user_ext) {}
            inline ~rm_wrapper() throw() {}
            
            inline bool has_ext( const vfs::entry &ep ) throw()
            {
                return  ep.has_extension(ext);
            }
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(rm_wrapper);
        };
    }
    
    void vfs:: remove_files_with_extensions( const string &dirname, const string &extension)
    {
        rm_wrapper w(extension);
        entry::callback cb( &w, & rm_wrapper::has_ext );
        remove_files(dirname,cb);
    }

    
}
