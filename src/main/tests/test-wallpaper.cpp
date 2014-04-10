#include "yocto/utest/run.hpp"
#include "yocto/fs/local-fs.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/ios/raw-file.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(wallpaper)
{
    vfs &fs = local_fs::instance();
    if(argc<=1)
        return 0;
    //__________________________________________________________________________
    //
    // Root Directory
    //__________________________________________________________________________
    string root = argv[1];
    vfs::as_directory(root);
    {
        auto_ptr<vfs::scanner> scan( fs.new_scanner(root) );
        for(const vfs::entry *ep = scan->next(); ep; ep = scan->next())
        {
            if(!ep->is_subdir())
                continue;
            std::cerr << ep->base_name << std::endl;
            const string lookup = ep->path + "/contents/images/";
            auto_ptr<vfs::scanner> sub;
            try
            {
                sub.reset(fs.new_scanner(lookup));
            }
            catch(const exception &e)
            {
                std::cerr << "*** " << e.what() << std::endl;
                std::cerr << "*** " << e.when() << std::endl;
                continue;
            }
            //std::cerr << "\tscanning " << lookup << std::endl;
            string   px;
            uint64_t sz = 0;
            string   ex;
            for(const vfs::entry *img = sub->next(); img; img=sub->next() )
            {
                if( !img->is_regular()) continue;
                if( !img->extension )   continue;
                string ext = img->extension;
                for(size_t i=0;i<ext.size();++i)
                    ext[i] = tolower(ext[i]);
                if( !(ext=="png" || ext == "jpg" ) )
                    continue;
                const uint64_t nb = fs.get_file_size(img->path);
                if(nb>sz)
                {
                    sz = nb;
                    px = img->path;
                    ex = ext;
                }
            }
            if(sz>0)
            {
                std::cerr << "\t\tGot '" << px << "'" << std::endl;
                const string target = ep->base_name + ("." + ex);
                std::cerr << "\t\tid=" << target << std::endl;
                ios::raw_file::copy(target,px);
            }
        }
        
    }
    
}
YOCTO_UNIT_TEST_DONE()
