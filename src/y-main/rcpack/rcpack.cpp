#include "yocto/ios/rc.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/exception.hpp"
#include "yocto/fs/local-fs.hpp"
#include "yocto/ptr/auto.hpp"
#include <iostream>

using namespace yocto;

static inline
void __rcpack( const string &resdir, const string &root, ios::resources::packer &rc )
{
    vfs & fs = local_fs::instance();
    auto_ptr<vfs::scanner> scan( fs.new_scanner( resdir ) );
    
    for( const vfs::entry *ep = scan->next(); ep; ep = scan->next() )
    {
        
        if( ep->attr == ep->is_dir )
        {
            const string id = ep->base_name;
            if( id.size() <= 0 )
                throw exception("invalid sub directory name");
            if( id[0] == '.' )
                continue;
            std::cerr << "\t** subdir  " << ep->base_name << std::endl;
            string        sub_dir  = resdir + ep->base_name;
            string        sub_root = root    + ep->base_name;
            __rcpack( vfs::as_directory(sub_dir), vfs::as_directory(sub_root), rc);
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
    const char *progname = vfs::get_base_name(argv[0]);
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
            __rcpack( vfs::as_directory(resdir), root, rc );
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
