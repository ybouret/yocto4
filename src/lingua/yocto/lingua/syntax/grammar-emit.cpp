#include "yocto/lingua/syntax/walker.hpp"
#include "yocto/lingua/syntax/grammar.hpp"
#include "yocto/fs/vfs.hpp"
#include "yocto/ios/ocstream.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {



            void  grammar:: emit(const string &dirname) const
            {
                string        dirName    = vfs::to_directory(dirname);
                const string  baseName   = vfs::base_name_from(name);
                //const string  sourceName = dirName + baseName + ".cpp";
                const string  headerName = dirName + baseName + ".hpp";

                ios::wcstream hdr(headerName);
                //ios::wcstream src(sourceName);

                hdr << "#ifndef " << baseName << "_included\n";
                hdr << "#define " << baseName << "_included\n";
                hdr << "#include \"yocto/lingua/syntax/walker.hpp\"\n";

                hdr << "#endif\n";
            }
            
        }
        
    }
    
}

