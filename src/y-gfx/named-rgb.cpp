#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/exception.hpp"
#include "yocto/string/tokenizer.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/fs/vfs.hpp"
#include "yocto/string/conv.hpp"
#include "yocto/sort/heap.hpp"

using namespace yocto;

static inline bool is_sep(char C) throw()
{
    return C == ' ' || C == '\t';
}

class named
{
public:
    const string    name;
    const unsigned  r,g,b;
    
    named( const string &id, unsigned R, unsigned G, unsigned B) :
    name(id),
    r(R),
    g(G),
    b(B)
    {
    }
    
    
    named( const named &other ) :
    name( other.name ),
    r(other.r),
    g(other.g),
    b(other.b)
    {
    }
    
    
    inline ~named() throw() {}
    
    static inline int compare( const named &lhs, const named &rhs ) throw()
    {
        return string::compare(lhs.name, rhs.name);
    }
    
private:
    YOCTO_DISABLE_ASSIGN(named);
};

int main(int argc, char *argv[] )
{
    const char *prog = vfs::get_base_name(argv[0]);
    try
    {
        if( argc <= 2 )
            throw exception("usage: %s named.txt output.inc",prog);
        
        const string input_name  = argv[1];
        const string output_name = argv[2];
        
        std::cerr << "-- " << input_name << " ==> " << output_name << std::endl;
        
        ios::icstream inp(input_name);
        ios::ocstream fp(output_name,false);
        
        string         line;
        vector<string> seq;
        vector<named>  colors;
        size_t         name_max = 0;
        while( line.clear(), inp.read_line(line) > 0 )
        {
            seq.free();
            tokenizer::split(seq, line,is_sep);
            if(seq.size()>=4)
            {
                const string   name = seq[1];
                const unsigned r    = strconv::to<int>( seq[2], "r" );
                const unsigned g    = strconv::to<int>( seq[3], "g" );
                const unsigned b    = strconv::to<int>( seq[4], "b" );
                const named color(name,r,g,b);
                colors.push_back(color);
                if( name.size() > name_max ) name_max = name.size();
            }
        }
        hsort(colors, named::compare);
        std::cerr << "-- parsed " << colors.size() << " colors" << std::endl;
        for(size_t i=1; i <= colors.size(); ++i )
        {
            const named &c = colors[i];
            //std::cerr << c.name << std::endl;
            fp << "\t{";
            fp << "\"" << c.name << "\",";
            for(size_t j=c.name.size();j<=name_max;++j) fp << ' ';
            fp("%3u, %3u, %3u ", c.r, c.g, c.b);
            fp << "}";
            if(i<colors.size())
                fp << ",";
            fp << "\n";
        }
        
        return 0;
    }
    catch( const exception &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.when() << std::endl;
    }
    catch(...)
    {
        std::cerr << "unhandled exception in " << prog << std::endl;
    }
    return 1;
}
