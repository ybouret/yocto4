#include "yocto/program.hpp"
#include "yocto/math/stat/int-hist.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/string/tokenizer.hpp"
#include "yocto/string/conv.hpp"
#include "yocto/code/ipower.hpp"
#include "yocto/container/utils.hpp"
#include "yocto/ios/ocstream.hpp"

#include <cmath>

using namespace yocto;

static inline bool is_sep(char C) throw()
{
    return C == ' ' || C == '\t';
}

#if 0
static inline double smallest_diff( const array<double> &v )
{
    const size_t n     = v.size();
    bool         found = false;
    double       value = 0;
    for(size_t i=1;i<n;++i)
    {
        const double vi = v[i];
        for(size_t j=i+1;j<=n;++j)
        {
            const double vj    = v[j];
            const double delta = fabs(vi-vj);
            if(delta>0)
            {
                if(!found)
                {
                    found = true;
                    value = delta;
                }
                else
                {
                    if(delta<value)
                    {
                        value = delta;
                    }
                }
            }
        }
    }
    if(!found)
    {
        throw exception("cannot find the smallest difference!");
    }
    return value;
}
#endif


YOCTO_PROGRAM_START()
{
    if(argc<=1)
    {
        throw exception("usage: %s datafile",program);
    }

    vector<double> voro(100000,as_capacity);
    size_t n = 0;

    {
        vector<string> words(10,as_capacity);

        ios::icstream fp(argv[1]);
        string line;
        while(fp.read_line(line)>=0)
        {
            ++n;
            words.free();
            tokenizer::split(words, line, is_sep);
            if(words.size()<5)
            {
                throw exception("line %u: missing Voronoi Volume",unsigned(n));
            }
            const double vol = strconv::to<double>(words[5],"Voronoi");
            if(vol<=0)
            {
                throw exception("line %u: invalid Voronoi Volume",unsigned(n));
            }
            voro.push_back(vol);
            line.clear();
        }
    }
    std::cerr << "Read " << n << " lines" << std::endl;

    const double vmin = find_min_of(voro);
    const double vmax = find_max_of(voro);
    std::cerr << "vmin=" << vmin << ", vmax=" << vmax << std::endl;
    const double delta = vmax-vmin;

    vector<size_t> ivoro(n,as_capacity);
    for(size_t i=1;i<=n;++i)
    {
        const double v     = voro[i];
        const size_t score = size_t(floor((n*(v-vmin))/delta + 0.5));
        ivoro.push_back(score);
    }

    vector<size_t> H;
    vector<size_t> bins;
    math::i_histogram(bins, H, ivoro);
    {
        ios::ocstream fp("ihist.dat",false);
        for(size_t i=1;i<=bins.size();++i)
        {
            fp("%u %u\n", unsigned(bins[i]),unsigned(H[i]));
        }
    }
    const size_t thr =math::OtsuThreshold(bins, H);
    const size_t iv   = bins[thr];
    const double tvol = vmin  + (iv*delta)/n;
    std::cerr << "Theshold=" << tvol << std::endl;



}
YOCTO_PROGRAM_END()
