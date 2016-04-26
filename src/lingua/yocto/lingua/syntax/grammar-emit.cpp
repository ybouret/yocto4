#include "yocto/lingua/syntax/walker.hpp"
#include "yocto/lingua/syntax/grammar.hpp"
#include "yocto/fs/vfs.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {



            void  grammar:: emit_prolog(ios::ostream &fp) const
            {
                const string  baseName   = vfs::base_name_from(name) + "_walker";
                const string  className  = vfs::class_name_for(baseName);
                
#if 0
                string        dirName    = vfs::to_directory(dirname);
                const string  baseName   = vfs::base_name_from(name) + "_walker";
                const string  headerName = dirName + baseName + ".hpp";
                const string  guardLabel = vfs::cpp_label_from(baseName) + "_included";
                const string  className  = vfs::class_name_for(baseName);

                ios::wcstream hdr(headerName);
                //ios::wcstream src(sourceName);

                hdr << "#ifndef " << guardLabel << "\n";
                hdr << "#define " << guardLabel << "\n";
                hdr << "#include \"yocto/lingua/syntax/walker.hpp\"\n";

                hdr << "class " << className << " : public yocto::lingua::syntax::walker {\n";
                hdr << "public:\n";

                hdr << "private:\n";
                hdr << "\tYOCTO_DISABLE_COPY_AND_ASSIGN(" << className << ")l;\n";
                hdr << "};\n";
                hdr << "#endif\n";
#endif
            }
            
        }
        
    }
    
}

