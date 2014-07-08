#include "yocto/exception.hpp"
#include "yocto/sequence/vector.hpp"

#include "yocto/lua/lua-state.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/lua/lua-maths.hpp"

#include "yocto/fs/vfs.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"

#include "yocto/string/conv.hpp"

#include "yocto/math/stat/descr.hpp"
#include "yocto/math/fcn/intg.hpp"
#include "yocto/math/dat/linear.hpp"
#include "yocto/math/fcn/zfind.hpp"

using namespace yocto;


class Student
{
public:
    const string name;
    size_t       whom;
    const int    indx;
    double       grade;
    double       score;
    
    inline Student( const string &id, size_t by, double result) :
    name(id),
    whom(by),
    indx(++Counter),
    grade(result),
    score(0)
    {
    }
    
    inline ~Student() throw() {}
    
    Student( const Student &other ) :
    name( other.name ),
    whom( other.whom ),
    indx( other.indx ),
    grade( other.grade ),
    score( other.score )
    {
    }
    
    
private:
    static int Counter;
    
};

int Student::Counter = 0;

class GradeDist
{
public:
    
    const double vMin;
    const double vMax;
    const double vFac;
    
    
    GradeDist( lua_State *L, const string &fn ) :
    vMin( Lua::Config::Get<lua_Number>(L,"vMin") ),
    vMax( Lua::Config::Get<lua_Number>(L,"vMax") ),
    vFac(1),
    pdf(L,fn,true),
    PDF(this,&GradeDist::compute_pdf),
    intg(),
    ICDF(this,&GradeDist::shifted_cdf),
    solve(1e-4)
    {
        if(vMin>=vMax)
            throw exception("Invalid min/max");
        
        const double sum = intg(vMin,vMax,PDF,1e-4);
        if(sum<=0)
            throw exception("Invalid PDF");
        (double&)vFac = 1.0/sum;
        std::cerr << "vFac=" <<vFac << std::endl;
    }
    
    
    ~GradeDist() throw()
    {
    }
    
    double compute_pdf(double x)
    {
        return pdf(x);
    }
    
    double compute_cdf(double x)
    {
        if(x<=vMin)
        {
            return 0.0;
        }
        else
        {
            if(x>=vMax)
                return 1.0;
            else
                return vFac * intg(vMin,x,PDF,1e-4);
        }
    }
    
    double get_final( double y )
    {
        return ceil( solve_final(y) * 2)/2;
    }
    
    
private:
    double shifted_cdf(double x)
    {
        return compute_cdf(x) - target;
    }
    
    double                          target;
    Lua::Function<double>           pdf;
    math::numeric<double>::function PDF;
    math::integrator<double>        intg;
    math::numeric<double>::function ICDF;
    math::zfind<double>             solve;
    
    YOCTO_DISABLE_COPY_AND_ASSIGN(GradeDist);
    
    double solve_final(double y)
    {
        if(y<=0)
            return vMin;
        
        if(y>=1)
            return vMax;
        
        target = y;
        return solve(ICDF,vMin,vMax);
    }
    
};



