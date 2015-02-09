#include "yocto/utest/run.hpp"
#include "yocto/hashing/sha1.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/associative/map.hpp"

using namespace yocto;

static inline bool is_space(char C) throw()
{
    return C==' ' || C == '\t';
}

YOCTO_UNIT_TEST_IMPL(uuid)
{
    hashing::sha1 H;
    ios::icstream fp( ios::cstdin );
    map<uint32_t,string> ids;
    string line;
    while( fp.read_line(line) >= 0 )
    {
        line.clean(is_space);
        const uint32_t uuid = H.key<uint32_t>(line);
        //std::cerr << "'"  << line << "',\t#" << line.size() << "\t=>\t" << uuid << std::endl;
        if(!ids.insert(uuid,line))
        {
            std::cerr << "Collision!" << std::endl;
        }
        line.clear();
    }

}
YOCTO_UNIT_TEST_DONE()
