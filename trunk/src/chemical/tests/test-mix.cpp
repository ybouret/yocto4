#include "yocto/utest/run.hpp"
#include "yocto/chemical/initializer.hpp"
#include "yocto/chemical/solution.hpp"
#include "yocto/string/conv.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(mix)
{
    chemical::collection lib;
    vector<chemical::solution> solutions;
    const double Ca = 0.001;
    
    lib.add("H+");
    lib.add("HO-");
    lib.add("Na+");
    lib.add("Cl-");
    lib.add("AcH",0);
    lib.add("Ac-",-1);
    
    
    chemical::equilibria cs;
    const double Kw = 1e-14;
    cs.add_water(lib, Kw);
    cs.add_acid(lib, "acetic", "AcH", "Ac-", pow(10,-4.8));
    
    cs.build_from(lib);
    chemical::solution Sa(lib);
    
    {
        chemical::initializer ini_acid;
        ini_acid.electroneutrality(lib);
        ini_acid.define("Cl-", 0.0);
        ini_acid.define("Na+", 0.0);
        {
            chemical::constraint &all_acid = ini_acid.equals(Ca);
            all_acid["AcH"] = 1;
            all_acid["Ac-"] = 1;
        }
        
        ini_acid(cs,lib,0);
        std::cerr << "C=" << cs.C << std::endl;
        Sa.load(cs.C);
        std::cerr << "Sa=" << Sa << std::endl;
        std::cerr << "Sa.pH=" << Sa.pH() << std::endl;
    }
    
    chemical::solution Sb(lib);
    
    {
        chemical::initializer ini_base;
        ini_base.electroneutrality(lib);
        {
            chemical::constraint &all_acid = ini_base.equals(0.0);
            all_acid["AcH"] = 1;
            all_acid["Ac-"] = 1;
        }
        ini_base.define("Cl-", 0.0);
        ini_base.define("H+", pow(10,-9));
        ini_base(cs,lib,0);
        Sb.load(cs.C);
        std::cerr << "Sb="    << Sb << std::endl;
        std::cerr << "Sb.pH=" << Sb.pH() << std::endl;
        
    }
    
    solutions.push_back(Sa);
    solutions.push_back(Sb);
    
    chemical::solution S(lib);
    
    vector<double> W(2,0.5);
   
    ios::ocstream fp("mix.dat",false);
    const size_t N=100;
    for(size_t i=0;i<=N;++i)
    {
        const double alpha = double(i)/N;
        W[1] = (1.0-alpha);
        W[2] = alpha;
        S.mix(cs,solutions,W,0.0);
        fp("%g %g\n",alpha,S.pH());
    }
    
    
    
    
}
YOCTO_UNIT_TEST_DONE()
