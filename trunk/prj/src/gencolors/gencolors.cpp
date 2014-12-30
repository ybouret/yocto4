#include "yocto/string.hpp"
#include "yocto/string/tokenizer.hpp"
#include "yocto/exception.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/string/conv.hpp"
#include "yocto/sort/quick.hpp"

#include <iostream>

using namespace yocto;


static inline bool is_sep(const char C ) throw()
{
    return C == ' ' || C == '\t';
}

namespace {
 
    class color
    {
    public:
        const string name;
        const uint8_t r,g,b;
        
        inline color(const string &id, uint8_t R, uint8_t G, uint8_t B) :
        name(id),
        r(R),
        g(G),
        b(B)
        {
        }
        
        inline color(const color &c) :
        name( c.name ),
        r(c.r),
        g(c.g),
        b(c.b)
        {
        }
        
        
        inline ~color() throw()
        {
        }
        
#if 0
        inline friend bool operator<(const color &lhs, const color &rhs ) throw()
        {
            return string::compare(lhs.name, rhs.name);
        }
#endif
        
        static inline int compare(const color &lhs, const color &rhs) throw()
        {
            return string::compare(lhs.name, rhs.name);
        }
        
    private:
        YOCTO_DISABLE_ASSIGN(color);
    };
    
}

int main(int argc, char *argv[])
{
    
    try
    {
        if(argc<=1)
        {
            throw exception("usage: gencolors file.txt");
        }
        
        const string   fn = argv[1];
        vector<string> words(8,as_capacity);
        vector<color>  colors(1000,as_capacity);
        
        {
            ios::icstream fp(fn);
            string line;
            while( fp.read_line(line) >= 0 )
            {
                words.free();
                tokenizer::split(words,line,is_sep);
                if(words.size()>=4)
                {
                    const string  name = words[1];
                    const uint8_t r   = strconv::to_size(words[2],"r");
                    const uint8_t g   = strconv::to_size(words[3],"g");
                    const uint8_t b   = strconv::to_size(words[4],"b");
                    const color   C(name,r,g,b);
                    colors.push_back(C);
                    //std::cerr << C.name << " " << int(C.r) << " " << int(C.g) << " " << int(C.b) << std::endl;
                }
                line.clear();
            }
        }
        quicksort(colors,color::compare);
        const size_t N = colors.size();
        for(size_t i=1;i<=N;++i)
        {
            std::cerr << "\t" << colors[i].name << std::endl;
        }
        std::cerr << N << " colors" << std::endl;
        
        for(size_t i=1;i<=N;++i)
        {
            const color &C = colors[i];
            std::cout << "\t{ \"" << C.name << "\", " << int(C.r) << ", " << int(C.g) << ", " << int(C.b) << " }";
            if(i<N)
                std::cout << ",";
            std::cout << std::endl;
        }
        
        return 0;
    }
    catch(const exception &e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.when() << std::endl;
    }
    catch(...)
    {
        std::cerr << "unhandled exception" << std::endl;
    }
    return 1;
}