#ifndef YOCTO_IOS_FILE_LOADER_INCLUDED
#define YOCTO_IOS_FILE_LOADER_INCLUDED 1

#include "yocto/sequence/vector.hpp"
#include "yocto/string.hpp"

namespace yocto
{
    namespace ios
    {
        
        typedef vector<char> file_content;
        struct file_loader
        {
            static void append_to( file_content &content, const string &filename);
            static void append_to( file_content &content, const char   *filename);
        };
        
        
    }
    
}

#endif
