#include "yocto/associative/htree.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/sequence/vector.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(htree)
{
    htree<double> H;
    ios::icstream fp( ios::cstdin );
    string line;
    size_t i=0;
    vector<string> keys;
    while( fp.gets(line) )
    {
        if(!H.insert(line, new double(++i) ))
        {
            throw exception("multiple keys '%s'", line.c_str());
        }
        keys.push_back(line);
    }
    std::cerr << "keys=" << keys << std::endl;
    for(size_t i=1;i<=keys.size();++i)
    {
        const double *pp = H.find(keys[i]);
        if(!pp) throw exception("cannot find '%s'", keys[i].c_str());
        std::cerr << keys[i] << " => " << *pp << std::endl;
    }
}
YOCTO_UNIT_TEST_DONE()
