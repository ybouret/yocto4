#include "yocto/program.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ordered/sorted-vector.hpp"

using namespace yocto;


class NamedColor
{
public:
    const string  name;
    const uint8_t r,g,b;

    inline NamedColor(const string &id, const uint8_t R, const uint8_t G, const uint8_t B) :
    name(id),
    r(R), g(G), b(B)
    {
    }

    inline ~NamedColor() throw() {}

    inline NamedColor(const NamedColor &other) :
    name(other.name),
    r(other.r),
    g(other.g),
    b(other.b)
    {
    }


    struct Comparator
    {
        inline int operator()( const NamedColor &lhs, const NamedColor &rhs ) throw()
        {
            return string::compare(lhs.name,rhs.name);
        }
    };

private:
    YOCTO_DISABLE_ASSIGN(NamedColor);
};

#include "yocto/string/conv.hpp"
#include "yocto/string/tokenizer.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/code/utils.hpp"

static inline bool is_sep(const char C) throw()
{
    return C == ' ' || C == '\t';
}

static inline string check_name(const string &ID)
{
    string ans;
    for(size_t i=0;i<ID.size();++i)
    {
        char C = toupper(ID[i]);

        switch(C)
        {
            case ' ':
            case '\t':
            case '-':
                C = '_';
                break;
        }
        ans += C;
    }
    return ans;
}
YOCTO_PROGRAM_START()
{

    if(argc<=1)
    {
        throw exception("usage: %s colors_file.txt", program);
    }

    const string filename = argv[1];

    sorted_vector<NamedColor,NamedColor::Comparator> colors;
    size_t lmax = 0;
    {
        vector<string> words(8,as_capacity);

        string        line;
        ios::icstream fp(filename);
        while( fp.gets(line) )
        {
            words.free();
            tokenizer::split(words,line,is_sep);
            if(words.size()>=4)
            {
                //std::cerr << words << std::endl;
                const string name = check_name(words[1]);
                const size_t r    = strconv::to_size(words[2],"r"); if(r>255) throw exception("unexpected %s.r=%u", name.c_str(),unsigned(r));
                const size_t g    = strconv::to_size(words[3],"g"); if(g>255) throw exception("unexpected %s.g=%u", name.c_str(),unsigned(g));
                const size_t b    = strconv::to_size(words[4],"b"); if(b>255) throw exception("unexpected %s.b=%u", name.c_str(),unsigned(b));

                std::cerr << "--> " << name << " " << r << " " << g << " " << b << std::endl;
                const NamedColor col(name,r,g,b);
                if(!colors.insert(col))
                {
                    throw exception("multiple color '%s'", name.c_str());
                }
                lmax = max_of(lmax,name.size());
            }
        }
        std::cerr << "#colors=" << colors.size() << std::endl;
    }

    ios::wcstream hdr("named-colors-def.hpp");
    ios::wcstream src("named-colors-def.inc");
    const size_t n = colors.size();
    for(size_t i=1;i<=n;++i)
    {
        const NamedColor &col = colors[i];
        hdr << "#define YGFX_" << col.name;
        for(size_t i=col.name.size();i<lmax;++i) hdr << ' ' ;
        hdr(" %u\n", unsigned(i-1));

        src << "{\"" << col.name << "\"";
        for(size_t i=col.name.size();i<lmax;++i) src << ' ' ;
        src(",%3u,%3u,%3u", unsigned(col.r), unsigned(col.g), unsigned(col.b) );
        src << "}";
        if(i<n) src << ",";
        src << "\n";
    }




}
YOCTO_PROGRAM_END()