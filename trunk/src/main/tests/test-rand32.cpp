#include "yocto/utest/run.hpp"
#include "yocto/code/rand32.hpp"
#include "yocto/sys/wtime.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/ios/ocstream.hpp"
#include <cmath>
#include <ctime>

using namespace yocto;

namespace 
{
    struct ugen
    {
        const char *name;
        urand32    *proc;
    };
#define __UGEN(NAME)  { #NAME, &__##NAME }
    
    
    
    static inline 
    void ave_and_sig( double &ave, double &sig, const array<double> &r )
    {
        const size_t n = r.size();
        ave = 0;
        for( size_t i=n;i>0;--i)
        {
            ave += r[i];
        }
        
        ave /= n;
        if(n>1)
        {
            sig = 0;
            for( size_t i=n; i>0; --i )
            {
                const double dr = r[i] - ave;
                sig += dr*dr;
            }
            sig = sqrt(sig/(n-1));
        }
        else 
        {
            sig = fabs(ave);
        }
        
    }
    
#define NGEN 10000000
#define NCVG 1024
#define ITER 32
#define NSHUFFLE 2048
#define NCORR    2048
    
    static inline
    void process( ugen &gen, ios::ocstream &out )
    {
        static const double AVE = 0.5;
        static const double SIG = sqrt(1.0/12.0);
        wtime             chrono;
        
        std::cerr << "-- processing " << gen.name << std::endl;
        out("[%s]\n", gen.name);
        {
            std::cerr << "\t[RATE]" << std::endl;
            chrono.start();
            for( size_t i=0; i < NGEN; ++i )
            {
                (void)gen.proc->next();
            }
            double ell = chrono.query();
            std::cerr << "\tNGEN=" << NGEN << " in " << ell << "s" << std::endl;
            const double rate = (NGEN/ell)*1e-6;
            std::cerr << "\trate=" << rate << std::endl;
            out("\trate= %.4f Mups\n", rate);
        }
        
        {
            std::cerr << "\t[FP QUALITY]" << std::endl;
            vector<double> r(NCVG,as_capacity);
            string filename = gen.name;
            filename += "_cvg.dat";
            ios::ocstream fp( filename, false );
            double ave=0;
            double sig=0;
            double a_err = 0;
            double s_err = 0;
            for( size_t iter=0; iter < ITER; ++iter )
            {
                r.free();
                for( size_t i=0; i < NCVG; ++i )
                {
                    r.push_back( gen.proc->get<double>() );
                    ave_and_sig(ave, sig, r);
                    if(iter==0) fp("%u %g %g\n", unsigned(i+1), ave, sig );
                    a_err += fabs(ave-AVE);
                    s_err += fabs(sig-SIG);
                }
            }
            a_err /= (NCVG*ITER);
            s_err /= (NCVG*ITER);
            std::cerr << "\ta_err=" << a_err << std::endl;
            std::cerr << "\ts_err=" << s_err << std::endl;
            out("\ta_err= %.3f ppt\n", a_err*1000);
            out("\ts_err= %.3f ppt\n", s_err*1000);
        }
        
        {
            std::cerr << "\t[SHUFFLE]" << std::endl;
            const size_t blocks[] = { 16, 32, 64, 128 };
            for( size_t b=0; b < sizeof(blocks)/sizeof(blocks[0]); ++b )
            {
                
                const size_t blk = blocks[b];
                vector<size_t> indx(blk,0);
                std::cerr << "\t\tblk=" << blk << std::endl;
                double permTmx = 0;
                vector<double> v1,v2,v3;
                const size_t   i1 = 1, i2=blk/2, i3=blk;
                for( size_t i=0; i < NSHUFFLE; ++i )
                {
                    for(size_t j=1; j <= blk; ++j )
                    {
                        indx[j] = j-1;
                    }
                    chrono.start();
                    gen.proc->shuffle(indx(), blk);
                    permTmx += chrono.query();
                    v1.push_back( double(indx[i1]) );
                    v2.push_back( double(indx[i2]) );
                    v3.push_back( double(indx[i3]) );
                }
                double ave1,sig1,ave2,sig2,ave3,sig3;
                ave_and_sig(ave1, sig1, v1); ave1 /= (blk-1); sig1 /= (blk-1);
                ave_and_sig(ave2, sig2, v2); ave2 /= (blk-1); sig2 /= (blk-1);
                ave_and_sig(ave3, sig3, v3); ave3 /= (blk-1); sig3 /= (blk-1);
                const double rate = (blk*NSHUFFLE/permTmx)*1e-6;
                std::cerr << "\t\t\trate=" << rate << std::endl;
                std::cerr << "\t\t\tave1=" << ave1 << ", sig1=" << sig1 << std::endl;
                std::cerr << "\t\t\tave2=" << ave2 << ", sig2=" << sig2 << std::endl;
                std::cerr << "\t\t\tave3=" << ave3 << ", sig3=" << sig3 << std::endl;
                out("\tshuffle%4u: rate= %8.3f Mpps\n", unsigned(blk), rate);
                out("\t\tave1: %.4f   sig1: %.4f\n", ave1, sig1 );
                out("\t\tave2: %.4f   sig2: %.4f\n", ave2, sig2 );
                out("\t\tave3: %.4f   sig3: %.4f\n", ave3, sig3 );
                
            }
            
            {
                std::cerr << "\t[CORRELATION]" << std::endl;
                vector<double> v1;
                vector<double> v2;
                uint32_t seed = uint32_t(time(NULL));
                double corr = 0;
                for( size_t iter=0; iter < ITER; ++iter )
                {
                    v1.free();
                    gen.proc->seed(seed++);
                    for( size_t i=0; i < NCORR; ++i ) v1.push_back( gen.proc->get<double>() );
                    v2.free();
                    gen.proc->seed(seed++);
                    for( size_t i=0; i < NCORR; ++i ) v2.push_back( gen.proc->get<double>() );
                    double ans = 0;
                    for( size_t i=NCORR; i >0;--i)
                        ans += (v1[i]-0.5) * (v2[i] - 0.5);
                    ans /= NCORR;
                    //std::cerr << "\t\t\t-->" << ans << std::endl;
                    corr += ans;
                }
                corr /= ITER;
                std::cerr << "\t\tcorr=" << corr << std::endl;
                out("\tcorr: %.4g\n",corr);
            }
        }
        
    }
}

YOCTO_UNIT_TEST_IMPL(rand32)
{
    core::rand32::test();
    
    rand32_mwc   __mwc;
    rand32_shr3  __shr3;
    rand32_cong  __cong;
    rand32_fib   __fib;
    rand32_kiss  __kiss;
    rand32_lfib4 __lfib4;
    rand32_swb   __swb;
    ugen reg[] =
    {
        __UGEN(mwc),
        __UGEN(shr3),
        __UGEN(cong),
        __UGEN(fib),
        __UGEN(kiss),
        __UGEN(lfib4),
        __UGEN(swb)
    };
    
    ios::ocstream out("rand32.dat",false);
    const size_t num = sizeof(reg)/sizeof(reg[0]);
    for(size_t i=0; i < num; ++i )
    {
        process(reg[i],out);
    }
    
}
YOCTO_UNIT_TEST_DONE()

