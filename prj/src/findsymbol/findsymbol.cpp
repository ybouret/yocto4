#include "yocto/rtld/dylib.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/fs/local-fs.hpp"
#include "yocto/sequence/list.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/rtld/module.hpp"

#include <iostream>

using namespace yocto;

static inline void process_folder( const string &folder_name )
{
    vfs &fs = local_fs::instance();
    auto_ptr<vfs::scanner> scan( fs.new_scanner(folder_name) );
    const vfs::entry *ep = 0;
    char  buffer[256];
    while( 0 != (ep=scan->next()) )
    {
        if(ep->is_regular() && ! ep->link )
        {
            std::cerr << "Trying " << ep->path << std::endl;
            dylib *dll = dylib_load( ep->path.c_str(), buffer, sizeof(buffer) );
            if( dll )
            {
                std::cerr << ep->path << " is a dll" << std::endl;
                std::cerr << std::endl;
                dylib_free(dll);
            }
        }
    }
}


int main( int argc, char *argv[] )
{
    const char *prog = vfs::get_base_name(argv[0]);
    try
    {
        if(argc<2)
        {
            throw exception("need a symbol");
        }
        const string symbol = argv[1];
        std::cerr << "-- looking for " << symbol << std::endl;
        vfs &fs = local_fs::instance();
        list<string> folders;
        
        if(argc>2)
        {
            for(int i=2;i<argc;++i)
            {
                const string tmp = argv[i];
                folders.push_back( tmp );
            }
        }
        else
        {
            const string empty;
            folders.push_back(empty);
        }
        
        for( list<string>::iterator i=folders.begin(); i != folders.end(); ++i)
        {
            string &f = *i;
            fs.as_directory(f);
            std::cerr << "+ " << f << std::endl;
            process_folder(f);
        }
        
        
        
        
    }
    catch( const exception &e )
    {
        std::cerr << "*** in <" << prog <<">" << std::endl;
        std::cerr << "*** " << e.what() << std::endl;
        std::cerr << "*** " << e.when() << std::endl;
        return 0;
    }
    catch(...)
    {
        std::cerr << "*** in <" << prog <<">" << std::endl;
        std::cerr << "Unhandled exception!" << std::endl;
    }
    return 1;
}