int main(int argc, char *argv[] )
{
    static const char *progname = vfs::get_base_name(argv[0]);
    try
    {
        //______________________________________________________________________
        //
        // config.lua
        //______________________________________________________________________
        if(argc<=1)
            throw exception("usage: %s config.lua", progname);
        const string cfg = argv[1];
        Lua::State VM;
        lua_State *L = VM();
        Lua::Config::DoFile(L,cfg);
        
        //______________________________________________________________________
        //
        // load all names/data
        //______________________________________________________________________
        const string names = Lua::Config::Get<string>(L, "names");
        std::cerr << "Loading '" << names << "'" << std::endl;
        
        vector<Student> students;
        size_t whom_max = 0;
        {
            ios::icstream fp(names);
            vector<size_t> pos(8,as_capacity);
            string line;
            while( fp.gets(line) )
            {
                std::cerr << '<' << line << '>' << std::endl;
                pos.free();
                for(size_t i=0;i<line.size();++i)
                {
                    if( line[i] == ';' )
                    {
                        line[i] = 0;
                        pos.push_back(i);
                    }
                }
                if(pos.size()<1)
                    throw exception("Invalid line '%s'", line.c_str());
                
                const string name = &line[0];
                const size_t num  = pos.size();
                
                int    whom  = 0;
                double grade = 0;
                for(size_t i=1;i<=num;++i)
                {
                    string word = &line[pos[i]+1];
                    for(size_t j=0;j<=word.size();++j)
                    {
                        if(word[j]==',') word[j] = '.';
                    }
                    //std::cerr << "\tword#" << i << "='" << word << "'" << std::endl;
                    if(word.size()>0)
                    {
                        if(whom>0)
                            throw exception("Multiple Grades for '%s'", name.c_str());
                        whom  = i;
                        grade = strconv::to<double>( word, "grade");
                        if(whom>whom_max)
                            whom_max = whom;
                    }
                }
                const Student s(name,whom,grade);
                students.push_back(s);
            }
        }
        std::cerr << "Got " << students.size() << " students..." << std::endl;
        std::cerr << "Got " << whom_max << " examiners..." << std::endl;
        
        //______________________________________________________________________
        //
        // compute scores
        //______________________________________________________________________
        const size_t   N = students.size();
        vector<double> scores(N,as_capacity);
        
        for(size_t w=1;w<=whom_max;++w)
        {
            vector<double> grades(N,as_capacity);
            for(size_t i=1;i<=N;++i)
            {
                const Student &s = students[i];
                if(s.whom==w)
                {
                    grades.push_back(s.grade);
                }
            }
            const size_t M = grades.size();
            std::cerr << "Examiner" << w << ": " << std::endl;
            std::cerr << "\t" << M << " students" << std::endl;
            if(M>1)
            {
                double ave = 0;
                double sig = 0;
                math::compute_average_and_stddev(ave, sig, grades);
                std::cerr << "\t\tave=" << ave << std::endl;
                std::cerr << "\t\tsig=" << sig << std::endl;
            }
            double median = 0;
            double absdev = 0;
            math::compute_median_and_absdev(median, absdev, grades);
            std::cerr << "\t\tmedian=" << median << std::endl;
            std::cerr << "\t\tabsdev=" << absdev << std::endl;
            
            for(size_t i=1;i<=N;++i)
            {
                Student &s = students[i];
                if(s.whom==w)
                {
                    s.score = (s.grade-median)/absdev;
                    scores.push_back(s.score);
                }
            }
        }
        
        const size_t P = scores.size();
        std::cerr << "#Present=" << P << std::endl;
        
        quicksort(scores);
        vector<double> scr;
        vector<double> cdf;
        
        math::build_cdf(scr, cdf, scores);
        const size_t Q = scr.size();
        for(size_t i=2;i<Q;++i)
        {
            cdf[i] = (cdf[i]-cdf[1])/(cdf[Q]-cdf[1]);
        }
        cdf[1] = 0;
        cdf[Q] = 1;
        
        {
            ios::ocstream fp("scores_cdf.dat", false);
            for(size_t i=1;i<=scr.size();++i)
            {
                fp("%g %g\n", scr[i], cdf[i]);
            }
        }
        
        
        
        {
            ios::ocstream fp("scores.txt", false);
            for(size_t i=1;i<=N;++i)
            {
                const Student &s = students[i];
                if(s.whom<=0)
                {
                    fp << "ABS";
                }
                else
                {
                    fp("%7.3f",s.score);
                }
                fp << ' '  << s.name;
                if(s.whom>0)
                {
                    
                    fp << ' ';
                    fp("(%u,%f)", unsigned(s.whom), s.grade);
                }
                fp << '\n';
            }
        }
        
        //______________________________________________________________________
        //
        // inverse the population
        //______________________________________________________________________

        GradeDist G(L,"pdf");
        {
            ios::ocstream fp("g.dat",false);
            for(size_t i=0;i<=100;++i)
            {
                const double X = G.vMin + (i*(G.vMax-G.vMin))/100;
                fp("%g %g %g\n", X, G.compute_pdf(X), G.compute_cdf(X));
            }
        }
        
        
        vector<double> finals(N,as_capacity);
        
        for(size_t i=1;i<=N;++i)
        {
            Student &s = students[i];
            if(s.whom<=0)
                continue;
            const double value = math::linear(s.score, scr, cdf);
            std::cerr << s.name << ": score=" << s.score << " => cdf=" << value << std::endl;
            const double final = G.get_final(value);
            std::cerr << "\tfinal= " << final << std::endl;
            finals.push_back(final);;
        }
        
        if(finals.size()>1)
        {
            double final_ave  = 0;
            double final_sig  = 0;
            math::compute_average_and_stddev(final_ave, final_sig, finals);
            std::cerr << "Final Average=" << final_ave << std::endl;
            std::cerr << "Final Std_Dev=" << final_sig << std::endl;
        }
        
        vector<double> H(20,0);
        vector<double> bins(20,0);
        for(size_t i=1;i<=20;++i) bins[i] = i;
        math::histogram(H, bins, finals);
        {
            ios::ocstream fp("hist.dat",false);
            
            for(size_t i=1;i<=20;++i)
            {
                fp("%g %g\n", bins[i], H[i]);
            }
        }
        
        
        
        
        return 0;
    }
    catch(const exception &e)
    {
        std::cerr << "*** " << e.what() << std::endl;
        std::cerr << "*** " << e.when() << std::endl;
    }
    catch(...)
    {
        std::cerr << "unhandled exception in " << progname << std::endl;
    }
    return 1;
}