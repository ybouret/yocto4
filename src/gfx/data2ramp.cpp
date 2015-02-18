#include "yocto/program.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/string/tokenizer.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/ordered/sorted-vector.hpp"
#include "yocto/string/conv.hpp"

using namespace yocto;

struct RGB
{
    int r,g,b;
};

static inline bool is_sep(char C) throw() {

    switch(C)
    {
        case ' ':
        case '\t':
        case '\r':
        case '\n':
            return true;

        default:break;
    }
    return false;
}

YOCTO_PROGRAM_START()
{
    for(int i=1;i<argc;++i)
    {
        const string input_name = argv[i];
        string header_name = input_name;
        vfs::change_extension(header_name, "hpp");

        string source_name = input_name;
        vfs::change_extension(source_name, "cpp");

        std::cerr << "--> " << header_name << ", " << source_name << std::endl;

        string ramp_name = vfs::get_base_name(input_name);
        vfs::remove_extension(ramp_name);
        for(size_t j=0;j<ramp_name.length();++j)
        {
            char C = ramp_name[j];

            switch(C)
            {
                case ' ':

                    C = '_';
                    break;

                default:
                    break;
            }
            ramp_name[j] = C;
        }
        std::cerr << "--> ramp = " << ramp_name << std::endl;

        // parsing file
        string content;
        {
            ios::icstream fp(input_name);
            char C=0;
            while( fp.query(C) ) content.append(C);
        }
        vector<string> words;
        tokenizer::split(words, content, is_sep);
        if(words.size()!=1024)
        {
            throw exception("not enough words...");
        }


        RGB colors[256];
        memset(colors,0,sizeof(colors));

        sorted_vector<size_t> indices;

        size_t u=0;
        for(size_t j=0;j<256;++j)
        {
            const size_t k = strconv::to<size_t>(words[++u],"index");
            if(k>255)
                throw exception("invalid index #%u", unsigned(k));

            if(!indices.insert(k))
                throw exception("multiple index #%u", unsigned(k));

            const size_t r = strconv::to<size_t>(words[++u],"r");
            const size_t g = strconv::to<size_t>(words[++u],"g");
            const size_t b = strconv::to<size_t>(words[++u],"b");
            if(r>255||g>255||b>255)
                throw exception("invalid (%u,%u,%u)\n", unsigned(r),unsigned(g),unsigned(b));

            colors[k].r = r;
            colors[k].g = g;
            colors[k].b = b;

        }

        string ramp_guard = "YOCTO_GFX_RAMP_" + ramp_name + "_INCLUDED";
        ramp_guard.to_upper();


        ios::ocstream header(header_name,false);
        ios::ocstream source(source_name,false);

        header << "#ifndef " << ramp_guard << "\n";
        header << "#define " << ramp_guard << " 1\n";
        header << "#include \"yocto/gfx/ramp.hpp\"\n";
        header << "\n";

        // declaration
        header << "namespace yocto { namespace gfx {\n";
        header << "\tclass " << ramp_name << " : public ramp {\n";
        header << "\tpublic:\n";
        header << "\t\tvirtual ~" << ramp_name << "() throw();\n";
        header << "\t\texplicit " << ramp_name << "() throw();\n";
        header << "\tprivate: YOCTO_DISABLE_COPY_AND_ASSIGN(" << ramp_name << ");\n";
        header << "\t};\n";

        header << "}}\n";
        header << "#endif\n";

        source << "#include \"./" << vfs::get_base_name(header_name) << "\"\n";
        source << "namespace yocto { namespace gfx {\n";
        source << "\tstatic const rgb_t __" << ramp_name << "[256]={\n";

        for(size_t j=0;j<256;++j)
        {
            source("\trgb_t(%3d,%3d,%3d)",int(colors[j].r),int(colors[j].g),int(colors[j].b));
            if(j<255)
                source << ',';
            source << '\n';
        }


        source << "\t};\n";
        source << "\t" << ramp_name << ":: " << ramp_name << "() throw() : ramp(__" << ramp_name << ") {}\n";
        source << "\t" << ramp_name << "::~" << ramp_name << "() throw() {}\n";
        source << "}}\n";
        
    }
    
}
YOCTO_PROGRAM_END()