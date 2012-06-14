#include "yocto/ios/rc.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/string/vfs-utils.hpp"
#include "yocto/exception.hpp"
#include "yocto/filesys/local-fs.hpp"
#include "yocto/auto-ptr.hpp"
#include <iostream>

using namespace yocto;

static inline
void __rcpack( const string &resdir, const string &root, ios::resources::packer &rc )
{
    filesys::vfs & fs = filesys::local_fs::instance();
    auto_ptr<filesys::vfs::scanner> scan( fs.new_scanner( resdir ) );
    
    for( const filesys::vfs::entry *ep = scan->next(); ep; ep = scan->next() )
    {
        
        if( ep->attr == ep->is_dir )
        {
            if( (strcmp(ep->base_name, ".") == 0) || strcmp(ep->base_name,"..") == 0  )
            {
                continue;
            }
            std::cerr << "\t** subdir  " << ep->base_name << std::endl;
            string        sub_dir  = resdir + ep->base_name;
            string        sub_root = root    + ep->base_name;
            __rcpack( _vfs::as_directory(sub_dir), _vfs::as_directory(sub_root), rc);
        }
        else 
        {
            const string  rcname = root + ep->base_name;
            std::cerr << "\t** packing " << rcname << std::endl;
            rc(rcname,ep->path);
        }
    }
    
    
}

int main( int argc, char *argv[] )
{
    const char *progname = _vfs::get_base_name(argv[0]);
    try
    {
        if( argc <= 2 )
            throw exception("usage: %s executable resources_directory", progname);
        const string exename  = argv[1];
        error_type   status   = 0;
        string       resdir  = argv[2];
        
        std::cerr << "**** Packing resources from [" << resdir << "]" << std::endl;
        std::cerr << "**** into <" << exename << ">" << std::endl;
        {
            ios::resources::packer rc( exename, status);
            string root = "";
            __rcpack( _vfs::as_directory(resdir), root, rc );
        }
        
        
        return 0;
    }
    catch( const exception &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.when() << std::endl;
    }
    catch(...)
    {
        std::cerr << "unhandled exception in " << progname << std::endl;
    }
    return -1;
}
