#include "yocto/ios/ocstream.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/fs/vfs.hpp"

#include "yocto/string/tokenizer.hpp"
#include "yocto/exception.hpp"
#include "yocto/string/tokenizer.hpp"
#include "yocto/string/conv.hpp"

#include "yocto/sequence/list.hpp"

#include <iostream>
#include <cstring>

using namespace yocto;

static inline bool is_sep(char C) throw()
{
    return C==' ' || C=='\t';
}

struct RGB
{
    uint8_t r,g,b;
    int     was_set;
};

static inline
void parse( ios::istream &fp, RGB colors[256])
{
    static const char *name[4] = { "r", "g", "b", "" };
    string       line;
    size_t       idx = 0;
    list<string> words(32,as_capacity);
    memset(colors,0,256*sizeof(RGB));
    
    while( line.clear(), fp.read_line(line) >= 0 )
    {
        const size_t pos[4] = { idx, idx+64, idx+128, idx+192 };
        //-- check
        ++idx;
        if(idx>64)
            throw exception("Too Many Lines!");
        
        //-- split
        words.free();
        tokenizer::split(words, line, is_sep);
        //std::cerr << words << std::endl;
        if(words.size()!=16)
            throw exception("Invalid Line #%u",unsigned(idx));
        
        //-- fetch
        for(size_t i=0;i<4;++i)
        {
            const size_t j = pos[i]; assert(j<256);
            assert(words.size()>0);
            if( strconv::to<size_t>( words.front(), "index") != j )
            {
                throw exception("Malformed color #%u, line #%u", unsigned(j), unsigned(idx));
            }
            words.pop_front();
            size_t c[4];
            for(unsigned k=0;k<3;++k)
            {
                assert(words.size()>0);
                c[k] = strconv::to<size_t>( words.front(), name[k] );
                words.pop_front();
                if(c[k]>255) throw exception("Malformed color #%u, component '%s'", unsigned(j), name[k]);
            }
            if(colors[j].was_set!=0)
                throw exception("color was already set !!!");
            colors[j].was_set = 1;
            colors[j].r = uint8_t(c[0]);
            colors[j].g = uint8_t(c[1]);
            colors[j].b = uint8_t(c[2]);
        }
    }
}

static inline void make_guard( string &guard )
{
    for(size_t i=0;i<guard.length();++i)
    {
        char c = guard[i];
        
        switch(c)
        {
            case '.':
                c = '_';
                break;
            default:
                c= toupper(c);
        }
        
        guard[i] = c;
    }
}

int main( int argc, char *argv[] )
{
    const char *prog = vfs::get_base_name(argv[0]);
    try
    {
        if( argc != 3 )
            throw exception("usage: %s input.dat base_name", prog);
        
        const string input_filename  = argv[1];
        const string output_basename = argv[2];
        const string header_filename = output_basename + ".hpp";
        const string source_filename = output_basename + ".cpp";
        string       guard           = "YOCTO_GFX_RAMP";
        guard += vfs::get_base_name(input_filename);
        make_guard(guard);
        
        RGB colors[256];
        // load colors
        {
            ios::icstream input( input_filename );
            parse(input,colors);
        }
        
#if 0
        for(size_t i=0;i<256;++i)
        {
            std::cerr << int(colors[i].r) << " " << int(colors[i].g) << " " << int(colors[i].b) << std::endl;
        }
#endif
        
        // write header/source
        ios::ocstream hdr(header_filename,false);
        ios::ocstream src(source_filename,false);
        
        hdr << "#ifndef " << guard << "\n";
        hdr << "#define " << guard << " 1\n";
        hdr << "#include \"yocto/gfx/rgb.hpp\"\n\n";
        hdr << "namespace yocto { namespace gfx {\n\n";
        
        string rc_name = vfs::get_base_name(input_filename);
        vfs::remove_extension(rc_name);
        rc_name = "ramp" + rc_name;
        hdr << "\t\textern const rgb_t " << rc_name << "[256];";
        hdr << "\n}}\n";
        hdr << "#endif\n";
        
        
        src << "#include \"./" << vfs::get_base_name(header_filename) << "\"\n\n";
        src << "namespace yocto { namespace gfx {\n\n";
        src << "\t\t const rgb_t " << rc_name << "[256]=\n{\n";
        for(size_t i=0;i<256;++i)
        {
            const RGB &c = colors[i];
            src("\trgb_t(%3u,%3u,%3u)",unsigned(c.r),unsigned(c.g),unsigned(c.b));
            if(i<255) src << ",";
            src << "\n";
        }
        src << "};\n";
        src << "\n}}\n";

        return 0;
    }
    catch( const exception &e )
    {
        std::cerr << "** in " << prog     << std::endl;
        std::cerr << "*** "   << e.what() << std::endl;
        std::cerr << "*** "   << e.when() << std::endl;
    }
    catch(...)
    {
        std::cerr << "*** Unhandled Exception in " << prog << std::endl;
    }
    return 1;
}
