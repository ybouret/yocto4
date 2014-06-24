#include "yocto/ios/file-loader.hpp"
#include "yocto/ios/raw-file.hpp"
#include "yocto/exception.hpp"

#include <cstdio>

namespace yocto
{
    
    namespace ios
    {
     
        void file_loader:: append_to( file_content &content, const string &filename)
        {
            raw_file     fp(filename, readable);
            const size_t sz = fp.length();
            content.reserve(sz);
            
            char buffer[BUFSIZ];
            size_t count = 0;
            while(count<sz)
            {
                const size_t done = fp.get(buffer, sizeof(buffer) );
                if(!done)
                    throw exception("file_loader(missing bytes for '%s')", filename.c_str());
                for(size_t i=0;i<done;++i) content.push_back(buffer[i]);
                count += done;
            }
        }
        
        void file_loader:: append_to( file_content &content, const char   *filename)
        {
            const string fn(filename);
            append_to(content,fn);
        }
        
    }
}
