#include "yocto/program.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/string/tokenizer.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/string/conv.hpp"

#include <cstring>
#include <cctype>

using namespace yocto;

struct color
{
    int     indx;
    int     r;
    int     g;
    int     b;
};

static inline bool is_sep(const char C) throw()
{
    return ' ' == C || '\t' == C;
}

static inline string build_guard(const string &s)
{
    string guard = s;
    for(size_t i=0;i<guard.size();++i)
    {
        char C = toupper(guard[i]);

        switch(C)
        {
            case '/':
            case '\\':
            case ' ' :
            case '.':
                C = '_';
                break;

            default:
                break;
        }
        guard[i] = C;
    }
    return guard + "_INCLUDED";
}

static inline string build_class(const string &s)
{
    string ans=s;
    for(size_t i=0;i<ans.size();++i)
    {
        char C = (ans[i]);

        switch(C)
        {
            case '/':
            case '\\':
            case ' ' :
            case '.':
                C = '_';
                break;

            default:
                break;
        }
        ans[i] = C;
    }

    return ans;
}


YOCTO_PROGRAM_START()
{
    if(argc!=3)
    {
        throw exception("usage: %s source_file outdir", program);
    }

    //__________________________________________________________________________
    //
    // prepare input
    //__________________________________________________________________________

    const string  input_name = argv[1];
    ios::icstream inp(input_name);
    string        base_name  = vfs::get_base_name(input_name);

    //__________________________________________________________________________
    //
    // prepare colors
    //__________________________________________________________________________
    color col[256];
    memset(col,0,sizeof(col));
    for(size_t i=0;i<256;++i)
    {
        col[i].indx=-1;
    }

    std::cerr << "-- loading '" << base_name << "'" << std::endl;
    {
        size_t   count = 0;
        unsigned iline = 0;
        vector<string> words;
        string line;
        while(line.clear(),inp.read_line(line)>0)
        {
            ++iline;
            words.free();
            tokenizer::split(words, line,is_sep);

            const size_t nw = words.size();
            if(0!=(nw%4)) throw exception("%u:invalid number of items",iline);
            const size_t nc = nw/4;
            for(size_t i=0,j=0;i<nc;++i)
            {
#define __LOAD(VALUE) \
const int VALUE = strconv::to_int(words[++j],#VALUE); \
if(VALUE<0||VALUE>255) throw exception("%u: invalid " #VALUE "=%d", iline, VALUE)

                __LOAD(indx);
                if(col[indx].indx>=0) throw exception("%u: color %d already set", iline, indx);
                __LOAD(r);
                __LOAD(g);
                __LOAD(b);
                col[indx].indx = indx;
                col[indx].r    = r;
                col[indx].g    = g;
                col[indx].b    = b;
                ++count;
            }

        }
        if(count!=256) throw exception("missing colors!");
    }


    string       outdir     = argv[2];
    vfs::as_directory(outdir);
    vfs::remove_extension(base_name);
    const string class_name  = build_class(base_name);
    std::cerr << "-- building " << class_name << " ramp" << std::endl;
    const string header_name = outdir + base_name + ".hpp";
    const string source_name = outdir + base_name + ".cpp";
    std::cerr << "-- saving '" << header_name << "' and '" << source_name << "'" << std::endl;

    //
    {
        ios::wcstream hdr(header_name);
        const string guard = build_guard(header_name);
        hdr << "#ifndef " << guard << "\n";
        hdr << "#define " << guard << " 1\n";
        hdr << "#include \"yocto/graphics/ramp.hpp\"\n";
        hdr << "namespace yocto { namespace graphics{\n\n";

        hdr << "\tclass " << class_name << " : public ramp {\n";
        hdr << "\t\tpublic:\n";
        hdr << "\t\texplicit " << class_name << "() throw();\n";
        hdr << "\t\tvirtual ~" << class_name << "() throw();\n";
        hdr << "\t\tprivate: YOCTO_DISABLE_COPY_AND_ASSIGN(" << class_name << ");\n";
        hdr << "\t};\n";

        hdr << "\n} }\n";
        hdr << "#endif\n";
    }

    {
        ios::wcstream src(source_name);
        src << "#include \"" << header_name << "\"\n";
        src << "namespace yocto { namespace graphics{\n\n";

        // emit codes
        const string codes_name = class_name + "_codes";
        src << "static const RGBA " << codes_name << "[256]={\n";
        for(size_t i=0;i<256;++i)
        {
            const color &c = col[i];
            src("RGBA(0x%02x,0x%02x,0x%02x)",c.r,c.g,c.b);
            if(i<255) src << ',';
            if((i+1)%8==0) src << '\n';
        }
        src << "};\n\n";

        src << class_name  << ":: " << class_name << "() throw() : ramp(" << codes_name << ") {}\n\n";
        src << class_name  << "::~" << class_name << "() throw() {}\n\n";

        src << "\n} }\n";
    }
    
    
}
YOCTO_PROGRAM_END()

