#include "yocto/fs/vfs.hpp"
#include <cstring>

namespace yocto
{
	
    const char *  vfs::get_base_name( const char *buffer, size_t buflen ) throw() {
        
        assert( !(buffer==NULL&&buflen>0) );
        
        if( buflen>0 ) {
            const char *bufend     =   buffer+buflen;
            const char *bufcur     =   bufend-1;
            while( bufcur >= buffer ) {
                const int C = *bufcur;
                
                if( C == '/' || C == '\\' ) {
                    ++bufcur;
                    if( bufcur < bufend ) {
                        return bufcur;
                    } else {
                        return NULL;
                    }
                }
                else
                    --bufcur;
            }
            
            /* no slash */
            return buffer;
            
            
        } else {
            return NULL;
        }
    }
    
    const char  * vfs:: get_extension( const char *buffer, size_t buflen ) throw() {
        const char *name_position =  get_base_name(buffer,buflen);
        
        if( name_position ) {
            const char  *bufcur  = buffer + buflen;
            
            while( bufcur >= name_position ) {
                const int C = *(--bufcur);
                if( C == '.' ){
                    return ++bufcur;
                }
                else
                    continue;
            }
            
            /* no dot */
            return NULL;
            
        } else {
            /* no name */
            return NULL;
        }
    }
    
    
    const char *   vfs::get_file_dir( size_t &dirlen, const char *buffer, size_t buflen ) throw() {
        
        const char *file_buf = get_base_name(buffer,buflen);
        if( !file_buf ) {
            
            if( buflen == 0 ) {
                dirlen = 2;
                return "./";
            } else {
                dirlen = buflen;
                return buffer;
            }
            
        } else {
            
            if( file_buf == buffer ) {
                dirlen = 2;
                return "./";
            } else {
                dirlen = file_buf-buffer;
                return buffer;
            }
            
        }
        
        
    }
    
    const char *   vfs:: get_base_name( const char   *path ) throw() {
        return get_base_name( path, length_of( path ) );
    }
    
    const char *   vfs:: get_base_name( const string &path ) throw() {
        return get_base_name( &path[0], path.size() );
    }
    
    const char *   vfs:: get_extension( const char   *path ) throw() {
        return get_extension( path, length_of( path ) );
    }
    
    const char *   vfs:: get_extension( const string &path ) throw() {
        return get_extension( &path[0], path.size() );
    }
    
    const char *   vfs:: get_file_dir( size_t &dirlen, const char *path ) throw() {
        return get_file_dir( dirlen, path, length_of( path ) );
    }
    
    const char *   vfs:: get_file_dir( size_t &dirlen, const string &path ) throw() {
        return get_file_dir( dirlen, &path[0], path.size() );
    }
    
    string      vfs:: get_file_dir( const char   *path, const size_t pathlen )  {
        size_t dirlen = 0;
        const char *dirbuf =  get_file_dir( dirlen, path, pathlen );
        return string(dirbuf,dirlen);
    }
    
    string    vfs:: get_file_dir( const char   *path  )   {
        size_t dirlen = 0;
        const char *dirbuf =  get_file_dir( dirlen, path,  length_of( path ) );
        return string(dirbuf,dirlen);
    }
    
    string      vfs:: get_file_dir( const string &path ) {
        size_t dirlen = 0;
        const char *dirbuf = get_file_dir( dirlen, &path[0], path.size() );
        return string(dirbuf,dirlen);
    }
    
    string & vfs::as_directory( string &path  )
    {
        //-- phase 1: normalize path
        if( path.size() == 0 )
            path = "./";
        else
        {
            string  temp( path.size()+1, as_capacity);
            char last = path[0];
            switch( last ) {
                case '/':
                case '\\':
                    temp.append( (last='/') );
                    break;
                default:
                    temp.append(last);
            }
            
            for( size_t i=1; i < path.size(); ++i ) {
                char look = path[i];
                switch( look ) {
                    case '/':
                    case '\\':
                        look = '/';
                        if( last != '/' )
                            temp.append( look );
                        break;
                        
                    default:
                        temp.append(look);
                }
                
                last = look;
                
            }
            if( last != '/' )
                temp.append('/');
            path = temp;
            
        }
        return path;
    }
    
    void vfs::change_extension( string &path, const string &ext ) {
        const string  &src = path;
        const char    *pos = get_extension( src );
        if( pos )
        {
            const char *org = &src[0];
            string temp( org, pos-org );
            temp += ext;
            path = temp;
        } else
        {
            string temp = path + '.' + ext;
            path = temp;
        }
        
    }
    
    void vfs::remove_extension(string &path) throw() {
        const string  &src = path;
        const char    *pos = get_extension( src );
        if( pos )
        {
            path.trim( length_of(pos)+1 );
        } 
        
    }
    
    
    string  vfs::to_directory( const string &path )
    {
        string temp(path);
        return as_directory( temp );
    }

    string  vfs:: base_name_from(const string &name)
    {
        static const char valid[] =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789"
        "._-";

        const size_t n = name.size();
        string       ans(n,as_capacity);

        for(size_t i=0;i<n;++i)
        {
            char C = name[i];
            if( 0 == strchr(valid, C) )
            {
                C = '_';
            }
            ans.append(C);
        }

        return ans;
    }

    string  vfs::cpp_label_from(const string &name)
    {
        static const char valid[] =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789"
        "_";

        const size_t n = name.size();
        string       ans(n+1,as_capacity);
        ans += '_';
        for(size_t i=0;i<n;++i)
        {
            char C = name[i];
            if( 0 == strchr(valid, C) )
            {
                C = '_';
            }
            ans.append(C);
        }
        return ans;
    }

    string  vfs:: class_name_for(const string &name)
    {
        static const char valid[] =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789"
        "_";

        const size_t n = name.size();
        string       ans(n,as_capacity);

        for(size_t i=0;i<n;++i)
        {
            char C = name[i];
            if( 0 == strchr(valid, C) )
            {
                C = '_';
            }
            ans.append(C);
        }

        return ans;

    }

}


